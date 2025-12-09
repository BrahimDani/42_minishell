/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboxuser <vboxuser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:15:58 by kadrouin          #+#    #+#             */
/*   Updated: 2025/12/09 04:32:53 by vboxuser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_env	*create_env_node(char *env_str)
{
	t_env	*new_node;
	char	*equal_sign;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		perror("Failed to allocate memory for new node");
		exit(EXIT_FAILURE);
	}
	equal_sign = strchr(env_str, '=');
	if (equal_sign)
	{
		new_node->key = strndup(env_str, equal_sign - env_str);
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

static void	add_env_node(t_env **env_list, t_env **current, t_env *new_node)
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

static void	adjust_shlvl(t_env **env_list)
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

void	init_env(t_env **env_list, char **envp)
{
	int		i;
	t_env	*current;
	t_env	*new_node;

	i = 0;
	current = NULL;
	while (envp[i])
	{
		new_node = create_env_node(envp[i]);
		add_env_node(env_list, &current, new_node);
		i++;
	}
	adjust_shlvl(env_list);
}

static int	count_env_nodes(t_env *env_list)
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

static char	*format_env_entry(t_env *node)
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

void	convert_env(t_env *env_list, char ***envp)
{
	t_env	*current;
	int		count;
	int		i;

	count = count_env_nodes(env_list);
	*envp = malloc((count + 1) * sizeof(char *));
	if (!*envp)
	{
		perror("Failed to allocate memory for envp");
		exit(EXIT_FAILURE);
	}
	current = env_list;
	i = 0;
	while (i < count)
	{
		(*envp)[i] = format_env_entry(current);
		current = current->next;
		i++;
	}
	(*envp)[count] = NULL;
}
