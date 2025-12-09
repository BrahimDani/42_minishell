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

static int	check_invalid_option(char *token)
{
	if (token[0] == '-' && token[1])
	{
		ft_putstr_fd("minishell: unset: ", 2);
		ft_putstr_fd(token, 2);
		ft_putstr_fd(": invalid option\n", 2);
		ft_putstr_fd("unset: usage: unset [-f] [-v] [-n] [name ...]\n", 2);
		return (1);
	}
	return (0);
}

static void	remove_env_var(t_env **env_list, char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
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
}

int	ft_unset(char **tokens, t_env **env_list)
{
	int	i;

	if (!tokens[1])
		return (0);
	i = 1;
	while (tokens[i])
	{
		if (check_invalid_option(tokens[i]))
			return (2);
		remove_env_var(env_list, tokens[i]);
		i++;
	}
	return (0);
}
