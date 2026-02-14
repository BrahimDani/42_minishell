/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 00:00:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/10 18:47:41 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	close_pipes(int pipes[][2], int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (pipes[i][0] >= 0)
		{
			close(pipes[i][0]);
			pipes[i][0] = -1;
		}
		if (pipes[i][1] >= 0)
		{
			close(pipes[i][1]);
			pipes[i][1] = -1;
		}
		i++;
	}
}

int	create_pipes(int pipes[][2], int n_cmds, t_shell *sh)
{
	int	i;

	i = 0;
	while (i < n_cmds - 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("minishell: pipe");
			close_pipes(pipes, i);
			ms_status_set(sh, 1);
			return (-1);
		}
		i++;
	}
	return (0);
}
