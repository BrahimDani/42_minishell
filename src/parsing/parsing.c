/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboxuser <vboxuser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:03:26 by kadrouin          #+#    #+#             */
/*   Updated: 2025/11/22 14:31:25 by vboxuser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

char	**tokens_to_array(t_token *list)
{
	int		count;
	t_token	*tmp = list;
	char	**arr;

	count = 0;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	count = 0;
	while (list)
	{
		arr[count++] = ft_strdup(list->value);
		list = list->next;
	}
	arr[count] = NULL;
	return (arr);
}

t_token	*parse_line(char *line)
{
	t_token *tokens;

	if (!line || !*line)
		return (NULL);
	if (empty_line(line))
		return (NULL);
	if (!check_quote(line) || !check_redir(line) || !check_ampersand (line))
	{
		g_last_status = 2;
		return (NULL);
	}
	tokens = tokenize_line(line);
	if (!tokens)
		return (NULL);
	if (!check_pipe_tokens(tokens))
	{
		free_tokens(tokens);
		return (NULL);
	}
	return (tokens);
}

