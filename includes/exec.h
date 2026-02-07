/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:25:39 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/07 17:01:23 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "../includes/minishell.h"
# include <sys/types.h>

typedef int	(*t_pipe_array)[2];

typedef struct s_pipe_ctx
{
	int			(*pipes)[2];
	t_env		**env_list;
	char		**envp;
	t_cmd		*head;
	int			n_cmds;
}				t_pipe_ctx;

char			*make_env_string(t_env *node);
char			**build_envp_from_list(t_env *env_list);
void			free_envp_array(char **envp);
void			exec_error_exit(char *full_path, char **new_envp);
void			child_process(char *full_path, char **argv, t_env *env_list);
int				spawn_external(char *full_path, char **argv, char **envp,
					t_env *env_list);
char			**split_path_from_env(t_env *env_list);
char			*join_path_cmd(char *path, char *cmd);
char			*test_path(char *path, char *cmd);
char			*find_command_path(char *command, t_env *env_list);
int				check_special_dirs(char *cmd);
void			print_cmd_error(char *cmd, char *msg);
int				exec_absolute_path(char **argv, char **envp, t_env *env_list);
int				exec_from_path(char **argv, char **envp, t_env *env_list);
int				handle_heredoc_input(t_cmd *cmd, t_env *env_list);
int				handle_file_input(char *infile);
int				handle_input_redir(t_cmd *cmd, t_env **env_list);
int				handle_output_redir(t_cmd *cmd);
void			restore_fds(int saved_stdin, int saved_stdout);
int				check_redir_errors(t_cmd *cmd);
int				setup_redirections(t_cmd *cmd, t_env **env_list,
					int *saved_in, int *saved_out);
int				setup_stderr_redir(t_cmd *cmd);
void			setup_child_pipes(int pipes[][2], int i, int n_cmds);
int				handle_child_redirs(t_cmd *cmd, t_env **env_list);
void			exec_pipeline_child_cmd(t_cmd *cmd, t_cmd *head,
					t_env **env_list, char **envp);
t_pipe_array	alloc_pipes(int n_cmds);
t_pipe_array	init_pipes_array(int n_cmds);
void			wait_all_children(pid_t *pids, int n_cmds);
int				fork_and_check(int pipes[][2], int n_cmds);
void			execute_pipeline(t_cmd *cmd_list,
					t_env **env_list, char **envp, int n_cmds);
int				exec_external(char **argv, char **envp, t_env **env_list);
int				run_command(t_cmd *cmd, t_env **env_list, char **envp);
void			exec_cmd_list(t_cmd *cmd_list, t_env **env_list, char **envp);
int				create_pipes(int pipes[][2], int n_cmds);
void			close_pipes(int pipes[][2], int count);

#endif
