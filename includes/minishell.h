/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD
/*   By: brdany <brdany@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:19:35 by kadrouin          #+#    #+#             */
/*   Updated: 2025/08/27 18:34:08 by brdany           ###   ########.fr       */
=======
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:19:35 by kadrouin          #+#    #+#             */
/*   Updated: 2025/08/26 14:38:12 by kadrouin         ###   ########.fr       */
>>>>>>> 110107bc367a5fa24fa98fad3af87178e6cd444f
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "../includes/struct.h"
<<<<<<< HEAD
=======
#include "../includes/exec.h"
#include "../includes/parsing.h"
>>>>>>> 110107bc367a5fa24fa98fad3af87178e6cd444f
#include "../libft/libft.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>

// KOKO
void sigint_handler(int signum);
void init_env(t_env **env_list, char **envp);
void call_eof_handler(void);
void disable_ctrl_echo(void);
void enable_ctrl_echo(void);

<<<<<<< HEAD
// BRAMS
void	free_token(char **tokens);
char    **parse_line(char *line);
void    print_tokens(char **tokens);

#endif
=======
#endif
>>>>>>> 110107bc367a5fa24fa98fad3af87178e6cd444f
