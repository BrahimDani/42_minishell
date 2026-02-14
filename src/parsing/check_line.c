/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 18:53:53 by brdany            #+#    #+#             */
/*   Updated: 2026/02/07 16:23:56 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/parsing.h"

int	check_quote(char *line)
{
	int	i;
	int	quote;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			quote = line[i];
			i++;
			while (line[i] && line[i] != quote)
				i++;
			if (!line[i])
			{
				ft_putendl_fd("minishell: syntax error near quote",
					STDERR_FILENO);
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
	return (1);
}

static int	ampersand_error(int is_double, t_shell *sh)
{
	if (is_double)
		ft_putendl_fd("minishell: syntax error near unexpected token `&&'",
			STDERR_FILENO);
	else
		ft_putendl_fd("minishell: syntax error near unexpected token `&'",
			STDERR_FILENO);
	ms_status_set(sh, 2);
	return (0);
}

int	check_ampersand(char *line, t_shell *sh)
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
		else if (!in_squote && !in_dquote && line[i] == '&')
			return (ampersand_error(line[i + 1] == '&', sh));
		i++;
	}
	return (1);
}

int	empty_line(char *line)
{
	const char			*ws;

	ws = " \t\n\v\f\r";
	while (*line)
	{
		if (!ft_strchr(ws, *line))
			return (0);
		line++;
	}
	return (1);
}
