/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 14:10:09 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/16 07:26:05 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	init_pipeline(int n_cmds, int (**pipes)[2], t_pipe_ctx *ctx)
{
	*pipes = init_pipes_array(n_cmds, ctx->sh);
	if (!*pipes && n_cmds > 1)
		return (0);
	ctx->pipes = *pipes;
	ctx->n_cmds = n_cmds;
	return (1);
}

static pid_t	*alloc_pipeline_pids(t_pipe_ctx *ctx)
{
	pid_t	*pids;

	if (ctx->n_cmds <= 0)
		return (NULL);
	pids = malloc(sizeof(pid_t) * ctx->n_cmds);
	if (!pids)
	{
		perror("minishell: malloc");
		ms_status_set(ctx->sh, 1);
		close_parent_heredocs(ctx->head);
		close_pipes(ctx->pipes, ctx->n_cmds - 1);
		free(ctx->pipes);
	}
	return (pids);
}

static int	should_stop_pipeline(t_pipe_ctx *ctx)
{
	if (ctx->sh->signal_record != SIGINT)
		return (0);
	close_pipes(ctx->pipes, ctx->n_cmds - 1);
	return (1);
}

static void	finish_pipeline(t_pipe_ctx *ctx, pid_t *pids, int i)
{
	close_parent_heredocs(ctx->head);
	close_pipes(ctx->pipes, ctx->n_cmds - 1);
	wait_all_children(pids, i, ctx->sh);
	free(pids);
	free(ctx->pipes);
}

void	exec_pipeline_loop(t_cmd *cmd_list, t_pipe_ctx *ctx)
{
	pid_t	*pids;
	int		i;

	pids = alloc_pipeline_pids(ctx);
	if (!pids && ctx->n_cmds > 0)
		return ;
	ctx->pids = pids;
	i = 0;
	while (cmd_list && i < ctx->n_cmds)
	{
		if (should_stop_pipeline(ctx))
			break ;
		if (process_pipeline_cmd(cmd_list, ctx, &pids[i], i) == -1)
			break ;
		close_parent_pipe_ends(ctx->pipes, i, ctx->n_cmds);
		cmd_list = cmd_list->next;
		i++;
	}
	finish_pipeline(ctx, pids, i);
}
