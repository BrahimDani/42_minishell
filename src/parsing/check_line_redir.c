/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_line_redir.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 18:20:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/14 18:20:00 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/parsing.h"

static int	is_redir_start(char *line, int i)
{
	if (line[i] == '<' || line[i] == '>')
		return (1);
	return (0);
}

static void	advance_redir(char *line, int *i)
{
	if ((line[*i] == '<' || line[*i] == '>') && line[*i + 1] == line[*i])
		(*i)++;
	(*i)++;
	while (line[*i] == ' ' || line[*i] == '\t')
		(*i)++;
}

static int	redir_syntax_error(char c)
{
	if (!c)
		ft_putendl_fd("minishell: syntax error near unexpected token `newline'",
			STDERR_FILENO);
	else
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `",
			STDERR_FILENO);
		write(STDERR_FILENO, &c, 1);
		ft_putendl_fd("'", STDERR_FILENO);
	}
	return (0);
}

int	check_redir(char *line)
{
	int	i;
	int	in_squote;
	int	in_dquote;

	i = 0;
	in_squote = 0;
	in_dquote = 0;
	while (line[i])
	{
		if (!in_dquote && line[i] == '\'')
			in_squote = !in_squote;
		else if (!in_squote && line[i] == '"')
			in_dquote = !in_dquote;
		else if (!in_squote && !in_dquote && is_redir_start(line, i))
		{
			advance_redir(line, &i);
			if (!line[i] || ft_strchr("|<>", line[i]))
				return (redir_syntax_error(line[i]));
			continue ;
		}
		i++;
	}
	return (1);
}
