/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 04:30:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/09 11:56:00 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	process_line_expanded(char *line, int fd, char *delim, t_env *env)
{
	char	*expanded_line;

	if (ft_strcmp(line, delim) == 0)
	{
		free(line);
		return (0);
	}
	expanded_line = expand_heredoc(line, env);
	write(fd, expanded_line, ft_strlen(expanded_line));
	write(fd, "\n", 1);
	free(expanded_line);
	free(line);
	return (1);
}

int	process_line_raw(char *line, int fd, char *delim)
{
	if (ft_strcmp(line, delim) == 0)
	{
		free(line);
		return (0);
	}
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
	return (1);
}

void	pre_read_one(t_cmd *cmd, t_env *env_list)
{
	int	fd;

	if (cmd->heredoc && cmd->heredoc_fd < 0)
	{
		fd = read_heredoc(cmd->infile, env_list, cmd->heredoc_quoted);
		if (fd >= 0)
			cmd->heredoc_fd = fd;
	}
}
