/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 03:37:14 by brdany            #+#    #+#             */
/*   Updated: 2025/12/01 18:19:19 by kadrouin         ###   ########.fr       */
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
char	*expand_tilde_home(char *token, t_env *env_list, char *oldpwd);
void	update_pwd_vars(t_env **env_list, char *oldpwd, int print_new_path);
int		change_directory(char *path, char *oldpwd);
// EXIT
int	ft_exit(char **args);
// ECHO
int		ft_echo(char **tokens);
// EXPORT
int	ft_export(char **tokens, t_env **env_list);
int	ft_unset(char **tokens, t_env **env_list);
void	set_env_value(t_env **env_list, char *key, char *value);
int		is_valid_identifier(const char *str);
// ENV
int		ft_env(t_env **env_list);

#endif
