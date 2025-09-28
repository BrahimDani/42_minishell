/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 03:37:14 by brdany            #+#    #+#             */
/*   Updated: 2025/09/28 13:42:32 by kadrouin         ###   ########.fr       */
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
// ECHO
int		ft_echo(char **tokens);
// EXPORT
int		ft_export(char **tokens, t_env **env_list);
void	set_env_value(t_env **env_list, char *key, char *value);
int		is_valid_identifier(const char *str);
// ENV
int		ft_env(t_env **env_list);

#endif
