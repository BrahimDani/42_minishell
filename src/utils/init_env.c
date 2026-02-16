/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:15:58 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/10 19:10:47 by kadrouin         ###   ########.fr       */
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
		new_node = create_env_node(envp[i], *env_list);
		add_env_node(env_list, &current, new_node);
		i++;
	}
	adjust_shlvl(env_list);
}
