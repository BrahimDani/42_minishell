/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 04:40:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/05 04:39:15 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*handle_quoted_word(char *line, int *i, char quote_char)
{
	int		start;
	char	*content;

	(*i)++;
	start = *i;
	while (line[*i] && line[*i] != quote_char)
		(*i)++;
	content = ft_substr(line, start, *i - start);
	if (line[*i] == quote_char)
		(*i)++;
	return (content);
}

char	*handle_word(char *line, int *i)
{
	int		start;
	char	*word;

	start = *i;
	while (line[*i] && line[*i] != ' ' && line[*i] != '\t' && line[*i] != '\''
		&& line[*i] != '"' && !ft_strchr("|<>", line[*i]))
		(*i)++;
	word = ft_substr(line, start, *i - start);
	return (word);
}

int	skip_spaces(char *line, int *i)
{
	int	had_space;

	had_space = 0;
	while (line[*i] == ' ' || line[*i] == '\t')
	{
		had_space = 1;
		(*i)++;
	}
	return (had_space);
}

void	handle_normal_word(char *line, int *i, t_token **token, int space)
{
	char	*token_value;
	t_token	*new_tok;

	token_value = handle_word(line, i);
	new_tok = new_token(token_value, T_WORD);
	new_tok->space_before = space;
	add_token_back(token, new_tok);
	free(token_value);
}
