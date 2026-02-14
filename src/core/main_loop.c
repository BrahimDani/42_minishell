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
	char	*cont;

	rl_done = 0;
	line = readline("minishell> ");
	if (!line)
	{
		call_eof_handler();
		return (NULL);
	}
	add_history(line);
	while (line && !quotes_balanced(line))
	{
		cont = readline("> ");
		if (!cont)
		{
			write(STDERR_FILENO, "\n", 1);
			break ;
		}
		if (!join_continuation(&line, cont))
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

void	init_shell(int argc, char **argv, t_env **env_list, t_shell *sh)
{
	disable_ctrl_echo();
	rl_catch_signals = 0;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	init_env(env_list, sh->envp);
	if (isatty(STDIN_FILENO))
		rl_outstream = stderr;
	handle_cmd_mode(argc, argv, *env_list, sh);
}

void	main_loop(int is_interactive, t_env **env_list, t_shell *sh)
{
	char	*line;
	t_token	*token_list;

	while (1)
	{
		if (consume_sigint_flag() == SIGINT)
			ms_status_set(sh, 130);
		if (is_interactive)
			line = read_interactive_line();
		else
			line = read_non_interactive_line();
		if (!line)
			break ;
		token_list = parse_line(line, sh);
		if (!token_list)
		{
			free(line);
			if (!is_interactive && ms_status_get(sh) == 2)
				break ;
			continue ;
		}
		free(line);
		exec_from_tokens(token_list, env_list, sh);
	}
}
