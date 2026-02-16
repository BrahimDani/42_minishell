/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:11:54 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/16 05:19:15 by kadrouin         ###   ########.fr       */
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
	if (n_cmds - 1 > 0 && (create_pipes(pipes, n_cmds, sh) == -1))
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

	setup_child_exec_signals();
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

static int	handle_last_child_status(int status, t_shell *sh)
{
	int	sig;

	if (WIFEXITED(status))
	{
		ms_status_set(sh, WEXITSTATUS(status));
		return (0);
	}
	if (!WIFSIGNALED(status))
		return (0);
	sig = WTERMSIG(status);
	if (sig == SIGINT)
		write(STDOUT_FILENO, "\n", 1);
	else if (sig == SIGQUIT)
		print_sigquit_message();
	ms_status_set(sh, 128 + sig);
	if (sig == SIGINT)
		return (1);
	return (0);
}

void	wait_all_children(pid_t *pids, int n_cmds, t_shell *sh)
{
	int	i;
	int	status;
	int	printed_sigint;

	i = 0;
	printed_sigint = 0;
	while (i < n_cmds)
	{
		waitpid(pids[i], &status, 0);
		if (i == n_cmds - 1)
			printed_sigint = handle_last_child_status(status, sh);
		i++;
	}
	if (sh->signal_record == SIGINT && !printed_sigint)
		write(STDOUT_FILENO, "\n", 1);
	if (sh->signal_record == SIGINT)
		ms_status_set(sh, 128 + SIGINT);
}
