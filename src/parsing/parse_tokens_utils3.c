/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens_utils3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 04:50:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/05 04:42:06 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_stderr_redir(t_cmd *cmd, char *joined, int is_append)
{
	if (cmd->errfile)
		free(cmd->errfile);
	cmd->errfile = ft_strdup(joined);
	cmd->err_append = is_append;
}

void	handle_heredoc_redir(t_cmd *cmd, char *joined, int quoted)
{
	if (cmd->infile)
		free(cmd->infile);
	cmd->infile = ft_strdup(joined);
	cmd->heredoc = 1;
	cmd->heredoc_quoted = quoted;
}

void	handle_word_token(t_token **tokens, t_cmd *current)
{
	char	*joined_value;

	joined_value = join_adjacent_words(tokens);
	add_argument(current, joined_value);
	free(joined_value);
}

int	handle_redir_token(t_token **tokens, t_cmd *current,
	t_cmd *head, t_shell *sh)
{
	if (!handle_redirection(current, tokens, sh))
	{
		if (head)
			free_cmds(head);
		return (0);
	}
	return (1);
}
