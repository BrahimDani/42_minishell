/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:11:54 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/09 11:23:06 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**split_path_from_env(t_env *env_list)
{
	char	*path;

	path = get_env_value(env_list, "PATH");
	if (path == NULL)
		return (NULL);
	if (path[0] == '\0')
		return (NULL);
	return (split_path_keep_empty(path));
}

char	*find_command_path(char *cmd, t_env *env_list)
{
	char	**paths;
	char	*full_path;
	int		i;

	paths = split_path_from_env(env_list);
	if (!paths)
		paths = split_pwd_from_env(env_list);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = test_path(paths[i], cmd);
		if (full_path)
		{
			free_split(paths);
			return (full_path);
		}
		i++;
	}
	free_split(paths);
	return (NULL);
}

int	check_special_dirs(char *cmd)
{
	if (ft_strcmp(cmd, ".") == 0 || ft_strcmp(cmd, "..") == 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		return (1);
	}
	return (0);
}

int	exec_absolute_path(char **argv, char **envp, t_env *env_list)
{
	if (access(argv[0], F_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (127);
	}
	if (access(argv[0], X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (126);
	}
	return (spawn_external(argv[0], argv, envp, env_list));
}

int	exec_from_path(char **argv, char **envp, t_env *env_list)
{
	char	*full_path;
	int		ret;

	if (check_special_dirs(argv[0]))
		return (127);
	full_path = find_command_path(argv[0], env_list);
	if (!full_path)
	{
		print_cmd_error(argv[0], "command not found");
		return (127);
	}
	if (access(full_path, X_OK) != 0)
	{
		print_cmd_error(argv[0], "Permission denied");
		free(full_path);
		return (126);
	}
	ret = spawn_external(full_path, argv, envp, env_list);
	free(full_path);
	return (ret);
}
