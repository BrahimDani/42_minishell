/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens_words.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 04:50:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/16 10:14:13 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*join_adjacent_words(t_token **token)
{
	char	*result;
	char	*tmp;
	t_token	*current;

	if (!token || !*token || !(*token)->value)
		return (NULL);
	result = ft_strdup((*token)->value);
	if (!result)
		return (NULL);
	current = (*token)->next;
	while (current && current->type == T_WORD && !current->space_before)
	{
		if (!current->value)
			return (free(result), NULL);
		tmp = result;
		result = ft_strjoin(tmp, current->value);
		free(tmp);
		if (!result)
			return (NULL);
		*token = current;
		current = current->next;
	}
	return (result);
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
			quoted = 1;
		if (!cur->next || cur->next->space_before)
			break ;
		cur = cur->next;
	}
	return (quoted);
}
