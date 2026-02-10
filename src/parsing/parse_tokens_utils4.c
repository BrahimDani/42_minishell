/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens_utils4.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 04:50:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/10 17:46:27 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_redir_type(t_cmd *cmd, t_token **token,
		char *joined)
{
	if ((*token)->type == T_REDIR_IN)
		handle_redir_in(cmd, joined);
	else if ((*token)->type == T_REDIR_OUT || (*token)->type == T_APPEND)
		handle_redir_out(cmd, joined, (*token)->type);
}

int	handle_redirection(t_cmd *cmd, t_token **token)
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
	if ((*token)->type == T_HEREDOC)
		handle_heredoc_redir(cmd, joined, heredoc_quoted_any);
	else
		handle_redir_type(cmd, token, joined);
	free(joined);
	*token = t;
	return (1);
}
