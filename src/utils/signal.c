/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 12:52:57 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/16 04:17:21 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static volatile sig_atomic_t	g_last_signal = 0;

void	handle_signal_std(int signo, siginfo_t *info, void *context)
{
	static t_shell	*sh;

	(void)info;
	if (!sh)
	{
		sh = (t_shell *)context;
		return ;
	}
	g_last_signal = signo;
	ms_status_set(sh, 128 + signo);
	if (signo == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_signal_record(int signo, siginfo_t *info, void *context)
{
	static t_shell	*sh;

	(void)info;
	if (!sh)
	{
		sh = (t_shell *)context;
		return ;
	}
	g_last_signal = signo;
	sh->signal_record = signo;
}

void	handle_signal_heredoc(int signo, siginfo_t *info, void *context)
{
	static t_shell	*sh;

	(void)info;
	if (!sh)
	{
		sh = (t_shell *)context;
		return ;
	}
	g_last_signal = signo;
	ms_status_set(sh, 128 + signo);
	if (signo == SIGINT)
	{
		write(1, "\n", 1);
		close(STDIN_FILENO);
	}
}

void	init_signal_context(t_shell *sh)
{
	handle_signal_std(0, NULL, sh);
	handle_signal_record(0, NULL, sh);
	handle_signal_heredoc(0, NULL, sh);
	sh->signal_record = 0;
}

int	consume_sigint_flag(void)
{
	int	flag;

	flag = g_last_signal;
	g_last_signal = 0;
	return (flag);
}
