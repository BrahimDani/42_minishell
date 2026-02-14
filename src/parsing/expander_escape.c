/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_escape.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 20:55:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/03 20:46:30 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/parsing.h"

char	*handle_escape_double(char *result, const char **p)
{
	char	next;

	next = *(*p + 1);
	if (next == '\\' || next == '"' || next == '$'
		|| next == '`' || next == '\n')
	{
		result = ft_strncat_free(result, next, 1);
		*p += 2;
	}
	else
	{
		result = ft_strncat_free(result, **p, 1);
		(*p)++;
	}
	return (result);
}

char	*handle_escape_none(char *result, const char **p)
{
	if (*(*p + 1))
	{
		result = ft_strncat_free(result, *(*p + 1), 1);
		*p += 2;
	}
	else
	{
		result = ft_strncat_free(result, **p, 1);
		(*p)++;
	}
	return (result);
}

char	*expand_variable(const char *str, t_env *env_list, t_shell *sh)
{
	return (expand_variable_mode(str, env_list, QM_NONE, sh));
}
