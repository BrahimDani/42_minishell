/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 18:53:53 by brdany            #+#    #+#             */
/*   Updated: 2025/12/02 03:20:07 by kadrouin         ###   ########.fr       */
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
int	check_pipe_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*last;

	if (!tokens)
		return (1);
	if (tokens->type == T_PIPE)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", STDERR_FILENO);
		g_last_status = 2;
		return (0);
	}
	current = tokens;
	last = tokens;
	while (current)
	{
		last = current;
		current = current->next;
	}
	if (last->type == T_PIPE)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", STDERR_FILENO);
		g_last_status = 2;
		return (0);
	}
	current = tokens;
	while (current)
	{
		if (current->type == T_PIPE && current->next && current->next->type == T_PIPE)
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `|'", STDERR_FILENO);
			g_last_status = 2;
			return (0);
		}
		current = current->next;
	}
	return (1);
}

static void print_unexpected_token(const char *tok)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
	ft_putstr_fd((char *)tok, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
}

int validate_tokens_syntax(t_token *tokens)
{
	t_token *t = tokens;

	while (t)
	{
		if (t->type == T_REDIR_IN || t->type == T_REDIR_OUT
			|| t->type == T_APPEND || t->type == T_HEREDOC)
		{
			if (!t->next)
			{
				ft_putendl_fd("minishell: syntax error near unexpected token `newline'", STDERR_FILENO);
				g_last_status = 2;
				return (0);
			}
			if (t->next->type != T_WORD)
			{
				if (t->next->value)
					print_unexpected_token(t->next->value);
				else
					ft_putendl_fd("minishell: syntax error near unexpected token `newline'", STDERR_FILENO);
				g_last_status = 2;
				return (0);
			}
		}
		t = t->next;
	}
	return (1);
}