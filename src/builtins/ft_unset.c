/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:00:00 by copilot           #+#    #+#             */
/*   Updated: 2025/12/01 21:45:53 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_unset(char **tokens, t_env **env_list)
{
	int		i;
	t_env	*current;
	t_env	*prev;

	if (!tokens[1])
		return (0);
	i = 1;
	while (tokens[i])
	{
		// Check for invalid options (arguments starting with -)
		if (tokens[i][0] == '-' && tokens[i][1])
		{
			ft_putstr_fd("minishell: unset: ", 2);
			ft_putstr_fd(tokens[i], 2);
			ft_putstr_fd(": invalid option\n", 2);
			ft_putstr_fd("unset: usage: unset [-f] [-v] [-n] [name ...]\n", 2);
			return (2);
		}
		current = *env_list;
		prev = NULL;
		while (current)
		{
			if (ft_strcmp(current->key, tokens[i]) == 0)
			{
				if (prev)
					prev->next = current->next;
				else
					*env_list = current->next;
				free(current->key);
				free(current->value);
				free(current);
				break ;
			}
			prev = current;
			current = current->next;
		}
		i++;
	}
	return (0);
}
