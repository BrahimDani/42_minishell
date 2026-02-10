/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 17:42:22 by vboxuser          #+#    #+#             */
/*   Updated: 2026/02/10 17:38:19 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	process_token(t_token **tokens, t_cmd **current, t_cmd *head)
{
	if ((*tokens)->type == T_WORD)
	{
		handle_word_token(tokens, *current);
	}
	else if ((*tokens)->type == T_REDIR_IN || (*tokens)->type == T_REDIR_OUT
		|| (*tokens)->type == T_APPEND || (*tokens)->type == T_HEREDOC)
	{
		if (!handle_redir_token(tokens, *current, head))
			return (-1);
	}
	else if ((*tokens)->type == T_PIPE)
		*current = create_new_cmd(&head);
	return (1);
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*current;
	int		status;

	head = NULL;
	current = NULL;
	while (tokens)
	{
		if (!current)
			current = create_new_cmd(&head);
		status = process_token(&tokens, &current, head);
		if (status == -1)
			return (NULL);
		tokens = tokens->next;
	}
	return (head);
}
