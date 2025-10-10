/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brdany <brdany@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:03:26 by kadrouin          #+#    #+#             */
/*   Updated: 2025/10/01 17:09:55 by brdany           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

//Check if closed quote and if their is ("|\) in the beginning
static int  check_quote(char *line)
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

static int	check_pipe(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (i == 0 && line[i] == '|' && line[i + 1] == '|')
		{
			ft_putendl_fd("minishell: syntax error near `||'", STDERR_FILENO);
			return (0);
		}
		if (i == 0 && line[i] == '|')
		{
			ft_putendl_fd("minishell: sytnax error near `|'", STDERR_FILENO);
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
		if(line[i] == '&' && line[i + 1] == '&')
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `&&", STDERR_FILENO);
			return (0);
		}
		if(i == 0 && line[i] == '&')
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `&", STDERR_FILENO);
			return (0);
		}
		i++;
	}
	return(1);
}

static int	check_redir(char *line)
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

char	**parse_line(char *line)
{
	if (!line || !*line)
		return (NULL);
	if (empty_line(line))
		return (NULL);
	if (!check_quote(line))
		return (NULL);
	if (!check_redir(line))
		return (NULL);
	if (!check_ampersand(line))
		return (NULL);
	return (ft_split(line, ' '));
}

// void	free_token(char **tokens)
// {
// 	int	i;a

// 	i = 0;
// 	if (!tokens)
// 		return ;
// 	while (tokens[i])
// 	{
// 		free(tokens[i]);
// 		i++;
// 	}
// 	free(tokens);
// }
