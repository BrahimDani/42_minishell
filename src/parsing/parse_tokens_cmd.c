/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 04:50:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/16 06:46:22 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_cmd_fields(t_cmd *cmd)
{
	cmd->argv = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->heredoc_fd = -1;
	cmd->heredoc_quoted = 0;
	cmd->next = NULL;
	cmd->has_in_redir_error = 0;
	cmd->in_redir_first_error = NULL;
	cmd->has_out_redir_error = 0;
	cmd->out_redir_first_error = NULL;
}

void	add_cmd_to_list(t_cmd **head, t_cmd *new)
{
	t_cmd	*tmp;

	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

t_cmd	*create_new_cmd(t_cmd **head)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	init_cmd_fields(new);
	add_cmd_to_list(head, new);
	return (new);
}

static void	copy_argv_ptrs(char **new_argv, char **old_argv, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		new_argv[i] = old_argv[i];
		i++;
	}
}

int	add_argument(t_cmd *cmd, char *value)
{
	int		len;
	char	**new_argv;
	char	*new_value;

	if (!value)
		return (0);
	len = 0;
	while (cmd->argv && cmd->argv[len])
		len++;
	new_argv = malloc(sizeof(char *) * (len + 2));
	if (!new_argv)
		return (0);
	copy_argv_ptrs(new_argv, cmd->argv, len);
	new_value = ft_strdup(value);
	if (!new_value)
	{
		free(new_argv);
		return (0);
	}
	new_argv[len] = new_value;
	new_argv[len + 1] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
	return (1);
}
