/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 05:18:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/05 05:35:38 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

t_token	*extract_until_semicolon(t_token **tokens)
{
	t_token	*start;
	t_token	*cur;
	t_token	*prev;

	if (!tokens || !*tokens)
		return (NULL);
	start = *tokens;
	cur = start;
	prev = NULL;
	while (cur && cur->type != T_SEMICOLON)
	{
		prev = cur;
		cur = cur->next;
	}
	if (cur && cur->type == T_SEMICOLON)
	{
		if (prev)
			prev->next = NULL;
		*tokens = cur->next;
		free(cur->value);
		free(cur);
	}
	else
		*tokens = NULL;
	return (start);
}
