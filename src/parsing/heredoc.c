/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 18:18:50 by vboxuser          #+#    #+#             */
/*   Updated: 2026/01/05 06:29:26 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	read_heredoc_content(int fd, char *delim, int expand, t_env *env)
{
	char	*line;

	while (1)
	{
		line = read_heredoc_line();
		if (!line)
		{
			ft_putendl_fd(
				"minishell: warning here-doc delimited by endfile", 2);
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

static void	build_heredoc_path(char *path, int size)
{
	static int	counter = 0;
	char		*num;

	num = ft_itoa(counter++);
	path[0] = '\0';
	ft_strlcpy(path, "/tmp/.minishell_heredoc_", size);
	ft_strlcat(path, num, size);
	free(num);
}

int	read_heredoc(char *delimiter, t_env *env_list, int quoted)
{
	int		fd;
	char	*clean_delim;
	int		should_expand;
	char	path[50];

	clean_delim = process_delimiter(delimiter, &should_expand,
			quoted, env_list);
	build_heredoc_path(path, 50);
	fd = open(path, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (fd == -1)
	{
		free(clean_delim);
		return (-1);
	}
	read_heredoc_content(fd, clean_delim, should_expand, env_list);
	close(fd);
	fd = open(path, O_RDONLY);
	unlink(path);
	free(clean_delim);
	return (fd);
}
