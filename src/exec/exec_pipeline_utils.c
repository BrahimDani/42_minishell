/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:11:54 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/10 22:58:34 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	(*alloc_pipes(int n_cmds))[2]
{
	int	(*pipes)[2];
	int	i;

	if (n_cmds < 2)
		return (NULL);
	pipes = malloc(sizeof(int [2]) * (n_cmds - 1));
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < n_cmds - 1)
	{
		pipes[i][0] = -1;
		pipes[i][1] = -1;
		i++;
	}
	return (pipes);
}

int	(*init_pipes_array(int n_cmds, t_shell *sh))[2]
{
	int	(*pipes)[2];

	pipes = alloc_pipes(n_cmds);
	if (!pipes && n_cmds > 1)
		return (NULL);
	if (n_cmds - 1 > 0 && create_pipes(pipes, n_cmds, sh) == -1)
	{
		free(pipes);
		return (NULL);
	}
	return (pipes);
}

void	exec_pipeline_child_cmd(t_cmd *cmd, t_cmd *head,
	t_env **env_list, t_shell *sh)
{
	int		status;
	t_cmd	*cur;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	cur = head;
	while (cur)
	{
		if (cur != cmd && cur->heredoc_fd >= 0)
		{
			close(cur->heredoc_fd);
			cur->heredoc_fd = -1;
		}
		cur = cur->next;
	}
	if (handle_child_redirs(cmd, env_list, sh) == -1)
		status = 1;
	else if (cmd->argv && cmd->argv[0])
		status = run_command_child(cmd, env_list);
	else
		status = 0;
	free_cmds(head);
	ms_exit(status, *env_list);
}

void	wait_all_children(pid_t *pids, int n_cmds, t_shell *sh)
{
	int	i;
	int	status;
	int	sig;

	i = 0;
	while (i < n_cmds)
	{
		waitpid(pids[i], &status, 0);
		if (i == n_cmds - 1 && WIFEXITED(status))
			ms_status_set(sh, WEXITSTATUS(status));
		else if (i == n_cmds - 1 && WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			if (sig == SIGINT)
				write(STDOUT_FILENO, "\n", 1);
			else if (sig == SIGQUIT)
				ft_putstr_fd("Quit (core dumped)\n", 2);
			ms_status_set(sh, 128 + sig);
		}
		i++;
	}
}

int	fork_and_check(int pipes[][2], int n_cmds, t_shell *sh)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("minishell: fork failed\n", 2);
		ms_status_set(sh, 1);
		close_pipes(pipes, n_cmds - 1);
	}
	return (pid);
}
