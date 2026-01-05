/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_split_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 05:45:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/05 05:44:36 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_word_split(t_word_split *ws, char *value)
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
