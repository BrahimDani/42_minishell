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

int	create_tmpfile(void)
{
	static int	counter = 0;
	char		path[50];
	char		*num;
	char		*pid;
	int			fd;

	num = ft_itoa(counter++);
	pid = ft_itoa(getpid());
	path[0] = '\0';
	ft_strlcpy(path, "/tmp/.minishell_heredoc_", 50);
	ft_strlcat(path, pid, 50);
	ft_strlcat(path, "_", 50);
	ft_strlcat(path, num, 50);
	free(pid);
	free(num);
	fd = open(path, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("minishell: heredoc");
		return (-1);
	}
	return (fd);
}
