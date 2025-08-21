/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:15:58 by kadrouin          #+#    #+#             */
/*   Updated: 2025/08/21 10:22:04 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void init_env(t_env **env_list, char **envp)
{
    int i = 0;
    t_env *current = NULL;

    while (envp[i])
    {
        t_env *new_node = malloc(sizeof(t_env));
        if (!new_node)
        {
            perror("Failed to allocate memory for new node");
            exit(EXIT_FAILURE);
        }
        char *equal_sign = strchr(envp[i], '=');
        if (equal_sign)
        {
            size_t key_length = equal_sign - envp[i];
            new_node->key = strndup(envp[i], key_length);
            new_node->value = strdup(equal_sign + 1);
        }
        else
        {
            new_node->key = strdup(envp[i]);
            new_node->value = NULL;
        }
        new_node->next = NULL;

        if (*env_list == NULL)
        {
            *env_list = new_node;
            current = *env_list;
        }
        else
        {
            current->next = new_node;
            current = new_node;
        }
        i++;
    }
}