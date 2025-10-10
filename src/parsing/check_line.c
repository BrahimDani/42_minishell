/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brdany <brdany@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 18:53:53 by brdany            #+#    #+#             */
/*   Updated: 2025/10/01 15:27:38 by brdany           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	check_pipe(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (i == 0 && line[i] == '|' && line[i + 1] == '|')
		{
			ft_putendl_fd("minishell: syntax error near `||'", STDERR_FILENO);
			g_last_status = 2;
			return (0);
		}
		if (i == 0 && line[i] == '|')
		{
			ft_putendl_fd("minishell: sytnax error near `|'", STDERR_FILENO);
			g_last_status = 2;
			return (0);
		}
		i++;
	}
	return (1);
}

int	check_ampersand(char *line)
{
	int	i;

	i = 0;
	while(line[i])
	{
		if(i == 0 && line[i] == '&' && line[i + 1] == '&')
		{
			ft_putendl_fd("minishell: syntax error near `&&", STDERR_FILENO);
			g_last_status = 2;
			return (0);
		}
		if(i == 0 && line[i] == '&')
		{
			ft_putendl_fd("minishell: syntax error near `&", STDERR_FILENO);
			g_last_status = 2;
			return (0);
		}
		i++;
	}
	return(1);
}

int	check_redir(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '>' || line[i] == '<')
		{
			if (line[i + 1] == '>' || line[i + 1] == '<' || !line[i + 1])
			{
				if (line[i + 2] == '|' && line[i + 3] == '|')
				{
					ft_putendl_fd("minishell: syntax error near `||'", STDERR_FILENO);
					return (0);
				}
				if (line[i + 2] == '|')
				{
					ft_putendl_fd("minishell: syntax error near `|'", STDERR_FILENO);
					return (0);
				}
				if (!line[i + 2])
				{
					ft_putendl_fd("minishell: syntax error near `newline'", STDERR_FILENO);
					return (0);
				}
			}
		}
		i++;
	}
	if (!check_pipe(line))
		return (0);
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

