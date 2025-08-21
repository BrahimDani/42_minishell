/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:11:29 by kadrouin          #+#    #+#             */
/*   Updated: 2025/08/21 10:40:17 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_env *env_list = NULL;
	init_env(&env_list, envp);
	while (1)
	{
		char *line = readline("minishell> ");
		if (line)
		{
			add_history(line);
			printf("You entered: %s\n", line);
			free(line);
		}
		else
		{
			printf("No input received.\n");
		}
	}
	// Free the environment list
	t_env *current = env_list;
	while (current)
	{
		t_env *next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}

	return 0;
	
}