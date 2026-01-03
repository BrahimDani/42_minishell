/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 19:22:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/03 19:15:36 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parsing.h"

int	quotes_balanced(const char *s)
{
	int		i;
	char	q;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			q = s[i++];
			while (s[i] && s[i] != q)
				i++;
			if (!s[i])
				return (0);
		}
		i++;
	}
	return (1);
}

int	join_continuation(char **line, const char *cont)
{
	char	*joined;
	char	*tmp;

	joined = ft_strjoin(*line, "\n");
	free(*line);
	if (joined)
	{
		tmp = ft_strjoin(joined, cont);
		free(joined);
		free((char *)cont);
		*line = tmp;
		return (1);
	}
	else
	{
		free((char *)cont);
		*line = NULL;
		return (0);
	}
}

int	extend_line(char **line)
{
	char	*next;
	char	*with_nl;
	char	*joined;
	size_t	l2;
	ssize_t	r2;

	next = NULL;
	l2 = 0;
	r2 = getline(&next, &l2, stdin);
	if (r2 == -1)
		return (0);
	with_nl = ft_strjoin(*line, "");
	free(*line);
	if (!with_nl)
	{
		free(next);
		*line = NULL;
		return (0);
	}
	joined = ft_strjoin(with_nl, next);
	free(with_nl);
	free(next);
	*line = joined;
	return (1);
}

void	strip_newline(char *line)
{
	size_t	len;

	if (line)
	{
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
	}
}
