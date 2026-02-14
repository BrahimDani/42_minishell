/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:03:26 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/05 05:43:46 by kadrouin         ###   ########.fr       */
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

static int	parse_line_end(t_token *tokens, t_shell *sh)
{
	if (!tokens)
		return (0);
	if (!check_pipe_tokens(tokens, sh))
	{
		free_tokens(tokens);
		return (0);
	}
	if (!validate_tokens_syntax(tokens, sh))
	{
		free_tokens(tokens);
		return (0);
	}
	return (1);
}

t_token	*parse_line(char *line, t_shell *sh)
{
	t_token	*tokens;

	if (!line || !*line)
		return (NULL);
	if (empty_line(line))
		return (NULL);
	if (!check_quote(line) || !check_redir(line)
		|| !check_ampersand(line, sh))
	{
		ms_status_set(sh, 2);
		return (NULL);
	}
	tokens = tokenize_line(line);
	if (!parse_line_end(tokens, sh))
		return (NULL);
	return (tokens);
}

void	exec_from_tokens(t_token *tokens, t_env **env_list, t_shell *sh)
{
	t_cmd	*cmd_list;

	tokens = expand_tokens(tokens, *env_list, sh);
	cmd_list = parse_tokens(tokens, sh);
	free_tokens(tokens);
	if (!cmd_list)
		return ;
	if (!pre_read_heredocs(cmd_list, *env_list, sh))
		return (free_cmds(cmd_list));
	exec_cmd_list(cmd_list, env_list, sh);
	free_cmds(cmd_list);
}

int	pre_read_heredocs(t_cmd *cmd_list, t_env *env_list, t_shell *sh)
{
	t_cmd	*c;

	c = cmd_list;
	while (c)
	{
		if (!pre_read_one(c, env_list, sh))
			return (0);
		c = c->next;
	}
	return (1);
}
