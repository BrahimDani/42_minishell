/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 08:05:01 by kadrouin          #+#    #+#             */
/*   Updated: 2025/08/28 08:13:04 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.h"

char	**get_path_env(char *name)
{
	char	*value;
	char	**paths;

	value = getenv(name);
	if (!value)
		return (NULL);
	paths = ft_split(value, ':');
	return (paths);
}

void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}
