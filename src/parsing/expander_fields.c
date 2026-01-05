/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_fields.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 20:55:18 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/05 03:14:30 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/parsing.h"

int	count_fields(char **split)
{
	int	count;

	count = 0;
	while (split && split[count])
		count++;
	return (count);
}

void	free_split_words(char **split)
{
	int	idx;

	if (!split)
		return ;
	idx = 0;
	while (split[idx])
	{
		free(split[idx]);
		idx++;
	}
	free(split);
}

t_token	*handle_empty_result(t_token *cur, t_token **head, t_token *prev)
{
	t_token	*next;
	int		space_before;

	next = cur->next;
	space_before = cur->space_before;
	if (prev)
		prev->next = next;
	else
		*head = next;
	free(cur->value);
	free(cur);
	if (next && space_before && next->space_before == 0)
		next->space_before = 1;
	return (next);
}

t_token	*handle_spaces_only(t_token *cur, t_token **head, t_token *prev)
{
	t_token	*next;

	next = cur->next;
	if (prev)
		prev->next = next;
	else
		*head = next;
	if (prev && next && next->space_before == 0)
		next->space_before = 1;
	free(cur->value);
	free(cur);
	return (next);
}

t_token	*append_field_token(t_token *last, char *word)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(word);
	if (!new->value)
	{
		free(new);
		return (NULL);
	}
	new->type = T_WORD;
	new->no_expand = 0;
	new->was_quoted = 0;
	new->space_before = 1;
	new->next = last->next;
	last->next = new;
	return (new);
}
