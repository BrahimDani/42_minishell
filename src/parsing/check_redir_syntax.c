/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redir_syntax.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 18:53:53 by brdany            #+#    #+#             */
/*   Updated: 2026/01/03 20:25:24 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/parsing.h"

static void	print_unexpected_token(const char *tok)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putstr_fd((char *)tok, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
}

static int	is_redir_token(t_token_type type)
{
	return (type == T_REDIR_IN || type == T_REDIR_OUT
		|| type == T_APPEND || type == T_HEREDOC);
}

static int	validate_redir_token(t_token *t)
{
	if (!t->next)
	{
		ft_putendl_fd(
			"minishell: syntax error near unexpected token `newline'",
			STDERR_FILENO);
		g_last_status = 2;
		return (0);
	}
	if (t->next->type != T_WORD)
	{
		if (t->next->value)
			print_unexpected_token(t->next->value);
		else
			ft_putendl_fd(
				"minishell: syntax error near unexpected token `newline'",
				STDERR_FILENO);
		g_last_status = 2;
		return (0);
	}
	return (1);
}

int	validate_tokens_syntax(t_token *tokens)
{
	t_token	*t;

	t = tokens;
	while (t)
	{
		if (is_redir_token(t->type))
		{
			if (!validate_redir_token(t))
				return (0);
		}
		t = t->next;
	}
	return (1);
}
