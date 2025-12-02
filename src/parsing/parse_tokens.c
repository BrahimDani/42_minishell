/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 17:42:22 by vboxuser          #+#    #+#             */
/*   Updated: 2025/12/02 18:01:48 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// cree une nouvelle cmd et l'add a la liste
static t_cmd	*create_new_cmd(t_cmd **head)
{
	t_cmd	*new = malloc(sizeof(t_cmd));
	t_cmd	*tmp;

	if(!new)
		return (NULL);
	new->argv = NULL;
	new->infile = NULL;
	new->outfile = NULL;
	new->append = 0;
	new->heredoc = 0;
	new->heredoc_fd = -1;
	new->heredoc_quoted = 0;
	new->next = NULL;
	new->redirect_stderr_to_out = 0;
	new->has_in_redir_error = 0;
	new->in_redir_first_error = NULL;
	new->has_out_redir_error = 0;
	new->out_redir_first_error = NULL;
	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (new);
}
// ajoute un mot a argv tah la struct
static void	add_argument(t_cmd *cmd, char *value)
{
	int		i;
	char	**new_argv;

	i = 0;
	if (cmd->argv)
	{
		while (cmd->argv[i])
			i++;
	}
	new_argv = malloc(sizeof(char *) * (i + 2));
	if (!new_argv)
		return ;
	i = 0;
	if (cmd->argv)
	{
		while (cmd->argv[i])
		{
			new_argv[i] = cmd->argv[i];
			i++;
		}
		free(cmd->argv);
	}
	new_argv[i++] = ft_strdup(value);
	new_argv[i] = NULL;
	cmd->argv = new_argv;
}

// Concatène les tokens T_WORD adjacents (pour gérer echo $?"42" -> "042")
static int	aggregate_quoted(t_token *start)
{
	int quoted = 0;
	t_token *cur = start;
	if (!cur) return 0;
	while (cur && cur->type == T_WORD)
	{
		if (cur->was_quoted || cur->no_expand)
		{
			quoted = 1;
		}
		if (!cur->next || cur->next->space_before)
			break;
		cur = cur->next;
	}
	return quoted;
}

static char	*join_adjacent_words(t_token **token)
{
	char	*result;
	char	*tmp;
	t_token	*current;

	result = ft_strdup((*token)->value);
	current = (*token)->next;
	
	// Tant que le prochain token est un T_WORD SANS espace avant, on le concatène
	while (current && current->type == T_WORD && !current->space_before)
	{
		tmp = result;
		result = ft_strjoin(tmp, current->value);
		free(tmp);
		*token = current;
		current = current->next;
	}
	return (result);
}

// gere les redir 
static int	handle_redirection(t_cmd *cmd, t_token **token)
{
	t_token	*t = *token;
	char    *joined;
	int     heredoc_quoted_any = 0;

	if (!t->next || t->next->type != T_WORD)
	{
		if (!t->next)
			ft_putendl_fd("minishell: syntax error near unexpected token `newline'", 2);
		else if (t->next->value)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
			ft_putstr_fd(t->next->value, 2);
			ft_putendl_fd("'", 2);
		}
		else
			ft_putendl_fd("minishell: syntax error near unexpected token`'", 2);
		g_last_status = 2;
		return (0);
	}
	t = t->next;
	if ((*token)->type == T_HEREDOC)
		heredoc_quoted_any = aggregate_quoted(t);
	joined = join_adjacent_words(&t);
	// Conserve les anciens champs pour compatibilite (dernier prend effet)
    if ((*token)->type == T_REDIR_IN)
	{
		// Enregistre la premiere erreur d'ouverture pour respecter l'ordre gauche->droite
		if (!cmd->has_in_redir_error)
		{
			int fd_test = open(joined, O_RDONLY);
			if (fd_test < 0)
			{
				cmd->has_in_redir_error = 1;
				cmd->in_redir_first_error = ft_strdup(joined);
			}
			else
				close(fd_test);
		}
		if (cmd->infile)
			free(cmd->infile);
		cmd->infile = ft_strdup(joined);
	}
    else if ((*token)->type == T_REDIR_OUT || (*token)->type == T_APPEND)
	{
		/* If an earlier output redirection existed, emulate bash side-effect by creating it */
		if (cmd->outfile)
		{
			int fd_prev;
			if (cmd->append)
				fd_prev = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else
				fd_prev = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd_prev < 0 && !cmd->has_out_redir_error)
			{
				cmd->has_out_redir_error = 1;
				cmd->out_redir_first_error = ft_strdup(cmd->outfile);
			}
			else if (fd_prev >= 0)
				close(fd_prev);
			free(cmd->outfile);
		}
		cmd->outfile = ft_strdup(joined);
		cmd->append = ((*token)->type == T_APPEND);
	}
    else if ((*token)->type == T_HEREDOC)
	{
		if (cmd->infile)
			free(cmd->infile);
		cmd->infile = ft_strdup(joined);
		cmd->heredoc = 1;
		cmd->heredoc_quoted = heredoc_quoted_any;
	}
	free(joined);
	*token = t;
	return (1);
}

t_cmd   *parse_tokens(t_token *tokens)
{
	t_cmd	*head = NULL;
	t_cmd	*current = NULL;
	char	*joined_value;

	while (tokens)
	{
		if (!current)
			current = create_new_cmd(&head);
		if (tokens->type == T_WORD)
		{
			// Detect IO number '2' before redirection with no space (e.g., 2>)
			if (tokens->next && (tokens->next->type == T_REDIR_OUT
					|| tokens->next->type == T_APPEND)
				&& tokens->next->space_before == 0)
			{
				int j = 0;
				int all_digits = 1;
				while (tokens->value[j])
				{
					if (!ft_isdigit((unsigned char)tokens->value[j]))
					{
						all_digits = 0;
						break;
					}
					j++;
				}
				if (all_digits && ft_atoi(tokens->value) == 2)
				{
					current->redirect_stderr_to_out = 1;
					tokens = tokens->next;
					continue;
				}
			}
			// Joindre les tokens T_WORD adjacents (sans espace entre eux)
			joined_value = join_adjacent_words(&tokens);
			add_argument(current, joined_value);
			free(joined_value);
		}
		else if (tokens->type == T_REDIR_IN || tokens->type == T_REDIR_OUT
			|| tokens->type == T_APPEND || tokens->type == T_HEREDOC)
		{
			if (!handle_redirection(current, &tokens))
			{
				/* Syntax error: free any partially built command list to avoid leaks */
				if (head)
					free_cmds(head);
				return (NULL);
			}
		}
		else if (tokens->type == T_PIPE)
			current = create_new_cmd(&head);
		tokens = tokens->next;
	}
	return (head);
}
