/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brdany <brdany@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 02:09:22 by brdany            #+#    #+#             */
/*   Updated: 2025/09/10 02:16:23 by brdany           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_valid_identifier(const char *str)
{
	int	i;

	i = 0;
	if (!str || (!isalpha(str[0]) && str[0] != ' '))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if(!isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	set_env_value(t_env **env_list, char *key, char *value)
{
	t_env	*current;
	t_env	*new_node;
	
	current = *env_list;

	while (current)
	{
		if (strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = value ? ft_strdup(value) : NULL;
			return ;
		}
		current = current->next;
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	new_node->value = value ? ft_strdup(value) : NULL;
	new_node->next = *env_list;
	*env_list = new_node;
}

int	ft_export(char **tokens, t_env **env_list)
{
	int		i;
	char	*equal;
	char	*key;
	char	*value;
	t_env	*cur;

	if (!tokens[1])
	{
		cur = *env_list;
		while(cur)
		{
			write (1, "declare -x ", 11);
			write (1, cur->key, ft_strlen(cur->key));
			if (cur->value)
			{
				write (1, "=\"", 2);
				write (1, cur->value, ft_strlen(cur->value));
				write (1, "\"", 1);
			}
			write (1, "\n", 1);
			cur = cur->next;
		}
		return (0);
	}
	while (tokens[i])
	{
		if (!is_valid_identifier(tokens[i]))
		{
			write (2, "export: `", 9);
			write (2, tokens[i], ft_strlen(tokens[i]));
			write (2, "': not a valid identifier\n", 26);
			return (1);
		}
		equal = ft_strchr(tokens[i], '=');
		if (equal)
		{
			*equal = '\0';
			key = tokens[i];
			value =  equal + 1;
			set_env_value(env_list, key, value);
		}
		else
			set_end_value(*env_list, tokens[i], NULL);
		i++;
	}
	return (0);
}
