/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brdany <brdany@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:03:26 by kadrouin          #+#    #+#             */
/*   Updated: 2025/09/16 17:56:00 by brdany           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

// int	empty_line(char *line)
// {
// 	int	i;

// 	i = 0;
// 	while(line[i])
// 	{
// 		if (line[i] == ' ' 
// 			&& line[i] == '\t'
// 			&& line[i] == '\n'
// 			&& line[i] == '\v'
// 			&& line[i] == '\f'
// 			&& line[i] == '\r')
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

// int	valid_line(char *line)
// {
// 	if (empty_line(line))
// 	{
// 		free(line);
// 		return (1);
// 	}
// 	if (open_quote(line) || syntax_error(line))
// 	{
// 		free(line);
// 		return (1);
// 	}
// 	return (0);
// }

char	**parse_line(char *line)
{

	if (!line || !*line)
	{
		free(line);
		return (NULL);
	}
	//valid_line(line);
	// free(line);	
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

