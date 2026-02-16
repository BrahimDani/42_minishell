/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 19:20:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/16 02:00:34 by kadrouin         ###   ########.fr       */
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
		if (isatty(STDIN_FILENO))
			ft_putstr_fd("exit\n", 2);
		return (NULL);
	}
	if (*line)
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
	init_signal_context(sh);
	setup_prompt_signals();
	init_env(env_list, sh->envp);
	if (isatty(STDIN_FILENO))
		rl_outstream = stderr;
	handle_cmd_mode(argc, argv, *env_list, sh);
}

static void	update_signal_status(t_shell *sh)
{
	if (sh->signal_record)
	{
		ms_status_set(sh, 128 + sh->signal_record);
		sh->signal_record = 0;
	}
	if (consume_sigint_flag() == SIGINT)
		ms_status_set(sh, 130);
}

void	main_loop(int is_interactive, t_env **env_list, t_shell *sh)
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
		update_signal_status(sh);
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
