/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 04:40:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/05 04:39:13 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*new_token(char *value, t_token_type type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(value);
	new->type = type;
	new->no_expand = 0;
	new->was_quoted = 0;
	new->space_before = 0;
	new->next = NULL;
	return (new);
}

t_token	*new_token_no_expand(char *value, t_token_type type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(value);
	new->type = type;
	new->no_expand = 1;
	new->was_quoted = 1;
	new->space_before = 0;
	new->next = NULL;
	return (new);
}

t_token	*new_token_quoted(char *value, t_token_type type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(value);
	new->type = type;
	new->no_expand = 0;
	new->was_quoted = 1;
	new->space_before = 0;
	new->next = NULL;
	return (new);
}

void	add_token_back(t_token **lst, t_token *new)
{
	t_token	*tmp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_token_type	get_type(char *word)
{
	if (!ft_strncmp(word, "|", 2))
		return (T_PIPE);
	if (!ft_strncmp(word, ";", 2))
		return (T_SEMICOLON);
	if (!ft_strncmp(word, "<", 2))
		return (T_REDIR_IN);
	if (!ft_strncmp(word, ">|", 3))
		return (T_REDIR_OUT);
	if (!ft_strncmp(word, ">", 2))
		return (T_REDIR_OUT);
	if (!ft_strncmp(word, ">>", 3))
		return (T_APPEND);
	if (!ft_strncmp(word, "<<", 3))
		return (T_HEREDOC);
	return (T_WORD);
}
