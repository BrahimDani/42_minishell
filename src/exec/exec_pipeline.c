/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:11:54 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/10 22:17:51 by kadrouin         ###   ########.fr       */
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

int	handle_child_redirs(t_cmd *cmd, t_env **env_list, t_shell *sh)
{
	int	saved_in;
	int	saved_out;
	int	saved_err;

	if (handle_child_error(cmd) == -1)
		return (-1);
	saved_in = handle_input_redir(cmd, env_list, sh);
	if (saved_in == -2)
		return (-1);
	saved_out = handle_output_redir(cmd, sh);
	if (saved_out == -2)
		return (close_saved_fds(saved_in, -1, -1), -1);
	saved_err = setup_stderr_redir(cmd, sh);
	if (saved_err == -2)
		return (close_saved_fds(saved_in, saved_out, -1), -1);
	close_saved_fds(saved_in, saved_out, saved_err);
	return (0);
}

int	process_pipeline_cmd(t_cmd *cmd, t_pipe_ctx *ctx,
	pid_t *pid_slot, int idx)
{
	*pid_slot = fork_and_check(ctx->pipes, ctx->n_cmds, ctx->sh);
	if (*pid_slot < 0)
		return (-1);
	if (*pid_slot == 0)
	{
		setup_child_pipes(ctx->pipes, idx, ctx->n_cmds);
		free(ctx->pipes);
		exec_pipeline_child_cmd(cmd, ctx->head, ctx->env_list, ctx->sh);
	}
	return (0);
}

void	execute_pipeline(t_cmd *cmd_list,
	t_env **env_list, int n_cmds, t_shell *sh)
{
	int			(*pipes)[2];
	t_pipe_ctx	ctx;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	ctx = (t_pipe_ctx){NULL, env_list, cmd_list, n_cmds, sh};
	if (!init_pipeline(n_cmds, &pipes, &ctx))
	{
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		return ;
	}
	exec_pipeline_loop(cmd_list, &ctx);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
