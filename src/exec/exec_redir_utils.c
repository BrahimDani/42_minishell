/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:11:54 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/03 20:11:11 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>

int	check_redir_errors(t_cmd *cmd)
{
	if (cmd->has_in_redir_error && cmd->in_redir_first_error)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->in_redir_first_error);
		g_last_status = 1;
		return (1);
	}
	if (cmd->has_out_redir_error && cmd->out_redir_first_error)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->out_redir_first_error);
		g_last_status = 1;
		return (1);
	}
	return (0);
}

int	setup_redirections(t_cmd *cmd, t_env **env_list,
		int *saved_in, int *saved_out)
{
	*saved_in = handle_input_redir(cmd, env_list);
	if (*saved_in == -2)
		return (-1);
	*saved_out = handle_output_redir(cmd);
	if (*saved_out == -2)
	{
		restore_fds(*saved_in, -1);
		return (-1);
	}
	return (0);
}

static int	open_err_fd(t_cmd *cmd)
{
	int	err_fd;

	if (cmd->err_append)
		err_fd = open(cmd->errfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		err_fd = open(cmd->errfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	return (err_fd);
}

static int	handle_errfile(t_cmd *cmd)
{
	int	saved_stderr;
	int	err_fd;

	saved_stderr = dup(STDERR_FILENO);
	if (saved_stderr < 0)
		return (fd_redir_op_error("dup"));
	err_fd = open_err_fd(cmd);
	if (err_fd < 0)
		return (fd_errfile_open_error(saved_stderr, cmd->errfile));
	if (dup2(err_fd, STDERR_FILENO) < 0)
	{
		close(err_fd);
		close(saved_stderr);
		return (fd_redir_op_error("dup2"));
	}
	close(err_fd);
	return (saved_stderr);
}

int	setup_stderr_redir(t_cmd *cmd)
{
	int	saved_stderr;

	saved_stderr = -1;
	if (cmd->errfile)
		return (handle_errfile(cmd));
	else if (cmd->redirect_stderr_to_out)
	{
		saved_stderr = dup(STDERR_FILENO);
		if (saved_stderr < 0)
			return (fd_redir_op_error("dup"));
		if (dup2(STDOUT_FILENO, STDERR_FILENO) < 0)
		{
			close(saved_stderr);
			return (fd_redir_op_error("dup2"));
		}
	}
	return (saved_stderr);
}
