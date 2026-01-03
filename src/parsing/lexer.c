/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 12:16:44 by vboxuser          #+#    #+#             */
/*   Updated: 2026/01/03 18:52:28 by kadrouin         ###   ########.fr       */
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
	new->was_quoted = 0;
	new->space_before = 0;
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
	new->was_quoted = 1;
	new->space_before = 0;
	new->next = NULL;
	return (new);
}

static t_token *new_token_quoted(char *value, t_token_type type)
{
	t_token *new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(value);
	new->type = type;
	new->no_expand = 0; // double quotes allow expansion
	new->was_quoted = 1;
	new->space_before = 0;
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
	if (!ft_strncmp(word, ";", 2))
		return (T_SEMICOLON);
	if (!ft_strncmp(word, "<", 2))
		return (T_REDIR_IN);
	if (!ft_strncmp(word, ">|", 3))
		return (T_REDIR_OUT);
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
	while (line[*i] && line[*i] != ' ' && line[*i] != '\t' && line[*i] != '\'' 
		&& line[*i] != '"' && !ft_strchr("|<>;", line[*i]))
		(*i)++;
	word = ft_substr(line, start, *i - start);
	return (word);
}

static int	skip_spaces(char *line, int *i)
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

static int	handle_double_redir(char *line, int *i, t_token **token, int space)
{
	char	*token_value;
	t_token	*new_tok;

	if (line[*i] == '>' && line[*i + 1] == '|')
	{
		token_value = ft_substr(line, *i, 2);
		new_tok = new_token(token_value, get_type(token_value));
		new_tok->space_before = space;
		add_token_back(token, new_tok);
		free(token_value);
		*i += 2;
		return (1);
	}
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
static int	handle_fd_redir(char *line, int *i, t_token **token, int space)
{
	char	*token_value;
	t_token	*new_tok;

	if ((line[*i] == '2' || line[*i] == '1' || line[*i] == '0')
		&& (line[*i + 1] == '>' || line[*i + 1] == '<'))
	{
		if (line[*i + 1] == '>' && line[*i + 2] == '>')
		{
			token_value = ft_substr(line, *i, 3);
			new_tok = new_token(token_value, T_APPEND);
			new_tok->space_before = space;
			add_token_back(token, new_tok);
			free(token_value);
			*i += 3;
			return (1);
		}
		token_value = ft_substr(line, *i, 2);
		if (line[*i + 1] == '>')
			new_tok = new_token(token_value, T_REDIR_OUT);
		else
			new_tok = new_token(token_value, T_REDIR_IN);
		new_tok->space_before = space;
		add_token_back(token, new_tok);
		free(token_value);
		*i += 2;
		return (1);
	}
	return (0);
}
static int	handle_simple_operator(char *line, int *i, t_token **token, int space)
{
	char	*token_value;
	t_token	*new_tok;

	if (ft_strchr("|<>;", line[*i]))
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

static int	handle_quotes(char *line, int *i, t_token **token, int space)
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

static void	handle_normal_word(char *line, int *i, t_token **token, int space)
{
	char	*token_value;
	t_token	*new_tok;

	token_value = handle_word(line, i);
	new_tok = new_token(token_value, T_WORD);
	new_tok->space_before = space;
	add_token_back(token, new_tok);
	free(token_value);
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
		if (handle_fd_redir(line, &i, &token, had_space))
			continue ;
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
