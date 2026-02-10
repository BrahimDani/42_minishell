/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 03:37:14 by brdany            #+#    #+#             */
/*   Updated: 2026/02/10 17:51:29 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../includes/minishell.h"

// BUILTINS UTILS
int		is_builtin(char *cmd);
int		exec_builtin(char **tokens, t_env **env_list);
// PWD
int		ft_pwd(void);
// CD
char	*get_env_value(t_env *env_list, char *key);
void	set_end_value(t_env *env_list, char *key, char *value);
int		ft_cd(char **tokens, t_env **env_list);
// CD UTILS
char	*handle_home_path(t_env *env_list, char *oldpwd);
char	*handle_oldpwd_path(t_env *env_list, char *oldpwd, int *print);
void	update_pwd_vars(t_env **env_list, char *oldpwd, int print_new_path);
int		change_directory(char *path, char *oldpwd);
int		ft_exit(char **args);
int		ft_echo(char **tokens);
// EXPORT
int		ft_export(char **tokens, t_env **env_list);
int		ft_unset(char **tokens, t_env **env_list);
void	set_env_value(t_env **env_list, char *key, char *value);
int		is_valid_identifier(const char *str);
int		cmp_env_keys(t_env *a, t_env *b);
int		env_count(t_env *env);
void	sort_env_array(t_env **arr, int n);
void	print_export_line(t_env *env);
void	handle_append_export(t_env **env_list, char *token, char *equal);
void	handle_simple_export(t_env **env_list, char *token, char *equal);
int		check_export_errors(char *token);
int		process_export_token(char *token, t_env **env_list);
t_env	**allocate_env_array(int n);
void	fill_env_array(t_env **arr, t_env *env_list);
int		print_all_exports(t_env *env_list);
// ENV
int		ft_env(t_env **env_list);

#endif
