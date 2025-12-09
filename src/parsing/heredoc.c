/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboxuser <vboxuser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 18:18:50 by vboxuser          #+#    #+#             */
/*   Updated: 2025/12/09 04:14:41 by vboxuser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*process_delimiter(char *delimiter, int *should_expand, int quoted, t_env *env_list)
{
	char	*clean_delim;
	char	*tmp;

	/* In bash/POSIX: if any quoting is present in the delimiter, the delimiter
	 * is used after quote removal without expansion, and the body is not expanded.
	 * If unquoted, expand the delimiter word and expand the body. */
	*should_expand = quoted ? 0 : 1;
	if (*should_expand)
	{
		tmp = expand_variable(delimiter, env_list);
		clean_delim = tmp ? tmp : ft_strdup(delimiter);
	}
	else
		clean_delim = ft_strdup(delimiter);
	return (clean_delim);
}

static char *read_heredoc_line(void)
{
	char    *line;
	char    *buf;
	size_t  n;
	ssize_t r;

	if (isatty(STDIN_FILENO))
	{
		line = readline("");
		return (line);
	}
	buf = NULL;
	n = 0;
	r = getline(&buf, &n, stdin);
	if (r == -1)
	{
		if (buf)
			free(buf);
		return (NULL);
	}
	if (r > 0 && buf[r - 1] == '\n')
		buf[r - 1] = '\0';
	return (buf);
}

static int	create_tmpfile(void)
{
	char	template[] = "/tmp/heredoc_XXXXXX";
	int		fd;

	fd = mkstemp(template);
	if (fd == -1)
	{
		perror("minishell: heredoc");
		return (-1);
	}
	unlink(template);
	return (fd);
}

static int	process_line_expanded(char *line, int fd, char *delim, t_env *env)
{
	char	*expanded_line;

	expanded_line = expand_heredoc(line, env);
	if (ft_strcmp(expanded_line, delim) == 0)
	{
		free(expanded_line);
		free(line);
		return (0);
	}
	write(fd, expanded_line, ft_strlen(expanded_line));
	write(fd, "\n", 1);
	free(expanded_line);
	free(line);
	return (1);
}

static int	process_line_raw(char *line, int fd, char *delim)
{
	if (ft_strcmp(line, delim) == 0)
	{
		free(line);
		return (0);
	}
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
	return (1);
}

static void	read_heredoc_content(int fd, char *delim, int expand, t_env *env)
{
	char	*line;

	while (1)
	{
		line = read_heredoc_line();
		if (!line)
		{
			ft_putendl_fd("minishell: warning here-doc delimited by endfile", 2);
			break ;
		}
		if (expand)
		{
			if (!process_line_expanded(line, fd, delim, env))
				break ;
		}
		else
		{
			if (!process_line_raw(line, fd, delim))
				break ;
		}
	}
}

int	read_heredoc(char *delimiter, t_env *env_list, int quoted)
{
	int		fd;
	char	*clean_delim;
	int		should_expand;

	clean_delim = process_delimiter(delimiter, &should_expand, quoted, env_list);
	fd = create_tmpfile();
	if (fd == -1)
	{
		free(clean_delim);
		return (-1);
	}
	read_heredoc_content(fd, clean_delim, should_expand, env_list);
	lseek(fd, 0, SEEK_SET);
	free(clean_delim);
	return (fd);
}
