/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brdany <brdany@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 21:12:56 by brdany            #+#    #+#             */
/*   Updated: 2025/09/08 01:35:30 by brdany           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_echo(char **tokens)
{
	int i;
	int new_line;
	
	new_line = 1;
	i = 1;
	
	if (tokens[1] && ft_strcmp(tokens[1], "-n") == 0)
	{
		new_line = 0;
		i++;
	}
	while (tokens[i])
	{
		write (1, tokens[i], ft_strlen(tokens[i]));
		if (tokens[i + 1])
			write (1, " ", 1);
		i++;
	}
	if(new_line)
		write (1, "\n", 1);
	return (0);
}
