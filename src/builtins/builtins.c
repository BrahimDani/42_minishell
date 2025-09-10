/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brdany <brdany@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 05:08:14 by brdany            #+#    #+#             */
/*   Updated: 2025/09/08 03:00:56 by brdany           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int exec_builtin(char **tokens, t_env **env_list)
{
	if (ft_strcmp(tokens[0], "echo") == 0)
		return (ft_echo(tokens));
	if (ft_strcmp(tokens[0], "cd") == 0)
		return (ft_cd(tokens, env_list));
	else if (ft_strcmp(tokens[0], "pwd") == 0)
		return (ft_pwd());
	else if (ft_strcmp(tokens[0], "export") == 0)
		return(ft_export(tokens, env_list));
	// else if (ft_strcmp(tokens[0], "unset") == 0)
	// 	return(ft_unset());
	// else if (ft_strcmp(tokens[0], "env") == 0)
	// 	return(ft_env());
	// else if (ft_strcmp(tokens[0], "exit") == 0)
	// 	return(ft_exit());
	return (1);
}
