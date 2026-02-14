/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 20:54:01 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/10 20:32:53 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	char	*digits;

	if (!ft_is_number(str))
		return (1);
	if (ft_strlen(str) > 20)
		return (1);
	digits = str;
	if (*digits == '+' || *digits == '-')
		digits++;
	while (*digits == '0')
		digits++;
	if (ft_atoll(str) == 0 && *digits != '\0')
		return (1);
	return (0);
}

static int	validate_exit_code(char **args, t_shell *sh, int *numeric_error)
{
	long	code;
	char	*number;
	int		was_alloc;

	*numeric_error = 0;
	code = 0;
	if (sh)
		code = sh->last_status;
	if (!args[1])
		return ((unsigned char)code);
	number = strip_outer_quotes(args[1], &was_alloc);
	if (is_overflow_long(number))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		*numeric_error = 1;
		if (was_alloc)
			free(number);
		return (2);
	}
	code = ft_atol(number) % 256;
	if (was_alloc)
		free(number);
	return ((unsigned char)code);
}

int	parse_exit_args(char **args, int *should_exit, t_shell *sh)
{
	int	code;
	int	numeric_error;

	*should_exit = 1;
	code = validate_exit_code(args, sh, &numeric_error);
	if (numeric_error)
		return (2);
	if (args[1] && args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		if (!sh || !sh->cmd_mode)
			*should_exit = 0;
		return (1);
	}
	return (code);
}

int	ft_exit(char **args, t_env *env_list, t_cmd *cmd_list, t_shell *sh)
{
	int	should_exit;
	int	code;

	code = parse_exit_args(args, &should_exit, sh);
	if (!should_exit)
		return (code);
	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", 2);
	if (cmd_list)
		free_cmds(cmd_list);
	ms_exit((unsigned char)code, env_list);
	return (0);
}
