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

	pid = fork();
	char *full_path = find_command_path(args[0]);
	if (full_path)
	{
		if (pid == 0)
		{
			// Child process
			execve(full_path, args, env_list);
			printf("args[0] = %s\n", args[0]);
			ft_putstr_fd("minishell: execve failed\n", 2);
			exit(EXIT_FAILURE);
		}
		else if (pid < 0)
		{
			ft_putstr_fd("minishell: fork failed\n", 2);
			return (-1);
		}
		else
		{
			// Parent process
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				return (WEXITSTATUS(status));
		}
	}
	return (-1);
}
