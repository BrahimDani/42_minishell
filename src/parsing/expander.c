/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 10:44:43 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/05 05:10:29 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/parsing.h"

static void	expand_tilde(t_token *token, t_env *env_list)
{
	char	*home;
	char	*joined;

	if (token->was_quoted || token->value[0] != '~')
		return ;
	if (token->value[1] != '\0' && token->value[1] != '/')
		return ;
	home = get_env_value(env_list, "HOME");
	if (!home)
		return ;
	if (token->value[1] == '\0')
	{
		free(token->value);
		token->value = ft_strdup(home);
	}
	else
	{
		joined = ft_strjoin(home, token->value + 1);
		if (joined)
		{
			free(token->value);
			token->value = joined;
		}
	}
}

static void	expand_token_value(t_token *token, t_env *env_list)
{
	char			*expanded;
	t_quote_mode	mode;

	if (!token || !token->value)
		return ;
	if (token->no_expand)
		return ;
	expand_tilde(token, env_list);
	if (token->was_quoted)
		mode = QM_DOUBLE;
	else
		mode = QM_NONE;
	expanded = expand_variable_mode(token->value, env_list, mode);
	if (expanded)
	{
		free(token->value);
		token->value = expanded;
	}
}

static t_token	*handle_empty_value(t_token **tokens, t_token *current,
		t_token *prev, t_token *next)
{
	if (current->space_before && next)
		next->space_before = 1;
	if (prev)
		prev->next = next;
	else
		*tokens = next;
	free(current->value);
	free(current);
	return (next);
}

t_token	*expand_tokens(t_token *tokens, t_env *env_list)
{
	t_token	*current;
	t_token	*next;
	t_token	*prev;

	mark_heredoc_tokens(tokens);
	current = tokens;
	prev = NULL;
	while (current)
	{
		next = current->next;
		if (current->type == T_WORD)
		{
			expand_token_value(current, env_list);
			if (!current->was_quoted && current->value
				&& current->value[0] == '\0')
				current = handle_empty_value(&tokens, current, prev, next);
			else if (!current->was_quoted && current->value
				&& (ft_strchr(current->value, ' ')
					|| ft_strchr(current->value, '\t')))
				current = handle_word_split(&tokens, current, prev, next);
		}
		prev = current;
		current = next;
	}
	return (tokens);
}

void	expand_token(t_cmd *cmd_list, t_env *env_list)
{
	t_cmd	*cmd;

	(void)env_list;
	cmd = cmd_list;
	while (cmd)
	{
		cmd = cmd->next;
	}
}
