/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboxuser <vboxuser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 12:16:44 by vboxuser          #+#    #+#             */
/*   Updated: 2025/10/23 22:17:52 by vboxuser         ###   ########.fr       */
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

static t_token	*new_token(char *value, t_token_type type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(value);
	new->type = type;
	new->next = NULL;
	return (new);
}

static void	add_token_back(t_token **lst, t_token *new)
{
	t_token	*tmp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

static	t_token_type get_type(char *word)
{
	if (!ft_strncmp(word, "|", 2))
		return (T_PIPE);
	if (!ft_strncmp(word, "<", 2))
		return (T_REDIR_IN);
	if (!ft_strncmp(word, ">", 2))
		return (T_REDIR_OUT);
	if (!ft_strncmp(word, ">>", 3))
		return (T_APPEND);
	if (!ft_strncmp(word, "<<", 3))
		return (T_HEREDOC);
	return (T_WORD);
}

t_token	*tokenize_line(char *line)
{
	t_token	*token = NULL;
	char	*token_value;
	int		i;
	int		start;

	i = 0;
	while (line[i])
	{
		while (line[i] == ' ')
			i++;
		if (!line[i])
			break;
		start = i;
		if ((line[i] == '>' || line[i] == '<') && line[i + 1] == line[i])
			i += 2;
		else if (ft_strchr("|<>", line[i]))
			i++;
		else
			while (line[i] && line[i] != ' ' && !ft_strchr("|<>", line[i]))
				i++;
		token_value = ft_substr(line, start, i - start);
		if (!token_value)
			return (NULL);
		add_token_back(&token, new_token(token_value, get_type(token_value)));
		free(token_value);
	}
	return (token);
}
