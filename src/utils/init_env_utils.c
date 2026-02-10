/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 19:05:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/10 19:09:27 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*create_env_node(char *env_str, t_env *env_list)
{
	t_env	*new_node;
	char	*equal_sign;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		perror("Failed to allocate memory for new node");
		ms_exit(EXIT_FAILURE, env_list);
	}
	equal_sign = strchr(env_str, '=');
	if (equal_sign)
	{
		new_node->key = ft_substr(env_str, 0, equal_sign - env_str);
		new_node->value = strdup(equal_sign + 1);
	}
	else
	{
		new_node->key = strdup(env_str);
		new_node->value = NULL;
	}
	new_node->next = NULL;
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

int	count_env_nodes(t_env *env_list)
{
	t_env	*current;
	int		count;

	current = env_list;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

char	*format_env_entry(t_env *node)
{
	char	*tmp;
	char	*result;

	if (node->value)
	{
		tmp = ft_strjoin(node->key, "=");
		result = ft_strjoin(tmp, node->value);
		free(tmp);
		return (result);
	}
	else
		return (ft_strdup(node->key));
}
