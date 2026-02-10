/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 04:40:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/10 17:37:45 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_double_redir(char *line, int *i, t_token **token, int space)
{
	char	*token_value;
	t_token	*new_tok;

	if ((line[*i] == '>' || line[*i] == '<') && line[*i + 1] == line[*i])
	{
		token_value = ft_substr(line, *i, 2);
		new_tok = new_token(token_value, get_type(token_value));
		new_tok->space_before = space;
		add_token_back(token, new_tok);
		free(token_value);
		*i += 2;
		return (1);
	}
	return (0);
}

int	handle_simple_operator(char *line, int *i, t_token **token, int space)
{
	char	*token_value;
	t_token	*new_tok;

	if (ft_strchr("|<>", line[*i]))
	{
		token_value = ft_substr(line, *i, 1);
		new_tok = new_token(token_value, get_type(token_value));
		new_tok->space_before = space;
		add_token_back(token, new_tok);
		free(token_value);
		(*i)++;
		return (1);
	}
	return (0);
}

int	handle_quotes(char *line, int *i, t_token **token, int space)
{
	char	*token_value;
	t_token	*new_tok;

	if (line[*i] == '\'')
	{
		token_value = handle_quoted_word(line, i, '\'');
		new_tok = new_token_no_expand(token_value, T_WORD);
		new_tok->space_before = space;
		add_token_back(token, new_tok);
		free(token_value);
		return (1);
	}
	if (line[*i] == '"')
	{
		token_value = handle_quoted_word(line, i, '"');
		new_tok = new_token_quoted(token_value, T_WORD);
		new_tok->space_before = space;
		add_token_back(token, new_tok);
		free(token_value);
		return (1);
	}
	return (0);
}
