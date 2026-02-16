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

int	process_line_expanded(char *line, int fd, char *delim, t_heredoc_ctx *ctx)
{
	char	*expanded_line;

	if (!delim)
	{
		free(line);
		return (-1);
	}
	if (ft_strcmp(line, delim) == 0)
	{
		free(line);
		return (0);
	}
	expanded_line = expand_heredoc(line, ctx->env, ctx->sh);
	if (!expanded_line)
	{
		free(line);
		return (-1);
	}
	write(fd, expanded_line, ft_strlen(expanded_line));
	write(fd, "\n", 1);
	free(expanded_line);
	free(line);
	return (1);
}

int	process_line_raw(char *line, int fd, char *delim)
{
	if (!delim)
	{
		free(line);
		return (-1);
	}
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

char	*init_heredoc_run(t_heredoc_run *run, char *delimiter, int quoted,
	t_shell *sh)
{
	char	*clean_delim;

	clean_delim = process_delimiter(delimiter, &run->should_expand, quoted);
	if (!clean_delim)
	{
		perror("minishell: malloc");
		ms_status_set(sh, 1);
		return (NULL);
	}
	run->fd = create_tmpfile(run->path, 50);
	if (run->fd == -1)
	{
		perror("minishell: heredoc");
		free(clean_delim);
		return (NULL);
	}
	return (clean_delim);
}

int	pre_read_one(t_cmd *cmd, t_env *env_list, t_shell *sh)
{
	int	fd;

	if (cmd->heredoc && cmd->heredoc_fd < 0)
	{
		fd = read_heredoc(cmd->infile, env_list, cmd->heredoc_quoted, sh);
		if (fd == -2)
			return (0);
		if (fd >= 0)
			cmd->heredoc_fd = fd;
	}
	return (1);
}
