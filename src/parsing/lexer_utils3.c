/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 04:40:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/05 04:40:33 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	add_operator_token(char *token_value, t_token **token, int space)
{
	t_token	*new_tok;

	if (!token_value)
		return (-1);
	new_tok = new_token(token_value, get_type(token_value));
	free(token_value);
	if (!new_tok)
		return (-1);
	new_tok->space_before = space;
	add_token_back(token, new_tok);
	return (1);
}

static int	add_quoted_token(char *token_value, t_token **token, int space,
		int no_expand)
{
	t_token	*new_tok;

	if (!token_value)
		return (-1);
	if (no_expand)
		new_tok = new_token_no_expand(token_value, T_WORD);
	else
		new_tok = new_token_quoted(token_value, T_WORD);
	free(token_value);
	if (!new_tok)
		return (-1);
	new_tok->space_before = space;
	add_token_back(token, new_tok);
	return (1);
}

int	handle_double_redir(char *line, int *i, t_token **token, int space)
{
	char	*token_value;
	int		status;

	if ((line[*i] == '>' || line[*i] == '<') && line[*i + 1] == line[*i])
	{
		token_value = ft_substr(line, *i, 2);
		status = add_operator_token(token_value, token, space);
		if (status == 1)
			*i += 2;
		return (status);
	}
	return (0);
}

int	handle_simple_operator(char *line, int *i, t_token **token, int space)
{
	char	*token_value;
	int		status;

	if (ft_strchr("|<>", line[*i]))
	{
		token_value = ft_substr(line, *i, 1);
		status = add_operator_token(token_value, token, space);
		if (status == 1)
			*i += 1;
		return (status);
	}
	return (0);
}

int	handle_quotes(char *line, int *i, t_token **token, int space)
{
	if (line[*i] == '\'')
		return (add_quoted_token(handle_quoted_word(line, i, '\''),
				token, space, 1));
	if (line[*i] == '"')
		return (add_quoted_token(handle_quoted_word(line, i, '"'),
				token, space, 0));
	return (0);
}
