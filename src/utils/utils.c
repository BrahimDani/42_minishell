/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 08:05:01 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/09 13:21:18 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.h"
#include <dirent.h>

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
	struct dirent	*ent;
	int			fds[4096];
	int			count;
	int			i;
	int			fd;
	int			j;
	int			dir_fd;

	count = 0;
	dir = opendir("/proc/self/fd");
	if (!dir)
	{
		fd = 3;
		while (fd < 1024)
			close(fd++);
		return ;
	}
	dir_fd = dirfd(dir);
	while ((ent = readdir(dir)) != NULL)
	{
		j = 0;
		while (ent->d_name[j])
		{
			if (!ft_isdigit((unsigned char)ent->d_name[j]))
			{
				j = -1;
				break ;
			}
			j++;
		}
		if (j <= 0)
			continue ;
		fd = ft_atoi(ent->d_name);
		if (fd == dir_fd)
			continue ;
		if (fd > 1024)
			continue ;
		if (fd > 2 && count < 4096)
			fds[count++] = fd;
	}
	closedir(dir);
	i = 0;
	while (i < count)
	{
		if (fcntl(fds[i], F_GETFD) != -1)
			close(fds[i]);
		i++;
	}
}
