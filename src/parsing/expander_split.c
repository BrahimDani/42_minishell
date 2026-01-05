/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 05:10:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/05 05:35:38 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	init_word_split(t_word_split *ws, char *value)
{
	ws->leading = 0;
	while (value[ws->leading] && is_ifs_char(value[ws->leading]))
		ws->leading++;
	ws->len = ft_strlen(value);
	ws->trailing = 0;
	while (ws->trailing < ws->len
		&& is_ifs_char(value[ws->len - 1 - ws->trailing]))
		ws->trailing++;
	ws->split_words = split_on_ifs(value);
	ws->field_count = 0;
	if (ws->split_words)
	{
		while (ws->split_words[ws->field_count])
			ws->field_count++;
	}
	ws->i = 0;
}

void	free_split_result(t_word_split *ws)
{
	if (ws->split_words)
	{
		ws->i = 0;
		while (ws->split_words[ws->i])
			free(ws->split_words[ws->i++]);
		free(ws->split_words);
	}
}

char	**split_on_ifs(char *str)
{
	int		i;
	char	**result;
	int		start;
	int		j;

	result = malloc(sizeof(char *) * (count_ifs_words(str) + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		while (str[i] && is_ifs_char(str[i]))
			i++;
		if (!str[i])
			break ;
		start = i;
		while (str[i] && !is_ifs_char(str[i]))
			i++;
		result[j++] = ft_substr(str, start, i - start);
	}
	result[j] = NULL;
	return (result);
}

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

t_token	*handle_word_split(t_token **tokens, t_token *current,
		t_token *prev, t_token *next)
{
	t_word_split	ws;
	t_token			*last_new;
	t_token			*new_token;

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
	last_new = current;
	ws.i = 1;
	while (ws.split_words[ws.i])
	{
		new_token = malloc(sizeof(t_token));
		if (!new_token)
		{
			ws.i++;
			continue ;
		}
		new_token->value = ft_strdup(ws.split_words[ws.i]);
		new_token->type = T_WORD;
		new_token->no_expand = 0;
		new_token->was_quoted = 0;
		new_token->space_before = 1;
		new_token->next = last_new->next;
		last_new->next = new_token;
		last_new = new_token;
		ws.i++;
	}
	if (ws.trailing > 0 && last_new->next
		&& last_new->next->space_before == 0)
		last_new->next->space_before = 1;
	free_split_result(&ws);
	return (current);
}
