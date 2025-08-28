/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:11:54 by kadrouin          #+#    #+#             */
/*   Updated: 2025/08/26 14:11:54 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	execute_command(char **args, char **env_list)
{
	pid_t	pid;
	int		status;
	char	*full_path;

	full_path = find_command_path(args[0]);
	if (!full_path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd("\n", 2);
		return (127);
	}
	pid = fork();
	if (pid == 0)
	{
		execve(full_path, args, env_list);
		ft_putstr_fd("minishell: execve failed\n", 2);
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		ft_putstr_fd("minishell: fork failed\n", 2);
		free(full_path);
		return (-1);
	}
	else
	{
		free(full_path);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
	}
	return (-1);
}

char	*find_command_path(char *cmd)
{
	char	**paths;
	char	*full_path;
	int		i;

	paths = get_path_env("PATH");
	if (!paths)
		return (NULL);

	i = 0;
	while (paths[i])
	{
		full_path = malloc(strlen(paths[i]) + strlen(cmd) + 2);
		if (!full_path)
			break ;
		sprintf(full_path, "%s/%s", paths[i], cmd);
		if (access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(paths);
	return (NULL);
}
