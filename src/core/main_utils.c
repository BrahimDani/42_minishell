/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 19:15:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/14 00:48:41 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parsing.h"

void	handle_cmd_mode(int argc, char **argv, t_env *env_list, t_shell *sh)
{
	t_env		*env_copy;
	t_token		*tokens;
	char		*line;

	if (argc >= 3 && ft_strcmp(argv[1], "-c") == 0)
	{
		sh->cmd_mode = 1;
		env_copy = env_list;
		line = argv[2];
		tokens = parse_line(line, sh);
		if (tokens)
		{
			exec_from_tokens(tokens, &env_copy, sh);
		}
		ms_exit(ms_status_get(sh), env_copy);
	}
}
