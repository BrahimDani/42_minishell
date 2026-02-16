/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 18:18:50 by vboxuser          #+#    #+#             */
/*   Updated: 2026/02/14 17:05:00 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_heredoc_sigint(char *line, t_heredoc_ctx *ctx)
{
	if (consume_sigint_flag() == SIGINT)
	{
		if (line)
			free(line);
		ms_status_set(ctx->sh, 130);
		return (1);
	}
	return (0);
}

static void	restore_stdin_and_signal(int saved_stdin)
{
	if (saved_stdin >= 0)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	setup_prompt_signals();
}

static int	cancel_heredoc(t_heredoc_run *run, char *clean_delim)
{
	close(run->fd);
	unlink(run->path);
	free(clean_delim);
	restore_stdin_and_signal(run->saved_stdin);
	return (-2);
}

int	read_heredoc_content(int fd, char *delim, int expand,
	t_heredoc_ctx *ctx)
{
	char	*line;

	while (1)
	{
		line = read_heredoc_line();
		if (handle_heredoc_sigint(line, ctx))
			return (0);
		if (!line)
			return (1);
		if (expand && !process_line_expanded(line, fd, delim, ctx))
			return (1);
		if (!expand && !process_line_raw(line, fd, delim))
			return (1);
	}
	return (1);
}

int	read_heredoc(char *delimiter, t_env *env_list, int quoted, t_shell *sh)
{
	t_heredoc_run	run;
	char			*clean_delim;

	clean_delim = process_delimiter(delimiter, &run.should_expand, quoted);
	run.ctx.env = env_list;
	run.ctx.sh = sh;
	run.fd = create_tmpfile(run.path, 50);
	if (run.fd == -1)
	{
		perror("minishell: heredoc");
		free(clean_delim);
		return (-1);
	}
	consume_sigint_flag();
	run.saved_stdin = dup(STDIN_FILENO);
	setup_heredoc_signals();
	if (!read_heredoc_content(run.fd, clean_delim, run.should_expand, &run.ctx))
		return (cancel_heredoc(&run, clean_delim));
	close(run.fd);
	run.fd = open(run.path, O_RDONLY);
	unlink(run.path);
	free(clean_delim);
	restore_stdin_and_signal(run.saved_stdin);
	return (run.fd);
}
