/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:03:26 by kadrouin          #+#    #+#             */
/*   Updated: 2025/08/26 14:38:29 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

void	print_tokens(char **tokens)
{
	int i;

	i = 0;
<<<<<<< HEAD
	if (!tokens)
		return ;
=======
>>>>>>> 110107bc367a5fa24fa98fad3af87178e6cd444f
	while(tokens[i])
	{
		printf("token[%d] = %s\n", i, tokens[i]);
		i++;
	}
}

<<<<<<< HEAD
char **parse_line(char *line)
{
	if (!line | !*line)
		return (NULL);
	return (ft_split(line, ' '));
}

void	free_token(char **tokens)
{
	int i;

	i = 0;
	if (!tokens)
		return ;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
=======
char	**parse_line(char *line)
{
	return (ft_split(line, ' '));
}

char	*find_command_path(char *command)
{
	char *path_env;
	char **paths;
	char *full_path;
	int i;

	if (ft_strchr(command, '/'))
		return (ft_strdup(command));
	
	path_env = getenv("PATH");
	paths = ft_split(path_env, ':');
	
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(full_path, command);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);  // Commande non trouvée
}
>>>>>>> 110107bc367a5fa24fa98fad3af87178e6cd444f
