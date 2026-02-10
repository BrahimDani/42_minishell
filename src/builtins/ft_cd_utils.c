/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 03:45:54 by vboxuser          #+#    #+#             */
/*   Updated: 2026/02/10 17:35:15 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*handle_home_path(t_env *env_list, char *oldpwd)
{
	char	*path;

	path = get_env_value(env_list, "HOME");
	if (!path)
	{
		write(2, "cd: HOME not set\n", 17);
		free(oldpwd);
	}
	return (path);
}

char	*handle_oldpwd_path(t_env *env_list, char *oldpwd, int *print)
{
	char	*path;

	path = get_env_value(env_list, "OLDPWD");
	if (!path)
	{
		write(2, "cd: OLDPWD not set\n", 19);
		free(oldpwd);
	}
	else
		*print = 1;
	return (path);
}

void	update_pwd_vars(t_env **env_list, char *oldpwd, int print_new_path)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		set_env_value(env_list, "OLDPWD", oldpwd);
		set_env_value(env_list, "PWD", cwd);
		if (print_new_path)
		{
			write(1, cwd, ft_strlen(cwd));
			write(1, "\n", 1);
		}
		free(cwd);
	}
}

int	change_directory(char *path, char *oldpwd)
{
	if (chdir(path) != 0)
	{
		perror("cd");
		free(oldpwd);
		return (1);
	}
	return (0);
}
