/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brdany <brdany@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:03:26 by kadrouin          #+#    #+#             */
/*   Updated: 2025/09/16 22:10:56 by brdany           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

//Check if closed quote and if their is ("|\) in the beginning
static int  open_quote(char *line)
{
	int i;
	int quote;

	i = 0;
	while(line[i])
	{
		if (line[i] == '|')
		{
			ft_putendl_fd("minishell: syntax error near pipe", 0);
			return (0);
		}
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

char	**parse_line(char *line)
{
	if (!line || !*line)
		return (NULL);
	if (empty_line(line))
		return (NULL);
	if (!open_quote(line))
		return (NULL);
	return (ft_split(line, ' '));
}

void	free_token(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return ;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

