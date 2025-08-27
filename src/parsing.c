/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brdany <brdany@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:03:26 by kadrouin          #+#    #+#             */
/*   Updated: 2025/08/27 18:41:10 by brdany           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

void	print_tokens(char **tokens)
{
	int i;

	i = 0;
	if (!tokens)
		return ;
	while(tokens[i])
	{
		printf("token[%d] = %s\n", i, tokens[i]);
		i++;
	}
}

char **parse_line(char *line)
{
	if (!line | !*line)
		return (NULL);
	return (ft_split(line, ' '));
}

void	free_token(char **tokens)
{
	int i;

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
