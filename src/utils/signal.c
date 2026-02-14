/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 12:52:57 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/14 04:15:20 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static volatile sig_atomic_t	g_last_signal = 0;

void	sigint_handler(int signum)
{
	g_last_signal = signum;
	rl_done = 1;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigint_heredoc_handler(int signum)
{
	g_last_signal = signum;
	close(STDIN_FILENO);
}

int	consume_sigint_flag(void)
{
	int	flag;

	flag = g_last_signal;
	g_last_signal = 0;
	return (flag);
}

void	call_eof_handler(void)
{
	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", 2);
}
