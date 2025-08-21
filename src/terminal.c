/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 13:15:00 by kadrouin          #+#    #+#             */
/*   Updated: 2025/08/21 13:17:54 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#ifndef ECHOCTL
# define ECHOCTL 0001000
#endif

void disable_ctrl_echo(void)
{
    struct termios term;
    
    if (tcgetattr(STDIN_FILENO, &term) == 0)
    {
        term.c_lflag &= ~ECHOCTL;
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
    }
}

void enable_ctrl_echo(void)
{
    struct termios term;
    
    if (tcgetattr(STDIN_FILENO, &term) == 0)
    {
        term.c_lflag |= ECHOCTL;
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
    }
}
