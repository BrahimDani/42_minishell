/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 14:10:09 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/10 22:18:40 by kadrouin         ###   ########.fr       */
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

int	init_pipeline(int n_cmds, int (**pipes)[2], t_pipe_ctx *ctx)
{
	*pipes = init_pipes_array(n_cmds, ctx->sh);
	if (!*pipes && n_cmds > 1)
		return (0);
	ctx->pipes = *pipes;
	ctx->n_cmds = n_cmds;
	return (1);
}

void	exec_pipeline_loop(t_cmd *cmd_list, t_pipe_ctx *ctx)
{
	pid_t	pids[1024];
	int		i;

	i = 0;
	while (cmd_list && i < ctx->n_cmds)
	{
		if (process_pipeline_cmd(cmd_list, ctx, &pids[i], i) == -1)
			break ;
		close_parent_pipe_ends(ctx->pipes, i, ctx->n_cmds);
		cmd_list = cmd_list->next;
		i++;
	}
	close_parent_heredocs(ctx->head);
	wait_all_children(pids, i, ctx->sh);
	free(ctx->pipes);
}
