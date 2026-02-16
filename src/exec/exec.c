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

int	exec_external(char **argv, char **envp, t_env **env_list)
{
	if (ft_strchr(argv[0], '/'))
		return (exec_absolute_path(argv, envp, *env_list));
	else
		return (exec_from_path(argv, envp, *env_list));
}

static int	exec_absolute_path_child(char **argv, t_env *env_list)
{
	if (access(argv[0], F_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (127);
	}
	if (access(argv[0], X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (126);
	}
	return (child_process(argv[0], argv, env_list));
}

static int	exec_from_path_child(char **argv, t_env *env_list)
{
	char	*full_path;
	int		special_status;
	int		status;

	special_status = check_special_dirs(argv[0]);
	if (special_status)
		return (special_status);
	full_path = find_command_path(argv[0], env_list);
	if (!full_path)
	{
		print_cmd_error(argv[0], "command not found");
		return (127);
	}
	if (access(full_path, X_OK) != 0)
	{
		print_cmd_error(argv[0], "Permission denied");
		free(full_path);
		return (126);
	}
	status = child_process(full_path, argv, env_list);
	free(full_path);
	return (status);
}

int	exec_external_child(char **argv, t_env **env_list)
{
	if (ft_strchr(argv[0], '/'))
		return (exec_absolute_path_child(argv, *env_list));
	return (exec_from_path_child(argv, *env_list));
}

/**
 * run_command - dispatch a parsed command (builtin or external)
 * Uses the same behavior as the previous main.c implementation.
 */
int	run_command(t_cmd *cmd, t_env **env_list, t_shell *sh)
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
		return (exec_external(cmd->argv, sh->envp, env_list));
	}
	if (is_builtin(cmd->argv[0]))
	{
		if (ft_strcmp(cmd->argv[0], "exit") == 0)
			return (ft_exit(cmd->argv, *env_list, cmd, sh));
		return (exec_builtin(cmd->argv, env_list, sh->last_status));
	}
	return (exec_external(cmd->argv, sh->envp, env_list));
}
