/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 18:46:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/03 18:42:25 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_append_export(t_env **env_list, char *token, char *equal)
{
	char	*key;
	char	*value;
	char	*existing;
	char	*new_value;

	*(equal - 1) = '\0';
	key = token;
	value = equal + 1;
	existing = get_env_value(*env_list, key);
	if (existing)
	{
		new_value = ft_strjoin(existing, value);
		if (new_value)
		{
			set_env_value(env_list, key, new_value);
			free(new_value);
		}
	}
	else
		set_env_value(env_list, key, value);
}

void	handle_simple_export(t_env **env_list, char *token, char *equal)
{
	char	*key;
	char	*value;

	if (equal)
	{
		*equal = '\0';
		key = token;
		value = equal + 1;
		set_env_value(env_list, key, value);
	}
	else
		set_env_value(env_list, token, NULL);
}

int	check_export_errors(char *token)
{
	if (token[0] == '-' && token[1] != '\0')
	{
		write(2, "export: ", 8);
		write(2, token, ft_strlen(token));
		write(2, ": invalid option\n", 17);
		return (2);
	}
	if (token[0] == '\0')
		return (0);
	if (!is_valid_identifier(token))
	{
		write(2, "export: `", 9);
		write(2, token, ft_strlen(token));
		write(2, "': not a valid identifier\n", 26);
		return (1);
	}
	return (-1);
}

int	process_export_token(char *token, t_env **env_list)
{
	char	*equal;
	int		err;

	err = check_export_errors(token);
	if (err != -1)
		return (err);
	equal = ft_strchr(token, '=');
	if (equal && equal > token && *(equal - 1) == '+')
		handle_append_export(env_list, token, equal);
	else
		handle_simple_export(env_list, token, equal);
	return (0);
}
