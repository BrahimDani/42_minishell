/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens_words.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 04:50:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/16 12:04:35 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	append_word_value(char **result, t_token *current)
{
	char	*tmp;

	if (!current->value)
		return (0);
	tmp = *result;
	*result = ft_strjoin(tmp, current->value);
	free(tmp);
	if (!*result)
		return (0);
	return (1);
}

char	*join_adjacent_words(t_token **token)
{
	char	*result;
	t_token	*current;

	if (!token || !*token || !(*token)->value)
		return (NULL);
	result = ft_strdup((*token)->value);
	if (!result)
		return (NULL);
	current = (*token)->next;
	while (current && current->type == T_WORD && !current->space_before)
	{
		if (!append_word_value(&result, current))
		{
			free(result);
			return (NULL);
		}
		*token = current;
		current = current->next;
	}
	return (result);
}

int	handle_word_token(t_token **tokens, t_cmd *current)
{
	char	*joined_value;

	joined_value = join_adjacent_words(tokens);
	if (!joined_value)
		return (0);
	if (!add_argument(current, joined_value))
	{
		free(joined_value);
		return (0);
	}
	free(joined_value);
	return (1);
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
