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

int	fd_redir_op_error(char *op, t_shell *sh)
{
	ft_putstr_fd("minishell: ", 2);
	perror(op);
	ms_status_set(sh, 1);
	return (-2);
}

int	save_and_redirect(int fd, int std_fd, t_shell *sh)
{
	int	saved_fd;

	saved_fd = dup(std_fd);
	if (saved_fd < 0)
		return (fd_redir_op_error("dup", sh));
	if (dup2(fd, std_fd) < 0)
	{
		close(saved_fd);
		return (fd_redir_op_error("dup2", sh));
	}
	return (saved_fd);
}

void	close_saved_fds(int saved_in, int saved_out)
{
	if (saved_in >= 0)
		close(saved_in);
	if (saved_out >= 0)
		close(saved_out);
}
