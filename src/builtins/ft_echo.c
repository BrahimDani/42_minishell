/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 21:12:56 by brdany            #+#    #+#             */
/*   Updated: 2026/01/03 18:42:05 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_n_option(const char *s)
{
	int	i;

	if (!s || s[0] != '-')
		return (0);
	i = 1;
	if (s[i] != 'n')
		return (0);
	while (s[i] == 'n')
		i++;
	return (s[i] == '\0');
}

int	ft_echo(char **tokens)
{
	int	i;
	int	new_line;

	new_line = 1;
	i = 1;
	while (tokens[i] && is_n_option(tokens[i]))
	{
		new_line = 0;
		i++;
	}
	while (tokens[i])
	{
		write(1, tokens[i], ft_strlen(tokens[i]));
		if (tokens[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (new_line)
		write(1, "\n", 1);
	return (0);
}
