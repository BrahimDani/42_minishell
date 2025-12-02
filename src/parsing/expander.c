/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 10:44:43 by kadrouin          #+#    #+#             */
/*   Updated: 2025/12/02 18:03:52 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parsing.h"

typedef enum e_quote_mode {
    QM_NONE = 0,
    QM_DOUBLE = 1,
    QM_SINGLE = 2
}   t_quote_mode;

static char *expand_variable_mode(const char *str, t_env *env_list, t_quote_mode mode)
{
    char    *result;
    const char *p;
    const char *var_end;
    char    *var_name;
    char    *var_value;
    char    *exit_code;

    if (mode == QM_SINGLE)
        return ft_strdup(str);
    result = ft_strdup("");
    p = str;
    while (*p)
    {
        if (*p == '\\')
        {
            if (mode == QM_DOUBLE)
            {
                char next = *(p + 1);
                if (next == '\\' || next == '"' || next == '$' || next == '`' || next == '\n')
                {
                    if (next)
                    {
                        result = ft_strncat_free(result, next, 1);
                        p += 2;
                        continue;
                    }
                }
                result = ft_strncat_free(result, *p, 1);
                p++;
                continue;
            }
            else /* QM_NONE */
            {
                if (*(p + 1))
                {
                    result = ft_strncat_free(result, *(p + 1), 1);
                    p += 2;
                    continue;
                }
                result = ft_strncat_free(result, *p, 1);
                p++;
                continue;
            }
        }
        if (*p == '$')
        {
            if (*(p + 1) == '?')
            {
                exit_code = ft_itoa(g_last_status);
                result = ft_strjoin_free(result, exit_code, 1);
                free(exit_code);
                p += 2;
                continue;
            }
            const char *var_start = (p + 1);
            var_end = var_start;
            while (*var_end && (ft_isalnum(*var_end) || *var_end == '_'))
                var_end++;
            size_t len = (size_t)(var_end - var_start);
            if (len == 0)
            {
                result = ft_strncat_free(result, '$', 1);
                p++;
                continue;
            }
            var_name = ft_substr((char *)var_start, 0, len);
            var_value = get_env_value(env_list, var_name);
            free(var_name);
            if (var_value)
                result = ft_strjoin_free(result, var_value, 1);
            p = var_end;
            continue;
        }
        result = ft_strncat_free(result, *p, 1);
        p++;
    }
    return result;
}

char *expand_variable(const char *str, t_env *env_list)
{
    return expand_variable_mode(str, env_list, QM_NONE);
}

/*
 * expand_heredoc - Expand a here-doc line with bash-like rules for unquoted delimiters:
 * - Perform $?, $VAR expansion
 * - Backslash only escapes $, `, \\ and newline; otherwise it is preserved
 * - No word splitting or pathname expansion
 */
char *expand_heredoc(const char *str, t_env *env_list)
{
    char        *result;
    const char  *p = str;
    const char  *var_end;
    char        *var_name;
    char        *var_value;
    char        *exit_code;

    result = ft_strdup("");
    while (*p)
    {
        if (*p == '\\')
        {
            char next = *(p + 1);
            if (next == '$' || next == '`' || next == '\\' || next == '\n')
            {
                if (next)
                {
                    result = ft_strncat_free(result, next, 1);
                    p += 2;
                    continue;
                }
            }
            /* Preserve backslash when not escaping a special */
            result = ft_strncat_free(result, *p, 1);
            p++;
            continue;
        }
        if (*p == '$')
        {
            if (*(p + 1) == '?')
            {
                exit_code = ft_itoa(g_last_status);
                result = ft_strjoin_free(result, exit_code, 1);
                free(exit_code);
                p += 2;
                continue;
            }
            const char *var_start = (p + 1);
            var_end = var_start;
            while (*var_end && (ft_isalnum(*var_end) || *var_end == '_'))
                var_end++;
            size_t len = (size_t)(var_end - var_start);
            if (len == 0)
            {
                result = ft_strncat_free(result, '$', 1);
                p++;
                continue;
            }
            var_name = ft_substr((char *)var_start, 0, len);
            var_value = get_env_value(env_list, var_name);
            free(var_name);
            if (var_value)
                result = ft_strjoin_free(result, var_value, 1);
            p = var_end;
            continue;
        }
        result = ft_strncat_free(result, *p, 1);
        p++;
    }
    return result;
}

/**
 * expand_tokens - Expand les variables dans les tokens
 * Respecte le flag no_expand (single quotes)
 * Perform word splitting on unquoted variable expansions (bash behavior)
 */
t_token *expand_tokens(t_token *tokens, t_env *env_list)
{
    t_token *current;
    t_token *prev;
    char    *expanded_value;
    char    *home;
    char    *joined;
    char    **split_words;
    int     i;
    t_token *new_token;
    t_token *last_new;

    /* Pre-pass: for heredoc delimiters, if any part is quoted, disable expansion */
    current = tokens;
    while (current)
    {
        if (current->type == T_HEREDOC && current->next)
        {
            t_token *d = current->next;
            int any_quoted = 0;
            t_token *scan = d;
            while (scan && scan->type == T_WORD)
            {
                if (scan->was_quoted || scan->no_expand)
                    any_quoted = 1;
                if (!scan->next || scan->next->space_before)
                    break;
                scan = scan->next;
            }
            if (any_quoted)
            {
                scan = d;
                while (scan && scan->type == T_WORD)
                {
                    scan->no_expand = 1;
                    scan->was_quoted = 1;
                    if (!scan->next || scan->next->space_before)
                        break;
                    scan = scan->next;
                }
            }
        }
        current = current->next;
    }

    prev = NULL;
    current = tokens;
    while (current)
    {
        /* === Ne pas expandre les tokens entre single quotes === */
        if (current->type == T_WORD && !current->no_expand)
        {
            /* Tilde expansion at start of unquoted word: ~ or ~/... */
            if (!current->was_quoted && current->value && current->value[0] == '~'
                && (current->value[1] == '\0' || current->value[1] == '/'))
            {
                home = get_env_value(env_list, "HOME");
                if (home)
                {
                    if (current->value[1] == '\0')
                    {
                        free(current->value);
                        current->value = ft_strdup(home);
                    }
                    else
                    {
                        joined = ft_strjoin(home, current->value + 1);
                        if (joined)
                        {
                            free(current->value);
                            current->value = joined;
                        }
                    }
                }
            }
            expanded_value = expand_variable_mode(current->value, env_list,
                current->was_quoted ? QM_DOUBLE : QM_NONE);
            if (expanded_value)
            {
                free(current->value);
                current->value = expanded_value; /* current->value now holds expanded */
                if (!current->was_quoted)
                {
                    /* Normalize tabs to spaces for IFS splitting */
                    i = 0;
                    while (current->value[i])
                    {
                        if (current->value[i] == '\t')
                            current->value[i] = ' ';
                        i++;
                    }
                    /* Measure leading/trailing spaces */
                    int leading = 0;
                    while (current->value[leading] == ' ') leading++;
                    int len = (int)ft_strlen(current->value);
                    int trailing = 0;
                    while (trailing < len && current->value[len - 1 - trailing] == ' ') trailing++;
                    /* Build fields */
                    split_words = ft_split(current->value, ' ');
                    /* Count fields */
                    int field_count = 0;
                    if (split_words)
                    {
                        while (split_words[field_count]) field_count++;
                    }
                    if (field_count == 0)
                    {
                        /* Distinguish empty string vs spaces-only */
                        if (len == 0) /* truly empty variable */
                        {
                            /* Remove token; preserve original spacing before it. */
                            int had_space_before = current->space_before;
                            t_token *next = current->next;
                            if (prev)
                                prev->next = next;
                            else
                                tokens = next;
                            if (had_space_before && next && next->space_before == 0)
                                next->space_before = 1;
                            /* Free split_words before continue */
                            if (split_words)
                            {
                                int j = 0;
                                while (split_words[j])
                                {
                                    free(split_words[j]);
                                    j++;
                                }
                                free(split_words);
                            }
                            free(current->value);
                            free(current);
                            current = (prev) ? prev->next : tokens;
                            continue;
                        }
                        /* Spaces-only: act as single separator */
                        if (prev && current->next && current->next->space_before == 0)
                            current->next->space_before = 1;
                        if (prev)
                            prev->next = current->next;
                        else
                            tokens = current->next;
                        /* Free split_words before continue */
                        if (split_words)
                        {
                            int j = 0;
                            while (split_words[j])
                            {
                                free(split_words[j]);
                                j++;
                            }
                            free(split_words);
                        }
                        free(current->value);
                        free(current);
                        current = (prev) ? prev->next : tokens;
                        continue;
                    }
                    else if (field_count == 1)
                    {
                        /* Single field: replace value with trimmed field */
                        char *single = ft_strdup(split_words[0]);
                        free(current->value);
                        current->value = single;
                        /* Leading spaces -> separation from previous token */
                        if (prev && current->space_before == 0 && leading > 0)
                            current->space_before = 1;
                        /* Trailing spaces -> separation before next quoted/unquoted adjacent */
                        if (trailing > 0 && current->next && current->next->space_before == 0)
                            current->next->space_before = 1;
                    }
                    else
                    {
                        /* Multiple fields */
                        char *first = ft_strdup(split_words[0]);
                        free(current->value);
                        current->value = first;
                        /* If no leading spaces and prev adjacent => join (space_before stays 0) */
                        if (prev && current->space_before == 0 && leading > 0)
                            current->space_before = 1;
                        last_new = current;
                        i = 1;
                        while (split_words[i])
                        {
                            new_token = malloc(sizeof(t_token));
                            if (!new_token)
                                break;
                            new_token->value = ft_strdup(split_words[i]);
                            new_token->type = T_WORD;
                            new_token->no_expand = 0;
                            new_token->was_quoted = 0;
                            new_token->space_before = 1; /* fields separated */
                            new_token->next = last_new->next;
                            last_new->next = new_token;
                            last_new = new_token;
                            i++;
                        }
                        /* Trailing spaces cause separation from following token */
                        if (trailing > 0 && last_new && last_new->next && last_new->next->space_before == 0)
                            last_new->next->space_before = 1;
                    }
                    /* Free split_words */
                    if (split_words)
                    {
                        i = 0;
                        while (split_words[i])
                        {
                            free(split_words[i]);
                            i++;
                        }
                        free(split_words);
                    }
                }
            }
        }
        prev = current;
        current = current->next;
    }
    return (tokens);
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