/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:11:29 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/14 00:46:11 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parsing.h"

void	ms_exit(int status, t_env *env_list)
{
	if (env_list)
		free_env_list(env_list);
	rl_clear_history();
	get_next_line(-1);
	exit(status);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env_list;
	t_shell	sh;
	int		is_interactive;

	env_list = NULL;
	sh.last_status = 0;
	sh.envp = envp;
	is_interactive = isatty(STDIN_FILENO);
	init_shell(argc, argv, &env_list, &sh);
	main_loop(is_interactive, &env_list, &sh);
	free_env_list(env_list);
	rl_clear_history();
	get_next_line(-1);
	return (sh.last_status);
}
