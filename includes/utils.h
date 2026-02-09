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

# include <dirent.h>

char	**get_path_env(char *name);
void	free_split(char **split);
int		count_cmds(t_cmd *cmd);
void	free_env_list(t_env *env_list);
void	close_extra_fds(void);
void	close_range_fallback(void);
int		parse_fd_name(const char *name);
int		should_keep_fd(int fd, int dir_fd);
int		collect_fds(DIR *dir, int dir_fd, int *fds, int max);
void	close_fds_list(int *fds, int count);

extern int	g_last_status;

#endif
