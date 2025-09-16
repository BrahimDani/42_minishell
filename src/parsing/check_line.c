/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brdany <brdany@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 18:53:53 by brdany            #+#    #+#             */
/*   Updated: 2025/09/16 21:51:06 by brdany           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


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

