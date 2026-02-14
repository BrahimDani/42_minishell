/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:11:54 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/08 00:17:36 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>

int	handle_heredoc_input(t_cmd *cmd, t_env *env_list, t_shell *sh)
{
	int	fd_in;

	if (cmd->heredoc_fd >= 0)
		fd_in = cmd->heredoc_fd;
	else
		fd_in = read_heredoc(cmd->infile, env_list, cmd->heredoc_quoted, sh);
	if (fd_in < 0)
	{
		if (!(fd_in == -2 && ms_status_get(sh) == 130))
			ms_status_set(sh, 1);
		return (-2);
	}
	return (fd_in);
}

int	handle_file_input(char *infile, t_shell *sh)
{
	int	fd_in;

	fd_in = open(infile, O_RDONLY);
	if (fd_in < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(infile);
		ms_status_set(sh, 1);
		return (-2);
	}
	return (fd_in);
}

int	handle_input_redir(t_cmd *cmd, t_env **env_list, t_shell *sh)
{
	int	fd_in;
	int	saved_stdin;

	if (!cmd->infile && !cmd->heredoc)
		return (-1);
	if (cmd->heredoc)
		fd_in = handle_heredoc_input(cmd, *env_list, sh);
	else
		fd_in = handle_file_input(cmd->infile, sh);
	if (fd_in < 0)
		return (fd_in);
	saved_stdin = save_and_redirect(fd_in, STDIN_FILENO, sh);
	close(fd_in);
	if (cmd->heredoc && cmd->heredoc_fd == fd_in)
		cmd->heredoc_fd = -1;
	if (saved_stdin < 0)
		return (-2);
	return (saved_stdin);
}

int	handle_output_redir(t_cmd *cmd, t_shell *sh)
{
	int	fd_out;
	int	saved_stdout;

	if (!cmd->outfile)
		return (-1);
	if (cmd->append)
		fd_out = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd_out = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->outfile);
		ms_status_set(sh, 1);
		return (-2);
	}
	saved_stdout = save_and_redirect(fd_out, STDOUT_FILENO, sh);
	close(fd_out);
	if (saved_stdout < 0)
		return (-2);
	return (saved_stdout);
}

void	restore_fds(int saved_stdin, int saved_stdout)
{
	if (saved_stdin >= 0)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout >= 0)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}
