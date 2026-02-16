/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_fork.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 05:17:38 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/16 05:17:38 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	close_parent_heredocs(t_cmd *cmd_list)
{
	while (cmd_list)
	{
		if (cmd_list->heredoc_fd >= 0)
		{
			close(cmd_list->heredoc_fd);
			cmd_list->heredoc_fd = -1;
		}
		cmd_list = cmd_list->next;
	}
}

void	close_parent_pipe_ends(int pipes[][2], int idx, int n_cmds)
{
	if (!pipes)
		return ;
	if (idx > 0 && pipes[idx - 1][0] >= 0)
	{
		close(pipes[idx - 1][0]);
		pipes[idx - 1][0] = -1;
	}
	if (idx < n_cmds - 1 && pipes[idx][1] >= 0)
	{
		close(pipes[idx][1]);
		pipes[idx][1] = -1;
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
