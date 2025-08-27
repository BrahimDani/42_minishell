/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:15:58 by kadrouin          #+#    #+#             */
/*   Updated: 2025/08/26 14:20:44 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void init_env(t_env **env_list, char **envp)
{
	int i = 0;
	t_env *current = NULL;

	while (envp[i])
	{
		t_env *new_node = malloc(sizeof(t_env));
		if (!new_node)
		{
			perror("Failed to allocate memory for new node");
			exit(EXIT_FAILURE);
		}
		char *equal_sign = strchr(envp[i], '=');
		if (equal_sign)
		{
			size_t key_length = equal_sign - envp[i];
			new_node->key = strndup(envp[i], key_length);
			new_node->value = strdup(equal_sign + 1);
		}
		else
		{
			new_node->key = strdup(envp[i]);
			new_node->value = NULL;
		}
		new_node->next = NULL;

		if (*env_list == NULL)
		{
			*env_list = new_node;
			current = *env_list;
		}
		else
		{
			current->next = new_node;
			current = new_node;
		}
		i++;
	}
}

void convert_env(t_env *env_list, char ***envp)
{
	t_env	*current = env_list;
	int		count = 0;

	while (current)
	{
		count++;
		current = current->next;
	}
	*envp = malloc((count + 1) * sizeof(char *));
	if (!*envp)
	{
		perror("Failed to allocate memory for envp");
		exit(EXIT_FAILURE);
	}
	current = env_list;
	for (int i = 0; i < count; i++)
	{
		if (current->value)
			(*envp)[i] = ft_strjoin(current->key, "=");
		else
			(*envp)[i] = ft_strdup(current->key);
		current = current->next;
	}
	(*envp)[count] = NULL;
}
