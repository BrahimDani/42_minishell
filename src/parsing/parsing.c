/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:03:26 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/05 05:39:36 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

char	**tokens_to_array(t_token *list)
{
	int		count;
	t_token	*tmp;
	char	**arr;

	tmp = list;
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

static void	parse_line_end(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	if (!check_pipe_tokens(tokens))
	{
		free_tokens(tokens);
		return (NULL);
	}
	if (!validate_tokens_syntax(tokens))
	{
		free_tokens(tokens);
		return (NULL);
	}
	return (1);
}

t_token	*parse_line(char *line)
{
	t_token	*tokens;

	if (!line || !*line)
		return (NULL);
	if (empty_line(line))
		return (NULL);
	if (!check_quote(line) || !check_redir(line)
		|| !check_ampersand(line))
	{
		g_last_status = 2;
		return (NULL);
	}
	tokens = tokenize_line(line);
	if (parse_line_end(tokens) == NULL)
		return (NULL);
	return (tokens);
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
		current_block = expand_tokens(current_block, *env_list);
		cmd_list = parse_tokens(current_block);
		free_tokens(current_block);
		if (!cmd_list)
			continue ;
		pre_read_heredocs(cmd_list, *env_list);
		exec_cmd_list(cmd_list, env_list, envp);
		free_cmds(cmd_list);
	}
}

void	pre_read_heredocs(t_cmd *cmd_list, t_env *env_list)
{
	t_cmd	*c;

	c = cmd_list;
	while (c)
	{
		pre_read_one(c, env_list);
		c = c->next;
	}
}
