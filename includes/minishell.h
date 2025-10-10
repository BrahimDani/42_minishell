/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brdany <brdany@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:19:35 by kadrouin          #+#    #+#             */
/*   Updated: 2025/09/05 03:43:07 by brdany           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../includes/struct.h"
# include "../includes/exec.h"
# include "../includes/parsing.h"
# include "../includes/utils.h"
# include "../libft/libft.h"
# include "../includes/builtins.h"

# include <sys/types.h>
# include <sys/wait.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <termios.h>
# include <unistd.h>
# include <stdbool.h>

// GENERAL
void	sigint_handler(int signum);
void	init_env(t_env **env_list, char **envp);
void	call_eof_handler(void);
void	disable_ctrl_echo(void);
void	enable_ctrl_echo(void);

#endif
