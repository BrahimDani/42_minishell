/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:11:29 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/09 13:15:23 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parsing.h"

int	g_last_status = 0;

void	ms_exit(int status, t_env *env_list)
{
	if (env_list)
		free_env_list(env_list);
	clear_history();
	get_next_line(-1);
	exit(status);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env_list;
	int		is_interactive;

	env_list = NULL;
	is_interactive = isatty(STDIN_FILENO);
	init_shell(argc, argv, envp, &env_list);
	main_loop(is_interactive, &env_list, envp);
	free_env_list(env_list);
	clear_history();
	get_next_line(-1);
	return (g_last_status);
}
