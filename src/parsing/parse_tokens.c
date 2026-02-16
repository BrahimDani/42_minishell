/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 17:42:22 by vboxuser          #+#    #+#             */
/*   Updated: 2026/01/05 04:44:48 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	parse_alloc_error(t_cmd *head, t_shell *sh)
{
	if (head)
		free_cmds(head);
	perror("minishell: malloc");
	ms_status_set(sh, 1);
	return (-1);
}

static void	apply_redirection(t_token **tokens, t_cmd *current,
	char *joined, int heredoc_quoted_any)
{
	if ((*tokens)->type == T_HEREDOC)
		handle_heredoc_redir(current, joined, heredoc_quoted_any);
	else if ((*tokens)->type == T_REDIR_IN)
		handle_redir_in(current, joined);
	else
		handle_redir_out(current, joined, (*tokens)->type);
}

static int	handle_redirection_token(t_token **tokens, t_cmd *current,
	t_shell *sh)
{
	t_token	*t;
	char	*joined;
	int		heredoc_quoted_any;

	if (!check_redir_syntax(*tokens, sh))
		return (0);
	t = (*tokens)->next;
	heredoc_quoted_any = 0;
	if ((*tokens)->type == T_HEREDOC)
		heredoc_quoted_any = aggregate_quoted(t);
	joined = join_adjacent_words(&t);
	if (!joined)
	{
		perror("minishell: malloc");
		ms_status_set(sh, 1);
		return (0);
	}
	apply_redirection(tokens, current, joined, heredoc_quoted_any);
	free(joined);
	*tokens = t;
	return (1);
}

static int	process_token(t_token **tokens, t_cmd **current,
	t_cmd *head, t_shell *sh)
{
	if ((*tokens)->type == T_WORD)
	{
		if (!handle_word_token(tokens, *current))
			return (parse_alloc_error(head, sh));
	}
	else if ((*tokens)->type == T_REDIR_IN || (*tokens)->type == T_REDIR_OUT
		|| (*tokens)->type == T_APPEND || (*tokens)->type == T_HEREDOC)
	{
		if (!handle_redirection_token(tokens, *current, sh))
		{
			if (head)
				free_cmds(head);
			return (-1);
		}
	}
	else if ((*tokens)->type == T_PIPE)
	{
		*current = create_new_cmd(&head);
		if (!*current)
			return (parse_alloc_error(head, sh));
	}
	return (1);
}

t_cmd	*parse_tokens(t_token *tokens, t_shell *sh)
{
	t_cmd	*head;
	t_cmd	*current;
	int		status;

	head = NULL;
	current = NULL;
	while (tokens)
	{
		if (!current)
		{
			current = create_new_cmd(&head);
			if (!current)
			{
				parse_alloc_error(head, sh);
				return (NULL);
			}
		}
		status = process_token(&tokens, &current, head, sh);
		if (status == -1)
			return (NULL);
		tokens = tokens->next;
	}
	return (head);
}
