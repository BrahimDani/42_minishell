/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 04:30:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/09 14:12:25 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	build_tmp_path(char *path, int size, int idx)
{
	char	*num;

	num = ft_itoa(idx);
	if (!num)
	{
		path[0] = '\0';
		return ;
	}
	path[0] = '\0';
	ft_strlcpy(path, "/tmp/.minishell_heredoc_", size);
	ft_strlcat(path, num, size);
	free(num);
}

char	*process_delimiter(char *delimiter, int *should_expand,
		int quoted)
{
	char	*clean_delim;

	*should_expand = 0;
	if (!quoted)
		*should_expand = 1;
	clean_delim = ft_strdup(delimiter);
	return (clean_delim);
}

char	*read_heredoc_line(void)
{
	char	*line;
	char	*buf;
	int		len;

	if (isatty(STDIN_FILENO))
	{
		line = readline("");
		return (line);
	}
	buf = get_next_line(STDIN_FILENO);
	if (!buf)
		return (NULL);
	len = ft_strlen(buf);
	if (len > 0 && buf[len - 1] == '\n')
		buf[len - 1] = '\0';
	return (buf);
}

int	create_tmpfile(char *path, int size)
{
	static int	counter = 0;
	int			fd;
	int			tries;

	tries = 0;
	while (tries < 1000)
	{
		build_tmp_path(path, size, counter++);
		if (path[0] == '\0')
			return (-1);
		fd = open(path, O_CREAT | O_EXCL | O_RDWR, 0600);
		if (fd >= 0)
			return (fd);
		if (errno != EEXIST)
			return (-1);
		tries++;
	}
	errno = EEXIST;
	return (-1);
}
