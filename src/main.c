/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brdany <brdany@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:11:29 by kadrouin          #+#    #+#             */
/*   Updated: 2025/09/05 03:42:45 by brdany           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parsing.h"
#include "../includes/builtins.h"

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int	main(int argc, char **envp)
{
	(void)argc;
	
	// Désactive l'affichage de ^C
	disable_ctrl_echo();
	
	// Configure les signaux
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	
	t_env	*env_list = NULL;
	init_env(&env_list, envp);
	while (1)
	{
		char	*line = readline("minishell> ");
		if (line)
		{
			add_history(line);
			char	**tokens = parse_line(line);
			// print_tokens(tokens);
			if (is_builtin(tokens[0]))
				exec_builtin(tokens, &env_list);
			else
				execute_command(tokens, envp);
			free_token(tokens);
			free(line);
		}
		else
		{
			call_eof_handler();
			break ;
		}
	}
	// Free the environment list
	t_env *current = env_list;
	while (current)
	{
		t_env	*next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}

	return (0);
	
}
