/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 05:10:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/05 05:47:13 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_token	*handle_single_word_split(t_token *current, t_token *prev,
		t_token *next, t_word_split *ws)
{
	free(current->value);
	current->value = ft_strdup(ws->split_words[0]);
	if (prev && current->space_before == 0 && ws->leading > 0)
		current->space_before = 1;
	if (ws->trailing > 0 && next && next->space_before == 0)
		next->space_before = 1;
	free_split_result(ws);
	return (current);
}

static t_token	*create_new_word_token(char *word)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(word);
	new_token->type = T_WORD;
	new_token->no_expand = 0;
	new_token->was_quoted = 0;
	new_token->space_before = 1;
	return (new_token);
}

static void	add_remaining_tokens(t_token *current, t_word_split *ws)
{
	t_token	*last_new;
	t_token	*new_token;

	last_new = current;
	ws->i = 1;
	while (ws->split_words[ws->i])
	{
		new_token = create_new_word_token(ws->split_words[ws->i]);
		if (!new_token)
		{
			ws->i++;
			continue ;
		}
		new_token->next = last_new->next;
		last_new->next = new_token;
		last_new = new_token;
		ws->i++;
	}
	if (ws->trailing > 0 && last_new->next
		&& last_new->next->space_before == 0)
		last_new->next->space_before = 1;
}

t_token	*handle_word_split(t_token **tokens, t_token *current,
		t_token *prev, t_token *next)
{
	t_word_split	ws;

	init_word_split(&ws, current->value);
	if (ws.field_count == 0)
	{
		if (prev && next && next->space_before == 0)
			next->space_before = 1;
		if (prev)
			prev->next = next;
		else
			*tokens = next;
		free_split_result(&ws);
		free(current->value);
		free(current);
		return (next);
	}
	if (ws.field_count == 1)
		return (handle_single_word_split(current, prev, next, &ws));
	free(current->value);
	current->value = ft_strdup(ws.split_words[0]);
	if (prev && current->space_before == 0 && ws.leading > 0)
		current->space_before = 1;
	add_remaining_tokens(current, &ws);
	free_split_result(&ws);
	return (current);
}
