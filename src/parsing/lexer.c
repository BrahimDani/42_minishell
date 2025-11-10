/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 12:16:44 by vboxuser          #+#    #+#             */
/*   Updated: 2025/11/04 10:31:28 by kadrouin         ###   ########.fr       */
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
	new->no_expand = 0;
	new->next = NULL;
	return (new);
}

/**
 * new_token_no_expand - Crée un token qui ne sera pas expandé
 */
static t_token	*new_token_no_expand(char *value, t_token_type type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(value);
	new->type = type;
	new->no_expand = 1;  // Pas d'expansion (single quotes)
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

/**
 * handle_quoted_word - Extrait le contenu entre quotes et avance l'index
 * @line: La ligne à parser
 * @i: Pointeur vers l'index actuel (sera modifié)
 * @quote_char: Le caractère de quote (' ou ")
 * 
 * Returns: Le contenu sans les quotes, ou NULL en cas d'erreur
 */
static char	*handle_quoted_word(char *line, int *i, char quote_char)
{
	int		start;
	char	*content;

	(*i)++;  // Sauter la quote ouvrante
	start = *i;
	while (line[*i] && line[*i] != quote_char)
		(*i)++;
	content = ft_substr(line, start, *i - start);
	if (line[*i] == quote_char)
		(*i)++;
	return (content);
}

/**
 * handle_word - Extrait un mot normal (sans quotes ni opérateurs)
 */
static char	*handle_word(char *line, int *i)
{
	int		start;
	char	*word;

	start = *i;
	while (line[*i] && line[*i] != ' ' && line[*i] != '\'' 
		&& line[*i] != '"' && !ft_strchr("|<>", line[*i]))
		(*i)++;
	word = ft_substr(line, start, *i - start);
	return (word);
}

t_token	*tokenize_line(char *line)
{
	t_token	*token = NULL;
	char	*token_value;
	int		i;

	i = 0;
	while (line[i])
	{
		while (line[i] == ' ')
			i++;
		if (!line[i])
			break;
		if ((line[i] == '>' || line[i] == '<') && line[i + 1] == line[i])
		{
			token_value = ft_substr(line, i, 2);
			add_token_back(&token, new_token(token_value, get_type(token_value)));
			free(token_value);
			i += 2;
		}
		else if (ft_strchr("|<>", line[i]))
		{
			token_value = ft_substr(line, i, 1);
			add_token_back(&token, new_token(token_value, get_type(token_value)));
			free(token_value);
			i++;
		}
		else if (line[i] == '\'')
		{
			token_value = handle_quoted_word(line, &i, '\'');
			add_token_back(&token, new_token_no_expand(token_value, T_WORD));
			free(token_value);
		}
		else if (line[i] == '"')
		{
			token_value = handle_quoted_word(line, &i, '"');
			add_token_back(&token, new_token(token_value, T_WORD));
			free(token_value);
		}
		else
		{
			token_value = handle_word(line, &i);
			add_token_back(&token, new_token(token_value, T_WORD));
			free(token_value);
		}
	}
	return (token);
}
