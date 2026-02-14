/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 02:09:22 by brdany            #+#    #+#             */
/*   Updated: 2026/02/14 02:07:01 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_valid_identifier(const char *str)
{
	int	i;

	i = 0;
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=' && !(str[i] == '+' && str[i + 1] == '='))
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	set_existing_env_value(t_env *current, char *value)
{
	free(current->value);
	if (value)
		current->value = ft_strdup(value);
	else
		current->value = NULL;
}

void	set_env_value(t_env **env_list, char *key, char *value)
{
	t_env	*current;
	t_env	*new_node;

	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			set_existing_env_value(current, value);
			return ;
		}
		current = current->next;
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	new_node->next = *env_list;
	*env_list = new_node;
}

int	ft_export(char **tokens, t_env **env_list)
{
	int	i;
	int	ret;

	if (!tokens[1])
		return (print_all_exports(*env_list));
	i = 1;
	while (tokens[i])
	{
		ret = process_export_token(tokens[i], env_list);
		if (ret != 0)
			return (ret);
		i++;
	}
	return (0);
}
