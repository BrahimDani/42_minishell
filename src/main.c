/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD
/*   By: brdany <brdany@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:11:29 by kadrouin          #+#    #+#             */
/*   Updated: 2025/08/27 18:32:38 by brdany           ###   ########.fr       */
=======
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:11:29 by kadrouin          #+#    #+#             */
/*   Updated: 2025/08/26 14:41:03 by kadrouin         ###   ########.fr       */
>>>>>>> 110107bc367a5fa24fa98fad3af87178e6cd444f
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parsing.h"

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int	main(int argc, char **envp)
{
<<<<<<< HEAD
    (void)argc;
    
    // Désactive l'affichage de ^C
    disable_ctrl_echo();
    
    // Configure les signaux
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
    
    t_env *env_list = NULL;
    init_env(&env_list, envp);
    while (1)
    {
        char *line = readline("minishell> ");
        if (line)
        {
            add_history(line);
            char **tokens = parse_line(line);
            print_tokens(tokens);
            free_token(tokens);
            free(line);
        }
        else
        {
        	call_eof_handler();
            break;
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
=======
	(void)argc;
	
	// Désactive l'affichage de ^C
	disable_ctrl_echo();
	
	// Configure les signaux
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	
	t_env *env_list = NULL;
	init_env(&env_list, envp);
	while (1)
	{
		char *line = readline("minishell> ");
		if (line)
		{
			add_history(line);
			char **tokens = parse_line(line);
			execute_command(tokens, envp);
			free(line);
		}
		else
		{
			call_eof_handler();
			break;
		}
	}
	t_env *current = env_list;
	while (current)
	{
		t_env *next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
>>>>>>> 110107bc367a5fa24fa98fad3af87178e6cd444f

	return 0;
	
}
