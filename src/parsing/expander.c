/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 10:44:43 by kadrouin          #+#    #+#             */
/*   Updated: 2025/11/04 10:24:31 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parsing.h"

static char *expand_variable(const char *str, t_env *env_list)
{
    char    *result;
    char    *var_start;
    char    *var_end;
    char    *var_name;
    char    *var_value;
    char    *exit_code;
    size_t  len;

    result = ft_strdup("");
    while (*str)
    {
        if (*str == '$')
        {
            /* === Gestion spéciale de $? === */
            if (*(str + 1) == '?')
            {
                exit_code = ft_itoa(g_last_status);
                result = ft_strjoin_free(result, exit_code, 1);
                free(exit_code);
                str += 2;  /* Sauter $ et ? */
                continue;
            }
            
            /* === Expansion des variables normales $VAR === */
            var_start = (char *)(str + 1);
            var_end = var_start;
            while (*var_end && (ft_isalnum(*var_end) || *var_end == '_'))
                var_end++;
            len = var_end - var_start;
            
            /* Si pas de nom de variable après $, on garde le $ tel quel */
            if (len == 0)
            {
                result = ft_strncat_free(result, '$', 1);
                str++;
                continue;
            }
            
            var_name = ft_substr(var_start, 0, len);
            var_value = get_env_value(env_list, var_name);
            free(var_name);
            if (var_value)
                result = ft_strjoin_free(result, var_value, 1);
            str = var_end;
        }
        else
        {
            result = ft_strncat_free(result, *str, 1);
            str++;
        }
    }
    return (result);
}

/**
 * expand_tokens - Expand les variables dans les tokens
 * Respecte le flag no_expand (single quotes)
 */
void    expand_tokens(t_token *tokens, t_env *env_list)
{
    t_token *current;
    char    *expanded_value;

    current = tokens;
    while (current)
    {
        /* === Ne pas expandre les tokens entre single quotes === */
        if (current->type == T_WORD && !current->no_expand)
        {
            expanded_value = expand_variable(current->value, env_list);
            if (expanded_value)
            {
                free(current->value);
                current->value = expanded_value;
            }
        }
        current = current->next;
    }
}

/**
 * expand_token - Expand les variables dans les commandes (ancienne version)
 * Pour compatibilité
 */
void    expand_token(t_cmd *cmd_list, t_env *env_list)
{
    t_cmd   *current_cmd = cmd_list;
    int     i;
    char    *expanded_value;

    while (current_cmd)
    {
        i = 0;
        while (current_cmd->argv && current_cmd->argv[i])
        {
            expanded_value = expand_variable(current_cmd->argv[i], env_list);
            if (expanded_value)
            {
                free(current_cmd->argv[i]);
                current_cmd->argv[i] = expanded_value;
            }
            i++;
        }
        current_cmd = current_cmd->next;
    }
}