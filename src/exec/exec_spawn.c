/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_spawn.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:11:54 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/03 20:04:10 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <errno.h>
#include <string.h>

void	exec_error_exit(char *full_path, char **new_envp)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(full_path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	free_envp_array(new_envp);
	exit(126);
}

void	child_process(char *full_path, char **argv, t_env *env_list)
{
	char	**new_envp;

	set_env_value(&env_list, "_", full_path);
	new_envp = build_envp_from_list(env_list);
	if (!new_envp)
		exit(1);
	execve(full_path, argv, new_envp);
	exec_error_exit(full_path, new_envp);
}

int	spawn_external(char *full_path, char **argv, char **envp, t_env *env_list)
{
	pid_t	pid;
	int		status;

	(void)envp;
	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("minishell: fork failed\n", 2);
		return (-1);
	}
	if (pid == 0)
		child_process(full_path, argv, env_list);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (-1);
}
