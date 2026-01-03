/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 10:44:43 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/03 20:49:45 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/parsing.h"

static void	expand_token_value(t_token *token, t_env *env_list)
{
	char	*expanded;

	if (!token || !token->value)
		return ;
	if (token->was_quoted || token->was_quoted_heredoc)
		return ;
	expanded = expand_variable_mode(token->value, env_list, QM_NONE);
	if (expanded)
	{
		free(token->value);
		token->value = expanded;
	}
}

t_token	*expand_tokens(t_token *tokens, t_env *env_list)
{
	t_token	*current;

	mark_heredoc_tokens(tokens);
	current = tokens;
	while (current)
	{
		if (current->type == T_WORD)
			expand_token_value(current, env_list);
		current = current->next;
	}
	return (tokens);
}

void	expand_token(t_cmd *cmd_list, t_env *env_list)
{
	t_token	*token;
	t_cmd	*cmd;

	cmd = cmd_list;
	while (cmd)
	{
		token = cmd->tokens;
		while (token)
		{
			if (token->type == T_WORD)
				expand_token_value(token, env_list);
			token = token->next;
		}
		cmd = cmd->next;
	}
}
