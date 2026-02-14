/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:19:35 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/10 22:14:05 by kadrouin         ###   ########.fr       */
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
# include <sys/stat.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <termios.h>
# include <unistd.h>
# include <limits.h>
# include <stdbool.h>
# include <errno.h>

// GENERAL
void	sigint_handler(int signum);
void	init_env(t_env **env_list, char **envp);
void	call_eof_handler(void);
void	disable_ctrl_echo(void);
void	enable_ctrl_echo(void);
// INIT_ENV_UTILS
t_env	*create_env_node(char *env_str, t_env *env_list);
void	add_env_node(t_env **env_list, t_env **current, t_env *new_node);
void	adjust_shlvl(t_env **env_list);
int		count_env_nodes(t_env *env_list);
char	*format_env_entry(t_env *node);
void	convert_env(t_env *env_list, char ***envp);
// MAIN_UTILS
int		quotes_balanced(const char *s);
void	handle_cmd_mode(int argc, char **argv, t_env *env_list,
			t_shell *sh);
int		join_continuation(char **line, const char *cont);
int		extend_line(char **line);
void	strip_newline(char *line);
void	ms_exit(int status, t_env *env_list);
char	*read_interactive_line(void);
char	*read_non_interactive_line(void);
void	init_shell(int argc, char **argv, t_env **env_list, t_shell *sh);
void	main_loop(int is_interactive, t_env **env_list, t_shell *sh);
int		parse_exit_args(char **args, int *should_exit, int last_status);
int		ft_is_number(char *str);

#endif
