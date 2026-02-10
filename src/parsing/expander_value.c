/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_value.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 20:55:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/10 17:36:02 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/parsing.h"

static int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static char	*expand_status_var(char *result, const char **p)
{
	char	*value;

	value = ft_itoa(g_last_status);
	if (value)
	{
		result = ft_strjoin_free(result, value, 1);
		free(value);
	}
	*p += 2;
	return (result);
}

static char	*expand_regular_var(char *result, const char **p, t_env *env_list)
{
	const char	*start;
	const char	*end;
	char		*name;
	char		*value;

	start = *p + 1;
	end = start;
	while (*end && is_var_char(*end))
		end++;
	if (end == start)
	{
		result = ft_strncat_free(result, '$', 1);
		(*p)++;
		return (result);
	}
	name = ft_substr((char *)start, 0, end - start);
	value = get_env_value(env_list, name);
	free(name);
	if (value)
		result = ft_strjoin_free(result, value, 1);
	*p = end;
	return (result);
}

char	*append_var_value(char *result, const char **p, t_env *env_list)
{
	if (*(*p + 1) == '?')
		return (expand_status_var(result, p));
	return (expand_regular_var(result, p, env_list));
}
