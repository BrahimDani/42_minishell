/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 20:54:01 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/07 23:18:16 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <limits.h>
#include "../includes/minishell.h"

extern int	g_last_status;

int	ft_is_number(char *str)
{
	if (!str || !*str)
		return (0);
	if (*str == '+' || *str == '-')
		str++;
	if (!*str)
		return (0);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

static char	*strip_outer_quotes(const char *arg, int *was_alloc)
{
	size_t	len;

	*was_alloc = 0;
	if (!arg)
		return (NULL);
	len = ft_strlen(arg);
	if (len >= 2 && (arg[0] == '\'' || arg[0] == '"')
		&& arg[len - 1] == arg[0])
	{
		*was_alloc = 1;
		return (ft_substr(arg, 1, len - 2));
	}
	return ((char *)arg);
}

static int	is_overflow_long(char *str)
{
	if (!ft_is_number(str))
		return (0);
	if (ft_strlen(str) > 20)
		return (0);
	if (ft_atoll(str) > LLONG_MAX || ft_atoll(str) < LLONG_MIN)
		return (0);
	return (1);
}

static void	exit_parse(char **args, char *number, int was_alloc)
{
	if (is_overflow_long(number))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		if (was_alloc)
			free(number);
		exit(2);
	}
}

int	ft_exit(char **args)
{
	long	code;
	char	*number;
	int		was_alloc;

	code = g_last_status;
	if (args[1])
	{
		number = strip_outer_quotes(args[1], &was_alloc);
		exit_parse(args, number, was_alloc);
		code = ft_atol(number) % 256;
		if (was_alloc)
			free(number);
		if (args[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			return (1);
		}
	}
	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", 2);
	exit((unsigned char)code);
}
