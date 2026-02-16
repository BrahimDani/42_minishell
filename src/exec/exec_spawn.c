/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_spawn.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:11:54 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/10 23:06:04 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <errno.h>
#include <string.h>

int	exec_error_exit(char *full_path, char **new_envp)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(full_path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	free_envp_array(new_envp);
	return (126);
}

int	child_process(char *full_path, char **argv, t_env *env_list)
{
	char	**new_envp;

	setup_child_exec_signals();
	set_env_value(&env_list, "_", full_path);
	new_envp = build_envp_from_list(env_list);
	if (!new_envp)
		return (1);
	close_extra_fds();
	execve(full_path, argv, new_envp);
	return (exec_error_exit(full_path, new_envp));
}

static void	restore_parent_signals(void)
{
	setup_prompt_signals();
}

static int	handle_signaled_status(int status)
{
	int	sig;

	if (!WIFSIGNALED(status))
		return (-1);
	sig = WTERMSIG(status);
	if (sig == SIGINT)
		write(STDOUT_FILENO, "\n", 1);
	else if (sig == SIGQUIT)
		print_sigquit_message();
	return (128 + sig);
}

int	spawn_external(char *full_path, char **argv, char **envp, t_env *env_list)
{
	pid_t	pid;
	int		status;
	int		exit_status;

	(void)envp;
	setup_parent_exec_signals();
	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("minishell: fork failed\n", 2);
		restore_parent_signals();
		return (-1);
	}
	if (pid == 0)
		ms_exit(child_process(full_path, argv, env_list), env_list);
	waitpid(pid, &status, 0);
	restore_parent_signals();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	exit_status = handle_signaled_status(status);
	return (exit_status);
}
