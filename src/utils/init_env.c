/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:15:58 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/09 13:08:47 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
		ms_exit(EXIT_FAILURE, env_list);
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
