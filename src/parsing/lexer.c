/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 12:16:44 by vboxuser          #+#    #+#             */
/*   Updated: 2026/02/16 05:55:47 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		if (tokens->value)
			free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

static t_token	*tokenize_fail(t_token *token)
{
	free_tokens(token);
	return (NULL);
}

static int	handle_token(char *line, int *i, t_token **token, int had_space)
{
	int	status;

	status = handle_double_redir(line, i, token, had_space);
	if (status != 0)
		return (status);
	status = handle_simple_operator(line, i, token, had_space);
	if (status != 0)
		return (status);
	status = handle_quotes(line, i, token, had_space);
	if (status != 0)
		return (status);
	return (handle_normal_word(line, i, token, had_space));
}

t_token	*tokenize_line(char *line)
{
	t_token	*token;
	int		i;
	int		had_space;
	int		status;

	token = NULL;
	i = 0;
	while (line[i])
	{
		had_space = skip_spaces(line, &i);
		if (!line[i])
			break ;
		if (line[i] == '$' && (line[i + 1] == '"' || line[i + 1] == '\''))
			i++;
		status = handle_token(line, &i, &token, had_space);
		if (status == -1)
			return (tokenize_fail(token));
	}
	return (token);
}
