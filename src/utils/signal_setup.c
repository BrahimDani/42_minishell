/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_setup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 00:30:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/16 01:54:59 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	setup_prompt_signals(void)
{
	setup_signal(SIGINT, SIG_STANDARD);
	setup_signal(SIGQUIT, SIG_IGNORE);
}

void	setup_parent_exec_signals(void)
{
	setup_signal(SIGINT, SIG_RECORD);
	setup_signal(SIGQUIT, SIG_IGNORE);
}

void	setup_child_exec_signals(void)
{
	setup_signal(SIGINT, SIG_DEFAULT);
	setup_signal(SIGQUIT, SIG_DEFAULT);
}

void	setup_heredoc_signals(void)
{
	setup_signal(SIGINT, SIG_HEREDOC);
	setup_signal(SIGQUIT, SIG_IGNORE);
}

void	print_sigquit_message(void)
{
	ft_putstr_fd("Quit\n", 2);
}
