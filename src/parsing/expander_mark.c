/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_mark.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 20:55:34 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/09 12:02:47 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/parsing.h"

static void	mark_heredoc_group(t_token *start)
{
	t_token	*scan;

	scan = start;
	while (scan && scan->type == T_WORD)
	{
		scan->no_expand = 1;
		if (!scan->next || scan->next->space_before)
			break ;
		scan = scan->next;
	}
}

void	mark_heredoc_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*scan;

	current = tokens;
	while (current)
	{
		if (current->type == T_HEREDOC && current->next)
		{
			scan = current->next;
			if (scan && scan->type == T_WORD)
				mark_heredoc_group(current->next);
		}
		current = current->next;
	}
}
