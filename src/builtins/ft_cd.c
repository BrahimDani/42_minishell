/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 05:11:34 by brdany            #+#    #+#             */
/*   Updated: 2025/09/28 14:10:58 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *get_env_value(t_env *env_list, char *key)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

void	set_end_value(t_env *env_list, char *key, char *value)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
		{
			free(env_list->value);
			env_list->value = ft_strdup(value);
			return ;
		}
		env_list = env_list->next;
	}
}

int	arg_count(char **tokens)
{
	int count = 0;
	
	while (tokens[count])
		count++;
	return (count);
}

int	ft_cd(char **tokens, t_env **env_list)
{
	char	*path;
	char	*oldpwd;
	char	*cwd;

	if (arg_count(tokens) > 2)
	{
		write (2, "cd: too many arguments\n", 23);
		return (1);
	}
	oldpwd = getcwd(NULL, 0);
	if (!tokens[1] && ft_strcmp(tokens[1], "-") == 1)
	{
		path = get_env_value(*env_list, "HOME");
		if (!path)
		{
			write (2, "cd: HOME not set\n", 17);
			free(oldpwd);
			return (1);
		}
	}
	else if (tokens[1] && ft_strcmp(tokens[1], "-") == 0)
	{
		path = get_env_value(*env_list, "OLDPWD");
		if (!path)
		{
			write(2, "cd: OLDPWD not set\n", 19);
			free(oldpwd);
			return (1);
		}
	}
	else
		path = tokens[1];
	if (chdir(path) != 0)
	{
		perror("cd");
		free(oldpwd);
		return (1);
	}
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		set_end_value(*env_list, "OLDPWD", oldpwd);
		set_end_value(*env_list, "PWD", cwd);
		free(cwd);
	}
	free(oldpwd);
	return (0);
}
