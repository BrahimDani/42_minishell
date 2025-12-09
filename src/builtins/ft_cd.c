/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboxuser <vboxuser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 05:11:34 by brdany            #+#    #+#             */
/*   Updated: 2025/12/09 03:54:33 by vboxuser         ###   ########.fr       */
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
	int	count;

	count = 0;
	while (tokens[count])
		count++;
	return (count);
}

static char	*get_cd_path(char **tokens, t_env *env_list, char *oldpwd, int *print)
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
		return (1);
	update_pwd_vars(env_list, oldpwd, print_new_path);
	free(oldpwd);
	if (tokens[1] && tokens[1][0] == '~' && tokens[1][1] == '/'
		&& path != tokens[1])
		free(path);
	return (0);
}

/*
  Résumé des changements

  La fonction ft_cd qui faisait 84 lignes a été divisée en 6 fonctions de maximum 25
  lignes chacune :

  Nouvelles fonctions auxiliaires (static) :

  1. handle_home_path (~10 lignes) - src/builtins/ft_cd.c:50
    - Gère le cas sans argument ou avec --
    - Retourne le chemin HOME
  2. handle_oldpwd_path (~12 lignes) - src/builtins/ft_cd.c:63
    - Gère le cas avec -
    - Retourne OLDPWD et active l'affichage du nouveau chemin
  3. expand_tilde_home (~22 lignes) - src/builtins/ft_cd.c:78
    - Gère l'expansion du tilde (~ et ~/...)
    - Construit le chemin complet depuis HOME
  4. update_pwd_vars (~15 lignes) - src/builtins/ft_cd.c:103
    - Met à jour les variables PWD et OLDPWD
    - Affiche le nouveau chemin si nécessaire
  5. get_cd_path (~9 lignes) - src/builtins/ft_cd.c:121
    - Détermine quel chemin utiliser selon l'argument
    - Orchestre les différents cas
  6. change_directory (~8 lignes) - src/builtins/ft_cd.c:133
    - Exécute le chdir() et gère les erreurs

  Fonction principale :

  7. ft_cd (~20 lignes) - src/builtins/ft_cd.c:144
    - Orchestration générale
    - Validation et nettoyage

  Toutes les fonctions respectent maintenant la limite de 25 lignes !
*/