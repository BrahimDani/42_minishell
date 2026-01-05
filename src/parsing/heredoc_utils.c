/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 04:30:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/05 04:37:44 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*process_delimiter(char *delimiter, int *should_expand,
		int quoted, t_env *env_list)
{
	char	*clean_delim;
	char	*tmp;

	*should_expand = 0;
	if (!quoted)
		*should_expand = 1;
	if (*should_expand)
	{
		tmp = expand_variable(delimiter, env_list);
		if (tmp)
			clean_delim = tmp;
		else
			clean_delim = ft_strdup(delimiter);
	}
	else
		clean_delim = ft_strdup(delimiter);
	return (clean_delim);
}

char	*read_heredoc_line(void)
{
	char	*line;
	char	*buf;
	size_t	n;
	ssize_t	r;

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

int	create_tmpfile(void)
{
	char	template[20];
	int		fd;

	ft_strlcpy(template, "/tmp/heredoc_XXXXXX", 20);
	fd = mkstemp(template);
	if (fd == -1)
	{
		perror("minishell: heredoc");
		return (-1);
	}
	unlink(template);
	return (fd);
}
