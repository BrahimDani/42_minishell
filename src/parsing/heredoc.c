/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboxuser <vboxuser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 18:18:50 by vboxuser          #+#    #+#             */
/*   Updated: 2025/11/23 22:55:49 by vboxuser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*process_delimiter(char *delimiter, int *should_expand)
{
	int		len;
	char	*clean_delim;

	len = ft_strlen(delimiter);

	if (len >= 2 && ((delimiter[0] == '\'' && delimiter[len - 1] == '\'')
		|| (delimiter[0] == '"' && delimiter[len - 1] == '"')))
	{
		*should_expand = 0;
		clean_delim = ft_substr(delimiter, 1, len - 2);
		return (clean_delim);
	}
	*should_expand = 1;
	return (ft_strdup(delimiter));
}

int read_heredoc(char *delimiter, t_env *env_list)
{
	char    template[] = "/tmp/heredoc_XXXXXX";
	int     fd;
	char    *line;
	char	*clean_delim;
	int		should_expand;
	char	*expanded_line;

	clean_delim = process_delimiter(delimiter, &should_expand);
	fd = mkstemp(template);
	if (fd == -1)
	{
		perror("minishell: heredoc");
		return (-1);
	}
	unlink(template);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putendl_fd("minishell: warning here-doc delimited by endfile", 2);
			break;
		}
		if (ft_strcmp(line, clean_delim) == 0)
		{
			free(line);
			break;
		}
		if (should_expand)
		{
			expanded_line = expand_variable(line, env_list);
			write (fd, expanded_line, ft_strlen(expanded_line));
			free(expanded_line);
		}
		else
			write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	lseek(fd, 0, SEEK_SET);
	free(clean_delim);
	return (fd);
}