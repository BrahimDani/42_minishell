/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:11:54 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/09 14:10:40 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	setup_child_pipes(int pipes[][2], int i, int n_cmds)
{
	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < n_cmds - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
	close_pipes(pipes, n_cmds - 1);
}

static int	handle_child_error(t_cmd *cmd)
{
	if (cmd->has_in_redir_error && cmd->in_redir_first_error)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->in_redir_first_error);
		return (-1);
	}
	if (cmd->has_out_redir_error && cmd->out_redir_first_error)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->out_redir_first_error);
		return (-1);
	}
	return (0);
}

int	handle_child_redirs(t_cmd *cmd, t_env **env_list)
{
	int	saved_in;
	int	saved_out;
	int	saved_err;

	if (handle_child_error(cmd) == -1)
		return (-1);
	saved_in = handle_input_redir(cmd, env_list);
	if (saved_in == -2)
		return (-1);
	saved_out = handle_output_redir(cmd);
	if (saved_out == -2)
	{
		if (saved_in >= 0)
			close(saved_in);
		return (-1);
	}
	saved_err = setup_stderr_redir(cmd);
	if (saved_in >= 0)
		close(saved_in);
	if (saved_out >= 0)
		close(saved_out);
	if (saved_err >= 0)
		close(saved_err);
	return (0);
}

static int	process_pipeline_cmd(t_cmd *cmd, t_pipe_ctx *ctx,
		pid_t *pid_slot, int idx)
{
	*pid_slot = fork_and_check(ctx->pipes, ctx->n_cmds);
	if (*pid_slot < 0)
		return (-1);
	if (*pid_slot == 0)
	{
		setup_child_pipes(ctx->pipes, idx, ctx->n_cmds);
		free(ctx->pipes);
		exec_pipeline_child_cmd(cmd, ctx->head, ctx->env_list, ctx->envp);
	}
	return (0);
}

void	execute_pipeline(t_cmd *cmd_list,
	t_env **env_list, char **envp, int n_cmds)
{
	int			(*pipes)[2];
	pid_t		pids[1024];
	t_pipe_ctx	ctx;
	int			i;
	t_cmd		*head;

	pipes = init_pipes_array(n_cmds);
	if (!pipes && n_cmds > 1)
		return ;
	head = cmd_list;
	ctx = (t_pipe_ctx){pipes, env_list, envp, head, n_cmds};
	i = 0;
	while (cmd_list && i < n_cmds)
	{
		if (process_pipeline_cmd(cmd_list, &ctx, &pids[i], i) == -1)
			break ;
		close_parent_pipe_ends(pipes, i, n_cmds);
		cmd_list = cmd_list->next;
		i++;
	}
	close_parent_heredocs(head);
	wait_all_children(pids, i);
	free(pipes);
}
