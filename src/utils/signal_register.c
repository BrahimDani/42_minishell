/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_register.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 04:42:20 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/16 04:42:22 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	setup_signal(int signo, t_sig state)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	if (state == SIG_DEFAULT || state == SIG_IGNORE)
	{
		sa.sa_flags = SA_RESTART;
		if (state == SIG_DEFAULT)
			sa.sa_handler = SIG_DFL;
		else
			sa.sa_handler = SIG_IGN;
	}
	else
	{
		sa.sa_flags = SA_RESTART | SA_SIGINFO;
		if (state == SIG_STANDARD)
			sa.sa_sigaction = handle_signal_std;
		else if (state == SIG_HEREDOC)
			sa.sa_sigaction = handle_signal_heredoc;
		else
			sa.sa_sigaction = handle_signal_record;
	}
	sigaction(signo, &sa, NULL);
}
