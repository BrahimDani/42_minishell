/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_fds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 14:19:26 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/09 14:22:09 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	close_range_fallback(void)
{
	int	fd;

	fd = 3;
	while (fd < 1024)
		close(fd++);
}

int	parse_fd_name(const char *name)
{
	int	j;

	j = 0;
	if (!name || !name[0])
		return (-1);
	while (name[j])
	{
		if (!ft_isdigit((unsigned char)name[j]))
			return (-1);
		j++;
	}
	return (ft_atoi(name));
}

int	should_keep_fd(int fd, int dir_fd)
{
	if (fd <= 2)
		return (0);
	if (fd == dir_fd)
		return (0);
	if (fd > 1024)
		return (0);
	return (1);
}

int	collect_fds(DIR *dir, int dir_fd, int *fds, int max)
{
	struct dirent	*ent;
	int				count;
	int				fd;

	count = 0;
	ent = readdir(dir);
	while (ent)
	{
		fd = parse_fd_name(ent->d_name);
		if (fd >= 0 && should_keep_fd(fd, dir_fd) && count < max)
			fds[count++] = fd;
		ent = readdir(dir);
	}
	return (count);
}

void	close_fds_list(int *fds, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (fcntl(fds[i], F_GETFD) != -1)
			close(fds[i]);
		i++;
	}
}
