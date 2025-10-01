/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:11:29 by kadrouin          #+#    #+#             */
/*   Updated: 2025/10/01 22:06:25 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parsing.h"

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int g_last_status = 0;

int main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**tokens;
	t_env	*env_list = NULL;

	(void)argc;
	(void)argv;
	disable_ctrl_echo();
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	init_env(&env_list, envp);
	while (1)
	{
		line = readline("minishell> ");
		if (line)
		{
			add_history(line);
			tokens = parse_line(line);
			if (!tokens)
			{
				free(line);
				continue;
			}
			if (is_builtin(tokens[0]))
			{
				if (strcmp(tokens[0], "exit") == 0)
					ft_exit(tokens); // quitte directement
				else
					exec_builtin(tokens, &env_list);
			}
			else
				execute_command(tokens, envp);
			// free_token(tokens);
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
