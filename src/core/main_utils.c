/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 19:15:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/03 19:23:21 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parsing.h"

void	handle_cmd_mode(int argc, char **argv, t_env *env_list,
	char **envp)
{
	t_token	*token_list;

	if (argc >= 3 && ft_strcmp(argv[1], "-c") == 0)
	{
		token_list = parse_line(argv[2]);
		if (token_list)
			exec_from_tokens(token_list, &env_list, envp);
		clear_history();
		free_env_list(env_list);
		exit(g_last_status);
	}
}
