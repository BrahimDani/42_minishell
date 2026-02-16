/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 19:05:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/14 02:08:06 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	env_node_alloc_fail(t_env *new_node, t_env *env_list)
{
	perror("minishell: malloc");
	if (new_node)
	{
		free(new_node->key);
		free(new_node->value);
		free(new_node);
	}
	ms_exit(EXIT_FAILURE, env_list);
}

static void	fill_env_node(t_env *new_node, char *env_str, char *equal_sign,
	t_env *env_list)
{
	if (equal_sign)
	{
		new_node->key = ft_substr(env_str, 0, equal_sign - env_str);
		if (!new_node->key)
			env_node_alloc_fail(new_node, env_list);
		new_node->value = ft_strdup(equal_sign + 1);
		if (!new_node->value)
			env_node_alloc_fail(new_node, env_list);
	}
	else
	{
		new_node->key = ft_strdup(env_str);
		if (!new_node->key)
			env_node_alloc_fail(new_node, env_list);
	}
}

t_env	*create_env_node(char *env_str, t_env *env_list)
{
	t_env	*new_node;
	char	*equal_sign;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		env_node_alloc_fail(NULL, env_list);
	new_node->key = NULL;
	new_node->value = NULL;
	new_node->next = NULL;
	equal_sign = ft_strchr(env_str, '=');
	fill_env_node(new_node, env_str, equal_sign, env_list);
	return (new_node);
}

void	add_env_node(t_env **env_list, t_env **current, t_env *new_node)
{
	if (*env_list == NULL)
	{
		*env_list = new_node;
		*current = *env_list;
	}
	else
	{
		(*current)->next = new_node;
		*current = new_node;
	}
}

void	adjust_shlvl(t_env **env_list)
{
	char	*lvl_str;
	int		lvl;
	char	*new_val;

	lvl_str = get_env_value(*env_list, "SHLVL");
	if (lvl_str)
	{
		lvl = ft_atoi(lvl_str);
		lvl += 1;
	}
	else
		lvl = 1;
	new_val = ft_itoa(lvl);
	if (new_val)
	{
		set_env_value(env_list, "SHLVL", new_val);
		free(new_val);
	}
}
