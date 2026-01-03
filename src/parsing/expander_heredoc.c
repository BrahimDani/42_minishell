/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 20:55:10 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/03 20:37:34 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/parsing.h"

static char	*heredoc_escape(char *result, const char **p)
{
	char	next;

	next = *(*p + 1);
	if (next == '$' || next == '`' || next == '\\' || next == '\n')
	{
		if (next)
		{
			result = ft_strncat_free(result, next, 1);
			*p += 2;
			return (result);
		}
	}
	result = ft_strncat_free(result, **p, 1);
	(*p)++;
	return (result);
}

char	*expand_heredoc(const char *str, t_env *env_list)
{
	char		*result;
	const char	*p;

	result = ft_strdup("");
	p = str;
	while (result && *p)
	{
		if (*p == '\\')
			result = heredoc_escape(result, &p);
		else if (*p == '$')
			result = append_var_value(result, &p, env_list);
		else
		{
			result = ft_strncat_free(result, *p, 1);
			p++;
		}
	}
	return (result);
}
