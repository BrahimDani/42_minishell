/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:11:29 by kadrouin          #+#    #+#             */
/*   Updated: 2025/11/04 10:29:16 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parsing.h"

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int g_last_status = 0;

/* ================ CLEANUP ================ */

/**
 * Libère la liste des variables d'environnement
 */
static void	free_env_list(t_env *env_list)
{
	t_env	*current;
	t_env	*next;

	current = env_list;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}
/* ================ EXÉCUTION ================ */

/**
 * Exécute une seule commande (builtin ou externe)
 */
static void	execute_single_cmd(t_cmd *cmd, t_env **env_list, char **envp)
{
	// TODO: Gérer les redirections (infile, outfile, append, heredoc)
	
	if (!cmd->argv || !cmd->argv[0])
		return ;

	/* === Vérifier si c'est un builtin === */
	if (is_builtin(cmd->argv[0]))
	{
		if (ft_strcmp(cmd->argv[0], "exit") == 0)
		{
			g_last_status = ft_exit(cmd->argv);
			// Note: ft_exit() peut appeler exit() directement
		}
		else
			g_last_status = exec_builtin(cmd->argv, env_list);
	}
	else
	{
		/* === Commande externe === */
		g_last_status = execute_command(cmd->argv, envp);
	}
}

/**
 * Exécute toutes les commandes de la liste
 * TODO: Gérer les pipes entre commandes
 */
static void	execute_cmds(t_cmd *cmd_list, t_env **env_list, char **envp)
{
	t_cmd	*current;

	current = cmd_list;
	while (current)
	{
		execute_single_cmd(current, env_list, envp);
		current = current->next;
	}
	// TODO: Si plusieurs commandes (pipes), gérer pipe() et fork()
}

/* ========================================================================== */
/*                             FONCTION MAIN                                  */
/* ========================================================================== */

int main(int argc, char **argv, char **envp)
{
	char	*line;
	t_env	*env_list;
	t_token	*token_list;
	t_cmd	*cmd_list;

	(void)argc;
	(void)argv;
	
	/* === Initialisation === */
	env_list = NULL;
	disable_ctrl_echo();
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	init_env(&env_list, envp);

	/* === Boucle principale du shell === */
	while (1)
	{
		line = readline("minishell> ");
		
		/* === Gestion du EOF (Ctrl+D) === */
		if (!line)
		{
			call_eof_handler();
			break ;
		}

		/* === Ajout à l'historique === */
		add_history(line);

	/* === 1️⃣ PARSING : Tokenization === */
	token_list = parse_line(line);
	if (!token_list)
	{
		free(line);
		continue ;
	}

	/* === 2️⃣ EXPANSION : Variables ($VAR, $?) === */
	expand_tokens(token_list, env_list);

	/* === 3️⃣ PARSING : Conversion en commandes === */
	cmd_list = parse_tokens(token_list);
	if (!cmd_list)
	{
		free_tokens(token_list);
		free(line);
		continue ;
	}

	/* === 4️⃣ EXÉCUTION === */
	execute_cmds(cmd_list, &env_list, envp);

	/* === 6️⃣ Nettoyage === */
		free_tokens(token_list);
		free_cmds(cmd_list);
		free(line);
	}

	/* === Libération finale === */
	free_env_list(env_list);
	return (g_last_status);
}

/* ========================================================================== */
/*                          ANCIEN MAIN (VERSION 1)                          */
/* ========================================================================== */

// int main(int argc, char **argv, char **envp)
// {
// 	char	*line;
// 	char	**tokens;
// 	t_env	*env_list = NULL;

// 	(void)argc;
// 	(void)argv;
// 	disable_ctrl_echo();
// 	signal(SIGINT, sigint_handler);
// 	signal(SIGQUIT, SIG_IGN);
// 	init_env(&env_list, envp);
// 	while (1)
// 	{
// 		line = readline("minishell> ");
// 		if (line)
// 		{
// 			add_history(line);
// 			t_token	*token_list = parse_line(line);
// 			if (!token_list)
// 			{
// 				free(line);
// 				continue;
// 			}
// 			tokens = tokens_to_array(token_list);
// 			if(!tokens)
// 			{
// 				free_tokens(token_list);
// 				free(line);
// 				continue;
// 			}
// 			if (is_builtin(tokens[0]))
// 			{
// 				if (strcmp(tokens[0], "exit") == 0)
// 					ft_exit(tokens);
// 				else
// 					exec_builtin(tokens, &env_list);
// 			}
// 			else
// 				execute_command(tokens, envp);
// 			free(line);
// 		}
// 		else
// 		{
// 			call_eof_handler();
// 			break ;
// 		}
// 	}
// 	// Free the environment list
// 	t_env *current = env_list;
// 	while (current)
// 	{
// 		t_env	*next = current->next;
// 		free(current->key);
// 		free(current->value);
// 		free(current);
// 		current = next;
// 	}
// 	return (0);
// }

/* ========================================================================== */
/*                          ANCIEN MAIN (VERSION 2)                          */
/* ========================================================================== */

// int main(int argc, char **argv, char **envp)
// {
// 	char	*line;
// 	t_env	*env_list = NULL;

// 	(void)argc;
// 	(void)argv;
// 	disable_ctrl_echo();
// 	signal(SIGINT, sigint_handler);
// 	signal(SIGQUIT, SIG_IGN);
// 	init_env(&env_list, envp);

// 	while (1)
// 	{
// 		line = readline("minishell> ");
// 		if (line)
// 		{
// 			add_history(line);

// 			/* === 1️⃣ Tokenization === */
// 			t_token	*token_list = parse_line(line);
// 			if (!token_list)
// 			{
// 				free(line);
// 				continue;
// 			}

// 			/* === 2️⃣ Parsing des tokens en commandes === */
// 			t_cmd *cmd_list = parse_tokens(token_list);

// 			/* === 3️⃣ Debug print === */
// 			print_cmds(cmd_list);  // 🧠 C'est ici que tu vois le résultat de ton parsing

// 			/* === 4️⃣ Nettoyage === */
// 			free_tokens(token_list);
// 			free_cmds(cmd_list);
// 			free(line);
// 		}
// 		else
// 		{
// 			call_eof_handler();
// 			break;
// 		}
// 	}

// 	/* === Libération de la liste d'environnement === */
// 	t_env *current = env_list;
// 	while (current)
// 	{
// 		t_env	*next = current->next;
// 		free(current->key);
// 		free(current->value);
// 		free(current);
// 		current = next;
// 	}
// 	return (0);
// }
