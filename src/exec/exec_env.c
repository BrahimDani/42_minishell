/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:11:54 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/03 20:03:52 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*make_env_string(t_env *node)
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

char	**build_envp_from_list(t_env *env_list)
{
	t_env	*cur;
	char	**new_envp;
	int		env_count;
	int		i;

	cur = env_list;
	env_count = 0;
	while (cur)
	{
		env_count++;
		cur = cur->next;
	}
	new_envp = malloc(sizeof(char *) * (env_count + 1));
	if (!new_envp)
		return (NULL);
	cur = env_list;
	i = 0;
	while (cur)
	{
		new_envp[i] = make_env_string(cur);
		cur = cur->next;
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

void	free_envp_array(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}
