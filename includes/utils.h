/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 08:07:47 by kadrouin          #+#    #+#             */
/*   Updated: 2025/11/30 18:09:14 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "../includes/minishell.h"

char	**get_path_env(char *name);
void	free_split(char **split);
int		count_cmds(t_cmd *cmd);
void	free_env_list(t_env *env_list);

extern int g_last_status;

#endif
