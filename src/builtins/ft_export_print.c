/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_print.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 18:50:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/03 18:45:54 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	**allocate_env_array(int n)
{
	t_env	**arr;

	if (n > 0)
		arr = (t_env **)malloc(sizeof(t_env *) * n);
	else
		arr = (t_env **)malloc(sizeof(t_env *));
	return (arr);
}

void	fill_env_array(t_env **arr, t_env *env_list)
{
	t_env	*cur;
	int		i;

	cur = env_list;
	i = 0;
	while (cur)
	{
		arr[i] = cur;
		cur = cur->next;
		i++;
	}
}

int	print_all_exports(t_env *env_list)
{
	int		n;
	int		i;
	t_env	**arr;

	n = env_count(env_list);
	arr = allocate_env_array(n);
	if (!arr)
		return (0);
	fill_env_array(arr, env_list);
	sort_env_array(arr, n);
	i = 0;
	while (i < n)
	{
		if (ft_strcmp(arr[i]->key, "_") != 0)
			print_export_line(arr[i]);
		i++;
	}
	free(arr);
	return (0);
}
