/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 08:07:47 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/09 14:23:38 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "../includes/minishell.h"

void	free_split(char **split);
int		count_cmds(t_cmd *cmd);
void	free_env_list(t_env *env_list);
int		ms_status_get(t_shell *sh);
void	ms_status_set(t_shell *sh, int status);
int		consume_sigint_flag(void);

#endif
