/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:11:54 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/10 17:47:14 by kadrouin         ###   ########.fr       */
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
