/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 04:30:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/05 06:27:38 by kadrouin         ###   ########.fr       */
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
	int			fd;

	num = ft_itoa(counter++);
	path[0] = '\0';
	ft_strlcpy(path, "/tmp/.minishell_heredoc_", 50);
	ft_strlcat(path, num, 50);
	free(num);
	fd = open(path, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("minishell: heredoc");
		return (-1);
	}
	return (fd);
}
