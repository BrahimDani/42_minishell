/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 18:53:53 by brdany            #+#    #+#             */
/*   Updated: 2026/01/03 20:24:58 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/parsing.h"

static int	check_first_pipe(t_token *tokens, t_shell *sh)
{
	if (tokens->type == T_PIPE)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'",
			STDERR_FILENO);
		ms_status_set(sh, 2);
		return (0);
	}
	return (1);
}

static int	check_last_pipe(t_token *tokens, t_shell *sh)
{
	t_token	*current;
	t_token	*last;

	current = tokens;
	last = tokens;
	while (current)
	{
		last = current;
		current = current->next;
	}
	if (last->type == T_PIPE)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'",
			STDERR_FILENO);
		ms_status_set(sh, 2);
		return (0);
	}
	return (1);
}

static int	check_consecutive_pipes(t_token *tokens, t_shell *sh)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == T_PIPE && current->next
			&& current->next->type == T_PIPE)
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `|'",
				STDERR_FILENO);
			ms_status_set(sh, 2);
			return (0);
		}
		current = current->next;
	}
	return (1);
}

int	check_pipe_tokens(t_token *tokens, t_shell *sh)
{
	if (!tokens)
		return (1);
	if (!check_first_pipe(tokens, sh))
		return (0);
	if (!check_last_pipe(tokens, sh))
		return (0);
	if (!check_consecutive_pipes(tokens, sh))
		return (0);
	return (1);
}
