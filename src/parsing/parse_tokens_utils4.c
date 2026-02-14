/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens_utils4.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 04:50:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/05 04:44:48 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_redir_type(t_cmd *cmd, t_token **token,
		char *joined, int is_stderr)
{
	if ((*token)->type == T_REDIR_IN)
		handle_redir_in(cmd, joined);
	else if ((*token)->type == T_REDIR_OUT || (*token)->type == T_APPEND)
	{
		if (is_stderr)
			handle_stderr_redir(cmd, joined, (*token)->type == T_APPEND);
		else
			handle_redir_out(cmd, joined, (*token)->type);
	}
}

int	handle_redirection(t_cmd *cmd, t_token **token, t_shell *sh)
{
	t_token	*t;
	char	*joined;
	int		heredoc_quoted_any;
	int		is_stderr;

	is_stderr = 0;
	if ((*token)->value && (*token)->value[0] == '2'
		&& ((*token)->type == T_REDIR_OUT || (*token)->type == T_APPEND))
		is_stderr = 1;
	t = *token;
	heredoc_quoted_any = 0;
	if (!check_redir_syntax(t, sh))
		return (0);
	t = t->next;
	if ((*token)->type == T_HEREDOC)
		heredoc_quoted_any = aggregate_quoted(t);
	joined = join_adjacent_words(&t);
	if ((*token)->type == T_HEREDOC)
		handle_heredoc_redir(cmd, joined, heredoc_quoted_any);
	else
		handle_redir_type(cmd, token, joined, is_stderr);
	free(joined);
	*token = t;
	return (1);
}

static int	handle_stderr_simple(t_token *token, t_cmd *current, t_token **next)
{
	if (token->value && token->value[0] == '2' && token->value[1])
	{
		if ((token->value[1] == '>' || token->value[1] == '<')
			&& token->next && token->next->type == T_WORD)
		{
			if (token->value[1] == '>' && token->value[2] == '>')
				handle_stderr_redir(current, token->next->value, 1);
			else if (token->value[1] == '>')
				handle_stderr_redir(current, token->next->value, 0);
			current->redirect_stderr_to_out = 0;
			*next = token->next;
			return (1);
		}
	}
	return (0);
}

static int	handle_stderr_complex(t_token *token, t_cmd *current,
		t_token **next)
{
	if (token->next && (token->next->type == T_REDIR_OUT
			|| token->next->type == T_APPEND)
		&& token->next->space_before == 0)
	{
		if (is_all_digits(token->value) && ft_atoi(token->value) == 2)
		{
			if (token->next->next && token->next->next->type == T_WORD)
			{
				handle_stderr_redir(current, token->next->next->value,
					token->next->type == T_APPEND);
				current->redirect_stderr_to_out = 0;
				*next = token->next->next;
				return (1);
			}
		}
	}
	return (0);
}

int	is_stderr_redirect(t_token *token, t_cmd *current, t_token **next)
{
	if (handle_stderr_simple(token, current, next))
		return (1);
	return (handle_stderr_complex(token, current, next));
}
