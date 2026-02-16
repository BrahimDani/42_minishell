/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 05:11:34 by brdany            #+#    #+#             */
/*   Updated: 2026/01/03 18:43:57 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_env_value(t_env *env_list, char *key)
{
	if (!key)
		return (NULL);
	while (env_list)
	{
		if (env_list->key && ft_strcmp(env_list->key, key) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

int	arg_count(char **tokens)
{
	int	count;

	count = 0;
	while (tokens[count])
		count++;
	return (count);
}

static char	*get_cd_path(char **tokens, t_env *env_list, char *oldpwd,
		int *print)
{
	if (!tokens[1] || (tokens[1] && ft_strcmp(tokens[1], "--") == 0))
		return (handle_home_path(env_list, oldpwd));
	else if (tokens[1] && ft_strcmp(tokens[1], "-") == 0)
		return (handle_oldpwd_path(env_list, oldpwd, print));
	else if (tokens[1][0] == '~')
		return (expand_tilde_home(tokens[1], env_list, oldpwd));
	else
		return (tokens[1]);
}

int	ft_cd(char **tokens, t_env **env_list)
{
	char	*path;
	char	*oldpwd;
	int		print_new_path;

	if (arg_count(tokens) > 2)
		return (write(2, "cd: too many arguments\n", 23), 1);
	oldpwd = getcwd(NULL, 0);
	print_new_path = 0;
	path = get_cd_path(tokens, *env_list, oldpwd, &print_new_path);
	if (!path)
		return (1);
	if (change_directory(path, oldpwd))
	{
		if (tokens[1] && tokens[1][0] == '~' && tokens[1][1] == '/'
			&& path != tokens[1])
			free(path);
		return (1);
	}
	update_pwd_vars(env_list, oldpwd, print_new_path);
	free(oldpwd);
	if (tokens[1] && tokens[1][0] == '~' && tokens[1][1] == '/'
		&& path != tokens[1])
		free(path);
	return (0);
}
