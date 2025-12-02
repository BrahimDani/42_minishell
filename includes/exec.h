/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:25:39 by kadrouin          #+#    #+#             */
/*   Updated: 2025/08/26 14:25:39 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "../includes/minishell.h"

int		exec_external(char **argv, char **envp, t_env **env_list);

char	*find_command_path(char *command, t_env *env_list);

int		run_command(t_cmd *cmd, t_env **env_list, char **envp);

void	exec_cmd_list(t_cmd *cmd_list, t_env **env_list, char **envp);

int     create_pipes(int pipes[][2], int n_cmds);

void    close_pipes(int pipes[][2], int count);

#endif
