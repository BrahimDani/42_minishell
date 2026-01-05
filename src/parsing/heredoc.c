/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 18:18:50 by vboxuser          #+#    #+#             */
/*   Updated: 2026/01/05 04:37:35 by kadrouin         ###   ########.fr       */
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

int	read_heredoc(char *delimiter, t_env *env_list, int quoted)
{
	int		fd;
	char	*clean_delim;
	int		should_expand;

	clean_delim = process_delimiter(delimiter, &should_expand,
			quoted, env_list);
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
