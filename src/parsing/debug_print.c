/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboxuser <vboxuser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 22:11:33 by vboxuser          #+#    #+#             */
/*   Updated: 2025/10/23 22:18:31 by vboxuser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

#include "../../includes/parsing.h"

void	free_cmds(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i;

	while (cmd)
	{
		tmp = cmd->next;

		// Libère argv
		if (cmd->argv)
		{
			i = 0;
			while (cmd->argv[i])
			{
				free(cmd->argv[i]);
				i++;
			}
			free(cmd->argv);
		}

		// Libère les fichiers
		if (cmd->infile)
			free(cmd->infile);
		if (cmd->outfile)
			free(cmd->outfile);

		// Libère la structure
		free(cmd);

		cmd = tmp;
	}
}


void	print_cmds(t_cmd *cmd)
{
	int i = 0;

	while (cmd)
	{
		printf("=== Command ===\n");
		if (cmd->argv)
		{
			printf("argv: ");
			while (cmd->argv[i])
				printf("[%s] ", cmd->argv[i++]);
			printf("\n");
		}
		else
			printf("argv: (none)\n");
		printf("infile: %s\n", cmd->infile ? cmd->infile : "(none)");
		printf("outfile: %s\n", cmd->outfile ? cmd->outfile : "(none)");
		printf("append: %d\n", cmd->append);
		printf("heredoc: %d\n", cmd->heredoc);
		printf("next: %p\n", cmd->next);
		printf("----------------\n");
		cmd = cmd->next;
		i = 0;
	}
}
