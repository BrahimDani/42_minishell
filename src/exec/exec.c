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

static void	exec_absolute_path_child(char **argv, t_env *env_list)
{
	if (access(argv[0], F_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		ms_exit(127, env_list);
	}
	if (access(argv[0], X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
		ms_exit(126, env_list);
	}
	child_process(argv[0], argv, env_list);
}

static void	exec_from_path_child(char **argv, t_env *env_list)
{
	char	*full_path;

	if (check_special_dirs(argv[0]))
		ms_exit(127, env_list);
	full_path = find_command_path(argv[0], env_list);
	if (!full_path)
	{
		print_cmd_error(argv[0], "command not found");
		ms_exit(127, env_list);
	}
	if (access(full_path, X_OK) != 0)
	{
		print_cmd_error(argv[0], "Permission denied");
		free(full_path);
		ms_exit(126, env_list);
	}
	child_process(full_path, argv, env_list);
}

static void	exec_external_child(char **argv, t_env **env_list)
{
	if (ft_strchr(argv[0], '/'))
		exec_absolute_path_child(argv, *env_list);
	else
		exec_from_path_child(argv, *env_list);
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

int	run_command_child(t_cmd *cmd, t_env **env_list, char **envp)
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
			return (ft_exit(cmd->argv));
		return (exec_builtin(cmd->argv, env_list));
	}
	(void)envp;
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

static void	execute_single_cmd(t_cmd *cmd, t_env **env_list, char **envp)
{
	int	saved_stdin;
	int	saved_stdout;
	int	saved_stderr;

	if (check_redir_errors(cmd))
	{
		close_cmd_heredoc_fd(cmd);
		return ;
	}
	if (setup_redirections(cmd, env_list, &saved_stdin, &saved_stdout) == -1)
	{
		close_cmd_heredoc_fd(cmd);
		return ;
	}
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
