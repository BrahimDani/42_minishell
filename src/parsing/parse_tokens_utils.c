/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 04:50:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/09 12:03:12 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_cmd_fields(t_cmd *cmd)
{
	cmd->argv = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->errfile = NULL;
	cmd->append = 0;
	cmd->err_append = 0;
	cmd->heredoc = 0;
	cmd->heredoc_fd = -1;
	cmd->heredoc_quoted = 0;
	cmd->next = NULL;
	cmd->redirect_stderr_to_out = 0;
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

void	add_argument(t_cmd *cmd, char *value)
{
	int		i;
	char	**new_argv;

	i = 0;
	if (cmd->argv)
	{
		while (cmd->argv[i])
			i++;
	}
	new_argv = malloc(sizeof(char *) * (i + 2));
	if (!new_argv)
		return ;
	i = 0;
	if (cmd->argv)
	{
		while (cmd->argv[i])
		{
			new_argv[i] = cmd->argv[i];
			i++;
		}
		free(cmd->argv);
	}
	new_argv[i++] = ft_strdup(value);
	new_argv[i] = NULL;
	cmd->argv = new_argv;
}

int	aggregate_quoted(t_token *start)
{
	int		quoted;
	t_token	*cur;

	quoted = 0;
	cur = start;
	if (!cur)
		return (0);
	while (cur && cur->type == T_WORD)
	{
		if (cur->was_quoted)
		{
			quoted = 1;
		}
		if (!cur->next || cur->next->space_before)
			break ;
		cur = cur->next;
	}
	return (quoted);
}
