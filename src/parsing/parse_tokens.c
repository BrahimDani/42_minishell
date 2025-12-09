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

static void	init_cmd_fields(t_cmd *cmd)
{
	cmd->argv = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->heredoc_fd = -1;
	cmd->heredoc_quoted = 0;
	cmd->next = NULL;
	cmd->redirect_stderr_to_out = 0;
	cmd->has_in_redir_error = 0;
	cmd->in_redir_first_error = NULL;
	cmd->has_out_redir_error = 0;
	cmd->out_redir_first_error = NULL;
}

static void	add_cmd_to_list(t_cmd **head, t_cmd *new)
{
	t_cmd	*tmp;

	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

static t_cmd	*create_new_cmd(t_cmd **head)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	init_cmd_fields(new);
	add_cmd_to_list(head, new);
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

static int	check_redir_syntax(t_token *t)
{
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
	return (1);
}

static void	handle_redir_in(t_cmd *cmd, char *joined)
{
	int	fd_test;

	if (!cmd->has_in_redir_error)
	{
		fd_test = open(joined, O_RDONLY);
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

static void	handle_prev_outfile(t_cmd *cmd)
{
	int	fd_prev;

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
}

static void	handle_redir_out(t_cmd *cmd, char *joined, t_token_type type)
{
	if (cmd->outfile)
	{
		handle_prev_outfile(cmd);
		free(cmd->outfile);
	}
	cmd->outfile = ft_strdup(joined);
	cmd->append = (type == T_APPEND);
}

static void	handle_heredoc_redir(t_cmd *cmd, char *joined, int quoted)
{
	if (cmd->infile)
		free(cmd->infile);
	cmd->infile = ft_strdup(joined);
	cmd->heredoc = 1;
	cmd->heredoc_quoted = quoted;
}

static int	handle_redirection(t_cmd *cmd, t_token **token)
{
	t_token	*t;
	char	*joined;
	int		heredoc_quoted_any;

	t = *token;
	heredoc_quoted_any = 0;
	if (!check_redir_syntax(t))
		return (0);
	t = t->next;
	if ((*token)->type == T_HEREDOC)
		heredoc_quoted_any = aggregate_quoted(t);
	joined = join_adjacent_words(&t);
	if ((*token)->type == T_REDIR_IN)
		handle_redir_in(cmd, joined);
	else if ((*token)->type == T_REDIR_OUT || (*token)->type == T_APPEND)
		handle_redir_out(cmd, joined, (*token)->type);
	else if ((*token)->type == T_HEREDOC)
		handle_heredoc_redir(cmd, joined, heredoc_quoted_any);
	free(joined);
	*token = t;
	return (1);
}

static int	is_all_digits(char *str)
{
	int	j;

	j = 0;
	while (str[j])
	{
		if (!ft_isdigit((unsigned char)str[j]))
			return (0);
		j++;
	}
	return (1);
}

static int	is_stderr_redirect(t_token *token, t_cmd *current, t_token **next)
{
	if (token->next && (token->next->type == T_REDIR_OUT
			|| token->next->type == T_APPEND)
		&& token->next->space_before == 0)
	{
		if (is_all_digits(token->value) && ft_atoi(token->value) == 2)
		{
			current->redirect_stderr_to_out = 1;
			*next = token->next;
			return (1);
		}
	}
	return (0);
}

static void	handle_word_token(t_token **tokens, t_cmd *current)
{
	char	*joined_value;

	joined_value = join_adjacent_words(tokens);
	add_argument(current, joined_value);
	free(joined_value);
}

static int	handle_redir_token(t_token **tokens, t_cmd *current, t_cmd *head)
{
	if (!handle_redirection(current, tokens))
	{
		if (head)
			free_cmds(head);
		return (0);
	}
	return (1);
}

static int	process_token(t_token **tokens, t_cmd **current, t_cmd *head)
{
	if ((*tokens)->type == T_WORD)
	{
		if (is_stderr_redirect(*tokens, *current, tokens))
			return (1);
		handle_word_token(tokens, *current);
	}
	else if ((*tokens)->type == T_REDIR_IN || (*tokens)->type == T_REDIR_OUT
		|| (*tokens)->type == T_APPEND || (*tokens)->type == T_HEREDOC)
	{
		if (!handle_redir_token(tokens, *current, head))
			return (-1);
	}
	else if ((*tokens)->type == T_PIPE)
		*current = create_new_cmd(&head);
	return (1);
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*current;
	int		status;

	head = NULL;
	current = NULL;
	while (tokens)
	{
		if (!current)
			current = create_new_cmd(&head);
		status = process_token(&tokens, &current, head);
		if (status == -1)
			return (NULL);
		tokens = tokens->next;
	}
	return (head);
}
