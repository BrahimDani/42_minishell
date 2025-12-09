/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboxuser <vboxuser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 18:53:53 by brdany            #+#    #+#             */
/*   Updated: 2025/12/09 04:06:27 by vboxuser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/parsing.h"

//Check if closed quote and if their is ("|\) in the beginning
int  check_quote(char *line)
{
	int i;
	int quote;

	i = 0;
	while(line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			quote = line[i];
			i++;
			while (line[i] && line[i] != quote)
				i++;
			if (!line[i])
			{
				ft_putendl_fd("minishell: syntax error near quote", STDERR_FILENO);
				return (0);
			}
		}
		i++;
	}
	return (1);
}

int	check_redir(char *line)
{
	(void)line;
	/* Token-level validation handles redirection syntax; keep this neutral. */
	return (1);
}

int	check_ampersand(char *line)
{
	int i = 0;
	int in_squote = 0;
	int in_dquote = 0;

	while (line[i])
	{
		if (!in_dquote && line[i] == '\'')
			in_squote = !in_squote;
		else if (!in_squote && line[i] == '"')
			in_dquote = !in_dquote;
		else if (!in_squote && !in_dquote && line[i] == '&')
		{
			if (line[i + 1] == '&')
				ft_putendl_fd("minishell: syntax error near unexpected token `&&'", STDERR_FILENO);
			else
				ft_putendl_fd("minishell: syntax error near unexpected token `&'", STDERR_FILENO);
			g_last_status = 2;
			return (0);
		}
		i++;
	}
	return (1);
}

//check if line is empty
int	empty_line(char *line)
{
	int	i;

	i = 0;
	while(line[i])
	{
		if (line[i] != ' ' 
			&& line[i] != '\t'
			&& line[i] != '\n'
			&& line[i] != '\v'
			&& line[i] != '\f'
			&& line[i] != '\r')
			return (0);
		i++;
	}
	return (1);
}

static int	check_first_pipe(t_token *tokens)
{
	if (tokens->type == T_PIPE)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'",
			STDERR_FILENO);
		g_last_status = 2;
		return (0);
	}
	return (1);
}

static int	check_last_pipe(t_token *tokens)
{
	t_token	*current;
	t_token	*last;

	current = tokens;
	last = tokens;
	while (current)
	{
		last = current;
		current = current->next;
	}
	if (last->type == T_PIPE)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'",
			STDERR_FILENO);
		g_last_status = 2;
		return (0);
	}
	return (1);
}

static int	check_consecutive_pipes(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == T_PIPE && current->next
			&& current->next->type == T_PIPE)
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `|'",
				STDERR_FILENO);
			g_last_status = 2;
			return (0);
		}
		current = current->next;
	}
	return (1);
}

int	check_pipe_tokens(t_token *tokens)
{
	if (!tokens)
		return (1);
	if (!check_first_pipe(tokens))
		return (0);
	if (!check_last_pipe(tokens))
		return (0);
	if (!check_consecutive_pipes(tokens))
		return (0);
	return (1);
}

static void print_unexpected_token(const char *tok)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
	ft_putstr_fd((char *)tok, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
}

static int	is_redir_token(t_token_type type)
{
	return (type == T_REDIR_IN || type == T_REDIR_OUT
		|| type == T_APPEND || type == T_HEREDOC);
}

static int	validate_redir_token(t_token *t)
{
	if (!t->next)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `newline'",
			STDERR_FILENO);
		g_last_status = 2;
		return (0);
	}
	if (t->next->type != T_WORD)
	{
		if (t->next->value)
			print_unexpected_token(t->next->value);
		else
			ft_putendl_fd("minishell: syntax error near unexpected token `newline'",
				STDERR_FILENO);
		g_last_status = 2;
		return (0);
	}
	return (1);
}

int	validate_tokens_syntax(t_token *tokens)
{
	t_token	*t;

	t = tokens;
	while (t)
	{
		if (is_redir_token(t->type))
		{
			if (!validate_redir_token(t))
				return (0);
		}
		t = t->next;
	}
	return (1);
}
