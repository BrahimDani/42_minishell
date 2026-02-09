/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 14:10:09 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/09 14:29:18 by kadrouin         ###   ########.fr       */
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
	if (idx > 0)
		close(pipes[idx - 1][0]);
	if (idx < n_cmds - 1)
		close(pipes[idx][1]);
}
