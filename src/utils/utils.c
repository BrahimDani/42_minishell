/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 08:05:01 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/09 14:22:19 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.h"

char	**get_path_env(char *name)
{
	char	*value;
	char	**paths;

	value = getenv(name);
	if (!value)
		return (NULL);
	paths = ft_split(value, ':');
	return (paths);
}

void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

int	count_cmds(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

void	free_env_list(t_env *env_list)
{
	t_env	*current;
	t_env	*next;

	current = env_list;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}

void	close_extra_fds(void)
{
	DIR			*dir;
	int			fds[4096];
	int			count;
	int			dir_fd;

	dir = opendir("/proc/self/fd");
	if (!dir)
	{
		close_range_fallback();
		return ;
	}
	dir_fd = dirfd(dir);
	count = collect_fds(dir, dir_fd, fds, 4096);
	closedir(dir);
	close_fds_list(fds, count);
}
