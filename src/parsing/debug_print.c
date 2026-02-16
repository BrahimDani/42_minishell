/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 22:11:33 by vboxuser          #+#    #+#             */
/*   Updated: 2026/02/10 20:02:46 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/parsing.h"

static void	free_cmd_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv && argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

static void	free_cmd_files(t_cmd *cmd)
{
	free(cmd->infile);
	free(cmd->outfile);
	if (cmd->heredoc_fd >= 0)
		close(cmd->heredoc_fd);
}

static void	free_cmd_errors(t_cmd *cmd)
{
	free(cmd->in_redir_first_error);
	free(cmd->out_redir_first_error);
}

void	free_cmds(t_cmd *cmd)
{
	t_cmd	*next;

	while (cmd)
	{
		next = cmd->next;
		free_cmd_argv(cmd->argv);
		free_cmd_files(cmd);
		free_cmd_errors(cmd);
		free(cmd);
		cmd = next;
	}
}
