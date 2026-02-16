/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:19:35 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/16 04:13:53 by kadrouin         ###   ########.fr       */
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
# include <unistd.h>
# include <limits.h>
# include <stdbool.h>
# include <errno.h>

// GENERAL
void	handle_signal_std(int signo, siginfo_t *info, void *context);
void	handle_signal_record(int signo, siginfo_t *info, void *context);
void	handle_signal_heredoc(int signo, siginfo_t *info, void *context);
void	setup_signal(int signo, t_sig state);
void	init_signal_context(t_shell *sh);
void	setup_prompt_signals(void);
void	setup_parent_exec_signals(void);
void	setup_child_exec_signals(void);
void	setup_heredoc_signals(void);
void	print_sigquit_message(void);
void	init_env(t_env **env_list, char **envp);
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
int		parse_exit_args(char **args, int *should_exit, t_shell *sh);
int		ft_is_number(char *str);

#endif
