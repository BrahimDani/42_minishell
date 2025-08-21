/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:11:29 by kadrouin          #+#    #+#             */
/*   Updated: 2025/08/21 13:12:21 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(int argc, char **envp)
{
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
            printf("You entered: %s\n", line);
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

    return 0;
    
}