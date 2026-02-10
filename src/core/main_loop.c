/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 19:20:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/10 19:58:06 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parsing.h"

char	*read_interactive_line(void)
{
	char	*line;

	line = readline("minishell> ");
	if (!line)
	{
		call_eof_handler();
		return (NULL);
	}
	add_history(line);
	while (line && !quotes_balanced(line))
	{
		if (!join_continuation(&line, readline("> ")))
			break ;
	}
	return (line);
}

char	*read_non_interactive_line(void)
{
	char	*line;

	line = get_next_line(STDIN_FILENO);
	if (!line)
		return (NULL);
	while (line && !quotes_balanced(line))
	{
		if (!extend_line(&line))
			break ;
	}
	strip_newline(line);
	return (line);
}

void	init_shell(int argc, char **argv, char **envp,
	t_env **env_list)
{
	close_extra_fds();
	disable_ctrl_echo();
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	init_env(env_list, envp);
	if (isatty(STDIN_FILENO))
		rl_outstream = stderr;
	handle_cmd_mode(argc, argv, *env_list, envp);
}

void	main_loop(int is_interactive, t_env **env_list, char **envp)
{
	char	*line;
	t_token	*token_list;

	while (1)
	{
		if (is_interactive)
			line = read_interactive_line();
		else
			line = read_non_interactive_line();
		if (!line)
			break ;
		token_list = parse_line(line);
		if (!token_list)
		{
			free(line);
			if (!is_interactive && g_last_status == 2)
				break ;
			continue ;
		}
		free(line);
		exec_from_tokens(token_list, env_list, envp);
	}
}
