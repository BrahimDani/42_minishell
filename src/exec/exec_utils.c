/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 14:07:34 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/10 20:26:36 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	run_command_child(t_cmd *cmd, t_env **env_list)
{
	if (!cmd || !cmd->argv)
		return (0);
	if (!cmd->argv[0])
		return (0);
	if (cmd->argv[0][0] == '\0')
	{
		ft_putstr_fd("minishell: : command not found\n", 2);
		return (127);
	}
	if (ft_strcmp(cmd->argv[0], "env") == 0 && cmd->argv[1] != NULL)
	{
		exec_external_child(cmd->argv, env_list);
		return (127);
	}
	if (is_builtin(cmd->argv[0]))
	{
		if (ft_strcmp(cmd->argv[0], "exit") == 0)
			return (ft_exit_status(cmd->argv));
		return (exec_builtin(cmd->argv, env_list, 0));
	}
	exec_external_child(cmd->argv, env_list);
	return (127);
}

static void	update_underscore_var(t_cmd *cmd, t_env **env_list)
{
	int	i;

	if (!is_builtin(cmd->argv[0]))
	{
		i = 0;
		while (cmd->argv[i])
			i++;
		if (i > 0)
			set_env_value(env_list, "_", cmd->argv[i - 1]);
	}
}

static void	close_cmd_heredoc_fd(t_cmd *cmd)
{
	if (cmd && cmd->heredoc_fd >= 0)
	{
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
}

static void	execute_single_cmd(t_cmd *cmd, t_env **env_list, t_shell *sh)
{
	t_saved_fds	saved;
	int			saved_stderr;

	if (check_redir_errors(cmd, sh))
		return (close_cmd_heredoc_fd(cmd));
	if (setup_redirections(cmd, env_list, &saved, sh) == -1)
		return (close_cmd_heredoc_fd(cmd));
	saved_stderr = setup_stderr_redir(cmd, sh);
	if (saved_stderr == -2)
		return (restore_fds(saved.in, saved.out));
	if (cmd->argv && cmd->argv[0])
	{
		ms_status_set(sh, run_command(cmd, env_list, sh));
		update_underscore_var(cmd, env_list);
	}
	else
		ms_status_set(sh, 0);
	restore_saved_stderr(saved_stderr);
	restore_fds(saved.in, saved.out);
}

void	exec_cmd_list(t_cmd *cmd_list, t_env **env_list, t_shell *sh)
{
	t_cmd	*current;

	current = cmd_list;
	if (count_cmds(cmd_list) == 1)
	{
		execute_single_cmd(current, env_list, sh);
		close_parent_heredocs(cmd_list);
		return ;
	}
	execute_pipeline(cmd_list, env_list, count_cmds(cmd_list), sh);
	return ;
}
