/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:07:23 by kadrouin          #+#    #+#             */
/*   Updated: 2025/08/26 14:07:23 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../includes/minishell.h"

typedef enum	e_token_type
{
	T_WORD,			// mot nrml
	T_PIPE,			// |
	T_REDIR_IN,		// <
	T_REDIR_OUT,	// >
	T_APPEND,		// >>
	T_HEREDOC,		// <<
}	t_token_type;

typedef struct	s_token
{
	char			*value;	// mots
	t_token_type	type;	// type de token (T_WORD , T_PIPE , ect...)
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char			**argv;		// arg (echo , mot , ect)
	char			*infile;	// ficher pour "<" ou "<<"
	char			*outfile;	// ... ">" ou ">>"
	int				append;		// 1 si ">>"
	int				heredoc;	// 1 si "<<"
	struct s_cmd	*next;
}	t_cmd;

void	free_tokens(t_token *tokens);
t_token	*parse_line(char *line);
int		empty_line(char *line);
int		valid_line(char *line);
int		check_ampersand(char *line);
int		check_redir(char *line);
int		check_pipe(char *line);
int		check_quote(char *line);
t_token	*tokenize_line(char *line);
char	**tokens_to_array(t_token *list);

//test
void	print_cmds(t_cmd *cmd);
t_cmd   *parse_tokens(t_token *tokens);
void	free_cmds(t_cmd *cmd);

#endif
