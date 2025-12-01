/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 17:42:22 by vboxuser          #+#    #+#             */
/*   Updated: 2025/12/01 17:32:35 by kadrouin         ###   ########.fr       */
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
	new->next = NULL;
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

// gere les redir 
static void	handle_redirection(t_cmd *cmd, t_token **token)
{
	t_token	*t = *token;

	if (!t->next || t->next->type != T_WORD)
	{
		ft_putendl_fd("minishell: syntax error near redirection", 2);
		g_last_status = 2;
		return ;
	}
	t = t->next;
	if ((*token)->type == T_REDIR_IN)
		cmd->infile = ft_strdup(t->value);
	else if ((*token)->type == T_REDIR_OUT)
		cmd->outfile = ft_strdup(t->value);
	else if ((*token)->type == T_APPEND)
	{
		cmd->outfile = ft_strdup(t->value);
		cmd->append = 1;
	}
	else if ((*token)->type == T_HEREDOC)
	{
		cmd->infile = ft_strdup(t->value);
		cmd->heredoc = 1;
	}
	*token = t;
}

t_cmd   *parse_tokens(t_token *tokens)
{
	t_cmd	*head = NULL;
	t_cmd	*current = NULL;

	while (tokens)
	{
		if (!current)
			current = create_new_cmd(&head);
		if (tokens->type == T_WORD)
			add_argument(current, tokens->value);
		else if (tokens->type == T_REDIR_IN || tokens->type == T_REDIR_OUT
			|| tokens->type == T_APPEND || tokens->type == T_HEREDOC)
			handle_redirection(current, &tokens);
			else if (tokens->type == T_PIPE)
			current = create_new_cmd(&head);
		tokens = tokens->next;
	}
	return (head);
}
