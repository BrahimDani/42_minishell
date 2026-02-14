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

static int	is_valid_unset_name(char *token)
{
	int	i;

	if (!token || !*token)
		return (0);
	if (!ft_isalpha(token[0]) && token[0] != '_')
		return (0);
	i = 1;
	while (token[i])
	{
		if (!ft_isalnum(token[i]) && token[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	print_unset_error(char *token)
{
	ft_putstr_fd("minishell: unset: `", 2);
	ft_putstr_fd(token, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
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
	int	status;

	if (!tokens[1])
		return (0);
	i = 1;
	status = 0;
	while (tokens[i])
	{
		if (!is_valid_unset_name(tokens[i]))
		{
			print_unset_error(tokens[i]);
			status = 1;
		}
		else
			remove_env_var(env_list, tokens[i]);
		i++;
	}
	return (status);
}
