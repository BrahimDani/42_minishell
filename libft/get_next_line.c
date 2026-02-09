/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 06:10:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/09 13:00:59 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#define BUFFER_SIZE 1024

static char	*ft_strjoin_gnl(char *s1, char *s2)
{
	char	*result;
	size_t	len1;
	size_t	len2;

	if (!s1)
		s1 = ft_strdup("");
	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = malloc(len1 + len2 + 1);
	if (!result)
		return (NULL);
	ft_memcpy(result, s1, len1);
	ft_memcpy(result + len1, s2, len2);
	result[len1 + len2] = '\0';
	free(s1);
	return (result);
}

static char	*extract_line(char **backup)
{
	char	*line;
	char	*newline_pos;
	char	*temp;
	size_t	len;

	if (!*backup || !**backup)
		return (NULL);
	newline_pos = ft_strchr(*backup, '\n');
	if (newline_pos)
	{
		len = newline_pos - *backup + 1;
		line = ft_substr(*backup, 0, len);
		temp = ft_strdup(newline_pos + 1);
		free(*backup);
		*backup = temp;
	}
	else
	{
		line = ft_strdup(*backup);
		free(*backup);
		*backup = NULL;
	}
	return (line);
}

static int	read_to_backup(int fd, char **backup)
{
	char		buffer[BUFFER_SIZE + 1];
	ssize_t		bytes_read;

	bytes_read = 1;
	while (bytes_read > 0 && (!*backup || !ft_strchr(*backup, '\n')))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			if (*backup)
				free(*backup);
			*backup = NULL;
			return (0);
		}
		buffer[bytes_read] = '\0';
		*backup = ft_strjoin_gnl(*backup, buffer);
	}
	return (1);
}

char	*get_next_line(int fd)
{
	static char	*backup;

	if (fd == -1)
	{
		if (backup)
			free(backup);
		backup = NULL;
		return (NULL);
	}
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!read_to_backup(fd, &backup))
		return (NULL);
	return (extract_line(&backup));
}
