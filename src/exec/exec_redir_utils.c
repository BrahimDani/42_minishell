/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:11:54 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/16 05:10:00 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_redir_errors(t_cmd *cmd, t_shell *sh)
{
	if (cmd->has_in_redir_error && cmd->in_redir_first_error)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->in_redir_first_error);
		ms_status_set(sh, 1);
		return (1);
	}
	if (cmd->has_out_redir_error && cmd->out_redir_first_error)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->out_redir_first_error);
		ms_status_set(sh, 1);
		return (1);
	}
	return (0);
}

int	setup_redirections(t_cmd *cmd, t_env **env_list,
	t_saved_fds *saved, t_shell *sh)
{
	saved->in = handle_input_redir(cmd, env_list, sh);
	if (saved->in == -2)
		return (-1);
	saved->out = handle_output_redir(cmd, sh);
	if (saved->out == -2)
	{
		restore_fds(saved->in, -1);
		return (-1);
	}
	return (0);
}
