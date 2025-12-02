/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:11:29 by kadrouin          #+#    #+#             */
/*   Updated: 2025/12/02 04:27:14 by kadrouin         ###   ########.fr       */
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

static int quotes_balanced(const char *s)
{
	int i = 0;
	while (s && s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			char q = s[i++];
			while (s[i] && s[i] != q)
				i++;
			if (!s[i])
				return 0;
		}
		i++;
	}
	return 1;
}

int main(int argc, char **argv, char **envp)
{
	char	*line;
	t_env	*env_list;
	t_token	*token_list;
	bool	ran_any = false;

	env_list = NULL;
	disable_ctrl_echo();
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
		init_env(&env_list, envp);

	/* No early-exit in interactive mode: match bash behavior. */

	/* Rediriger le prompt de readline vers stderr (seulement en mode interactif) */
	if (isatty(STDIN_FILENO))
		rl_outstream = stderr;
	else
	{
		/* In non-tty mode, disable stdio buffering on stdin to avoid
		 * read-ahead interfering with child processes consuming remaining input */
		setvbuf(stdin, NULL, _IONBF, 0);
	}

	/* === Mode non interactif: ./minishell -c "commande" === */
	if (argc >= 3 && ft_strcmp(argv[1], "-c") == 0)
	{
		line = argv[2];
		token_list = parse_line(line);
		if (token_list)
		{
			exec_from_tokens(token_list, &env_list, envp);
		}
		/* Clean readline history to avoid leaks flagged in tests */
		clear_history();
		free_env_list(env_list);
		return (g_last_status);
	}


	/* === Boucle principale du shell === */
	while (1)
	{
		/* Mode interactif (terminal) : utiliser readline */
		if (isatty(STDIN_FILENO))
		{
			line = readline("minishell> ");
			if (!line)
			{
				call_eof_handler();
				break ;
			}
			add_history(line);
			// handle multiline when quotes are not closed
			while (line && !quotes_balanced(line))
			{
				char *cont = readline("> ");
				if (!cont)
					break;
				char *joined;
				// insert a newline between continued parts to preserve semantics
				joined = ft_strjoin(line, "\n");
				free(line);
				if (joined)
				{
					char *tmp = ft_strjoin(joined, cont);
					free(joined);
					free(cont);
					line = tmp;
				}
				else
				{
					free(cont);
					line = NULL;
				}
			}
		}
		/* Mode non-interactif (pipe/tester) : utiliser getline */
		else
		{
			size_t	len = 0;
			ssize_t	nread;
			
			line = NULL;
			nread = getline(&line, &len, stdin);
			if (nread == -1)
			{
				if (line)
					free(line);
				/* If invoked with no command and PATH is unset/empty,
				 * return 127 on immediate EOF (env -i ./minishell case). */
				if (!ran_any && argc == 1)
				{
					char *path_var = get_env_value(env_list, "PATH");
					if (!path_var || path_var[0] == '\0')
						g_last_status = 127;
				}
				break ;
			}
			/* If quotes are unbalanced, keep reading and append with embedded newlines */
			while (line && !quotes_balanced(line))
			{
				char *next = NULL;
				size_t l2 = 0;
				ssize_t r2 = getline(&next, &l2, stdin);
				if (r2 == -1)
					break;
				char *with_nl = ft_strjoin(line, "");
				free(line);
				if (!with_nl)
				{
					free(next);
					line = NULL;
					break;
				}
				char *joined = ft_strjoin(with_nl, next);
				free(with_nl);
				free(next);
				line = joined;
			}
			/* Retirer un unique newline final si présent (hors continuation) */
			if (line)
			{
				size_t L = ft_strlen(line);
				if (L > 0 && line[L - 1] == '\n')
					line[L - 1] = '\0';
			}
		}
		
		token_list = parse_line(line);
		if (!token_list)
		{
			free(line);
			/* In non-interactive mode, abort on syntax error (exit 2),
			 * matching bash behavior for script-like input blocks. */
			if (!isatty(STDIN_FILENO) && g_last_status == 2)
				break;
			continue ;
		}
		exec_from_tokens(token_list, &env_list, envp);
		ran_any = true;
		free(line);
	}
	free_env_list(env_list);
	/* Clean readline history to avoid leaks before exiting */
	clear_history();
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
