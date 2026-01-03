/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:11:54 by kadrouin          #+#    #+#             */
/*   Updated: 2025/08/26 14:11:54 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <errno.h>
#include <string.h>
#include <fcntl.h>

int	exec_external(char **argv, char **envp, t_env **env_list)
{
	if (ft_strchr(argv[0], '/'))
		return (exec_absolute_path(argv, envp, *env_list));
	else
		return (exec_from_path(argv, envp, *env_list));
}

/**
 * run_command - dispatch a parsed command (builtin or external)
 * Uses the same behavior as the previous main.c implementation.
 */
int	run_command(t_cmd *cmd, t_env **env_list, char **envp)
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
		return (exec_external(cmd->argv, envp, env_list));
	}
	if (is_builtin(cmd->argv[0]))
	{
		if (ft_strcmp(cmd->argv[0], "exit") == 0)
			return (ft_exit(cmd->argv));
		return (exec_builtin(cmd->argv, env_list));
	}
	return (exec_external(cmd->argv, envp, env_list));
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

static void	execute_single_cmd(t_cmd *cmd, t_env **env_list, char **envp)
{
	int	saved_stdin;
	int	saved_stdout;
	int	saved_stderr;

	if (check_redir_errors(cmd))
		return ;
	if (setup_redirections(cmd, env_list, &saved_stdin, &saved_stdout) == -1)
		return ;
	saved_stderr = setup_stderr_redir(cmd);
	if (cmd->argv && cmd->argv[0])
	{
		g_last_status = run_command(cmd, env_list, envp);
		update_underscore_var(cmd, env_list);
	}
	else
		g_last_status = 0;
	if (saved_stderr >= 0)
	{
		dup2(saved_stderr, STDERR_FILENO);
		close(saved_stderr);
	}
	restore_fds(saved_stdin, saved_stdout);
}

void	exec_cmd_list(t_cmd *cmd_list, t_env **env_list, char **envp)
{
	t_cmd	*current;

	current = cmd_list;
	if (count_cmds(cmd_list) == 1)
	{
		execute_single_cmd(current, env_list, envp);
		return ;
	}
	execute_pipeline(cmd_list, env_list, envp, count_cmds(cmd_list));
	return ;
}
