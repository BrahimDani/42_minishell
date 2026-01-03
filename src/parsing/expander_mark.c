/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_mark.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 20:55:34 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/03 20:37:53 by kadrouin         ###   ########.fr       */
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
		scan->was_quoted = 1;
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
			while (scan && scan->type == T_WORD)
			{
				if (scan->was_quoted || scan->no_expand)
				{
					mark_heredoc_group(current->next);
					break ;
				}
				if (!scan->next || scan->next->space_before)
					break ;
				scan = scan->next;
			}
		}
		current = current->next;
	}
}
