/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:11:54 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/03 20:16:16 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	(*alloc_pipes(int n_cmds))[2]
{
	int	(*pipes)[2];

	if (n_cmds < 2)
		return (NULL);
	pipes = malloc(sizeof(int [2]) * (n_cmds - 1));
	return (pipes);
}

int	(*init_pipes_array(int n_cmds))[2]
{
	int	(*pipes)[2];

	pipes = alloc_pipes(n_cmds);
	if (!pipes && n_cmds > 1)
		return (NULL);
	if (n_cmds - 1 > 0 && create_pipes(pipes, n_cmds) == -1)
	{
		free(pipes);
		return (NULL);
	}
	return (pipes);
}

void	exec_pipeline_child_cmd(t_cmd *cmd, t_env **env_list, char **envp)
{
	int	status;

	if (handle_child_redirs(cmd, env_list) == -1)
		exit(1);
	if (cmd->argv && cmd->argv[0])
		status = run_command(cmd, env_list, envp);
	else
		status = 0;
	exit(status);
}

void	wait_all_children(pid_t *pids, int n_cmds)
{
	int	i;
	int	status;

	i = 0;
	while (i < n_cmds)
	{
		waitpid(pids[i], &status, 0);
		if (i == n_cmds - 1 && WIFEXITED(status))
			g_last_status = WEXITSTATUS(status);
		i++;
	}
}

int	fork_and_check(int pipes[][2], int n_cmds)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("minishell: fork failed\n", 2);
		g_last_status = 1;
		close_pipes(pipes, n_cmds - 1);
	}
	return (pid);
}
