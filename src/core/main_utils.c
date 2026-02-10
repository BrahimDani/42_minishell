/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 19:15:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/11 00:21:47 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parsing.h"

void	handle_cmd_mode(int argc, char **argv, t_env *env_list,
	char **envp)
{
	t_env		*env_copy;
	t_token		*tokens;
	char		*line;

	if (argc >= 3 && ft_strcmp(argv[1], "-c") == 0)
	{
		env_copy = env_list;
		line = argv[2];
		tokens = parse_line(line);
		if (tokens)
		{
			exec_from_tokens(tokens, &env_copy, envp);
			free_tokens(tokens);
		}
		clear_history();
		free_env_list(env_copy);
		ms_exit(g_last_status, env_copy);
	}
}
