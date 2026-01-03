/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:11:29 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/03 19:23:02 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parsing.h"

int	g_last_status = 0;

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
	return (g_last_status);
}
