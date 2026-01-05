/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 04:50:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/05 04:42:06 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*join_adjacent_words(t_token **token)
{
	char	*result;
	char	*tmp;
	t_token	*current;

	result = ft_strdup((*token)->value);
	current = (*token)->next;
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

int	check_redir_syntax(t_token *t)
{
	if (!t->next || t->next->type != T_WORD)
	{
		if (!t->next)
			ft_putendl_fd("minishell: syntax error near unexpected"
				" token `newline'", 2);
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

void	handle_redir_in(t_cmd *cmd, char *joined)
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

void	handle_prev_outfile(t_cmd *cmd)
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

void	handle_redir_out(t_cmd *cmd, char *joined, t_token_type type)
{
	if (cmd->outfile)
	{
		handle_prev_outfile(cmd);
		free(cmd->outfile);
	}
	cmd->outfile = ft_strdup(joined);
	cmd->append = (type == T_APPEND);
}
