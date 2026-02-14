/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_ops.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 03:35:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/14 03:35:00 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	fd_redir_op_error(char *op)
{
	ft_putstr_fd("minishell: ", 2);
	perror(op);
	g_last_status = 1;
	return (-2);
}

int	fd_errfile_open_error(int saved_stderr, char *errfile)
{
	close(saved_stderr);
	ft_putstr_fd("minishell: ", 2);
	perror(errfile);
	g_last_status = 1;
	return (-2);
}

int	save_and_redirect(int fd, int std_fd)
{
	int	saved_fd;

	saved_fd = dup(std_fd);
	if (saved_fd < 0)
		return (fd_redir_op_error("dup"));
	if (dup2(fd, std_fd) < 0)
	{
		close(saved_fd);
		return (fd_redir_op_error("dup2"));
	}
	return (saved_fd);
}

void	close_saved_fds(int saved_in, int saved_out, int saved_err)
{
	if (saved_in >= 0)
		close(saved_in);
	if (saved_out >= 0)
		close(saved_out);
	if (saved_err >= 0)
		close(saved_err);
}

void	restore_saved_stderr(int saved_stderr)
{
	if (saved_stderr >= 0)
	{
		dup2(saved_stderr, STDERR_FILENO);
		close(saved_stderr);
	}
}
