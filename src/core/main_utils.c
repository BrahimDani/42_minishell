/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 19:15:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/09 13:07:15 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parsing.h"

void	handle_cmd_mode(int argc, char **argv, t_env *env_list,
	char **envp)
{
	int		pipefd[2];
	t_env	*env_copy;

	if (argc >= 3 && ft_strcmp(argv[1], "-c") == 0)
	{
		if (pipe(pipefd) == -1)
		{
			perror("minishell: pipe");
			ms_exit(1, env_list);
		}
		if (argv[2] && *argv[2])
			write(pipefd[1], argv[2], ft_strlen(argv[2]));
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		env_copy = env_list;
		main_loop(0, &env_copy, envp);
		clear_history();
		free_env_list(env_copy);
		ms_exit(g_last_status, env_copy);
	}
}
