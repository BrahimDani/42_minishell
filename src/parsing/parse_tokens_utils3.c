/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens_utils3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 04:50:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/10 17:53:42 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_heredoc_redir(t_cmd *cmd, char *joined, int quoted)
{
	if (cmd->infile)
		free(cmd->infile);
	cmd->infile = ft_strdup(joined);
	cmd->heredoc = 1;
	cmd->heredoc_quoted = quoted;
}

int	is_all_digits(char *str)
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

void	handle_word_token(t_token **tokens, t_cmd *current)
{
	char	*joined_value;

	joined_value = join_adjacent_words(tokens);
	add_argument(current, joined_value);
	free(joined_value);
}

int	handle_redir_token(t_token **tokens, t_cmd *current, t_cmd *head)
{
	if (!handle_redirection(current, tokens))
	{
		if (head)
			free_cmds(head);
		return (0);
	}
	return (1);
}
