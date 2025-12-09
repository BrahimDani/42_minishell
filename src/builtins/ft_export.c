/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 02:09:22 by brdany            #+#    #+#             */
/*   Updated: 2025/12/01 21:45:53 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	cmp_env_keys(t_env *a, t_env *b)
{
	return (ft_strcmp(a->key, b->key));
}

static int	env_count(t_env *env)
{
	int count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

static void	sort_env_array(t_env **arr, int n)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	while (i < n)
	{
		j = i + 1;
		while (j < n)
		{
			if (cmp_env_keys(arr[i], arr[j]) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_export_line(t_env *env)
{
	write(1, "declare -x ", 11);
	write(1, env->key, ft_strlen(env->key));
	if (env->value)
	{
		write(1, "=\"", 2);
		/* Minimal escaping for bash-like display: escape double quotes and backslashes */
		{
			const char	*val = env->value;
			while (*val)
			{
				if (*val == '"' || *val == '\\')
					write(1, "\\", 1);
				write(1, val, 1);
				val++;
			}
		}
		write(1, "\"", 1);
	}
	write(1, "\n", 1);
}

int	is_valid_identifier(const char *str)
{
	int	i;

	i = 0;
	if (!str || (!isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=' && !(str[i] == '+' && str[i + 1] == '='))
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

static int	print_all_exports(t_env *env_list)
{
	int		n;
	int		i;
	t_env	**arr;
	t_env	*cur;

	n = env_count(env_list);
	arr = (t_env **)malloc(sizeof(t_env *) * (n > 0 ? n : 1));
	if (!arr)
		return (0);
	i = -1;
	cur = env_list;
	while (cur)
		arr[++i] = cur, cur = cur->next;
	sort_env_array(arr, n);
	i = -1;
	while (++i < n)
		if (ft_strcmp(arr[i]->key, "_") != 0)
			print_export_line(arr[i]);
	free(arr);
	return (0);
}

static void	handle_append_export(t_env **env_list, char *token, char *equal)
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

static void	handle_simple_export(t_env **env_list, char *token, char *equal)
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

static int	check_export_errors(char *token)
{
	if (token[0] == '-' && token[1] != '\0')
		return (write(2, "export: ", 8), write(2, token, ft_strlen(token)),
			write(2, ": invalid option\n", 16), 2);
	if (token[0] == '\0')
		return (0);
	if (!is_valid_identifier(token))
		return (write(2, "export: `", 9), write(2, token, ft_strlen(token)),
			write(2, "': not a valid identifier\n", 26), 1);
	return (-1);
}

static int	process_export_token(char *token, t_env **env_list)
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

int	ft_export(char **tokens, t_env **env_list)
{
	int	i;
	int	ret;

	if (!tokens[1])
		return (print_all_exports(*env_list));
	i = 1;
	while (tokens[i])
	{
		ret = process_export_token(tokens[i], env_list);
		if (ret != 0)
			return (ret);
		i++;
	}
	return (0);
}
