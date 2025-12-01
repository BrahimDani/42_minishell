/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:03:26 by kadrouin          #+#    #+#             */
/*   Updated: 2025/12/01 18:18:14 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

char	**tokens_to_array(t_token *list)
{
	int		count;
	t_token	*tmp = list;
	char	**arr;

	count = 0;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	count = 0;
	while (list)
	{
		arr[count++] = ft_strdup(list->value);
		list = list->next;
	}
	arr[count] = NULL;
	return (arr);
}

t_token	*parse_line(char *line)
{
	t_token *tokens;

	if (!line || !*line)
		return (NULL);
	if (empty_line(line))
		return (NULL);
	if (!check_quote(line) || !check_redir(line) || !check_ampersand (line))
	{
		g_last_status = 2;
		return (NULL);
	}
	tokens = tokenize_line(line);
	if (!tokens)
		return (NULL);
	if (!check_pipe_tokens(tokens))
	{
		free_tokens(tokens);
		return (NULL);
	}
	return (tokens);
}

static t_token	*extract_until_semicolon(t_token **tokens)
{
	t_token	*start;
	t_token	*cur;
	t_token	*prev;

	if (!tokens || !*tokens)
		return (NULL);
	start = *tokens;
	cur = *tokens;
	prev = NULL;
	while (cur && cur->type != T_SEMICOLON)
	{
		prev = cur;
		cur = cur->next;
	}
	if (cur && cur->type == T_SEMICOLON)
	{
		if (prev)
			prev->next = NULL;
		*tokens = cur->next;
		free(cur->value);
		free(cur);
	}
	else
		*tokens = NULL;
	return (start);
}

void	exec_from_tokens(t_token *tokens, t_env **env_list, char **envp)
{
	t_token	*current_block;
	t_cmd	*cmd_list;

	while (tokens)
	{
		current_block = extract_until_semicolon(&tokens);
		if (!current_block)
			continue ;
		// Expand variables AFTER splitting by ; so each block has current env state
		expand_tokens(current_block, *env_list);
		cmd_list = parse_tokens(current_block);
		free_tokens(current_block);
		if (!cmd_list)
			continue ;
		exec_cmd_list(cmd_list, env_list, envp);
		free_cmds(cmd_list);
	}
}

