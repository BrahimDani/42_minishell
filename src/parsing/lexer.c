/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 12:16:44 by vboxuser          #+#    #+#             */
/*   Updated: 2026/02/10 17:38:02 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

t_token	*tokenize_line(char *line)
{
	t_token	*token;
	int		i;
	int		had_space;

	token = NULL;
	i = 0;
	while (line[i])
	{
		had_space = skip_spaces(line, &i);
		if (!line[i])
			break ;
		if (line[i] == '$' && (line[i + 1] == '"' || line[i + 1] == '\''))
			i++;
		if (handle_double_redir(line, &i, &token, had_space))
			continue ;
		if (handle_simple_operator(line, &i, &token, had_space))
			continue ;
		if (handle_quotes(line, &i, &token, had_space))
			continue ;
		handle_normal_word(line, &i, &token, had_space);
	}
	return (token);
}
