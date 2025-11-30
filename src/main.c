/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:11:29 by kadrouin          #+#    #+#             */
/*   Updated: 2025/11/30 19:21:19 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parsing.h"

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int g_last_status = 0;

/* ========================================================================== */
/*                             FONCTION MAIN                                  */
/* ========================================================================== */

int main(int argc, char **argv, char **envp)
{
	char	*line;
	t_env	*env_list;
	t_token	*token_list;

	env_list = NULL;
	disable_ctrl_echo();
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	init_env(&env_list, envp);

	/* === Mode non interactif: ./minishell -c "commande" === */
	if (argc >= 3 && ft_strcmp(argv[1], "-c") == 0)
	{
		line = argv[2];
		token_list = parse_line(line);
		if (token_list)
		{
			expand_tokens(token_list, env_list);
			exec_from_tokens(token_list, &env_list, envp);
		}
		free_env_list(env_list);
		return (g_last_status);
	}

	/* === Boucle principale du shell (mode interactif) === */
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			call_eof_handler();
			break ;
		}
		add_history(line);
		token_list = parse_line(line);
		if (!token_list)
		{
			free(line);
			continue ;
		}
		expand_tokens(token_list, env_list);
		exec_from_tokens(token_list, &env_list, envp);
		free(line);
	}
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
