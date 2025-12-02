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
	T_SEMICOLON,	// ;
	T_REDIR_IN,		// <
	T_REDIR_OUT,	// >
	T_APPEND,		// >>
	T_HEREDOC,		// <<
}	t_token_type;

typedef struct	s_token
{
	char			*value;	// mots
	t_token_type	type;	// type de token (T_WORD , T_PIPE , ect...)
	int				no_expand;	// 1 si entre single quotes (pas d'expansion)
	int				was_quoted;	// 1 si provenait de quotes (simple ou double)
	int				space_before;	// 1 s'il y avait un espace avant ce token
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char			**argv;		// arg (echo , mot , ect)
	char			*infile;	// ficher pour "<" ou "<<"
	char			*outfile;	// ... ">" ou ">>"
	int				append;		// 1 si ">>"
	int				heredoc;	// 1 si "<<"
	int				heredoc_quoted; // 1 si le delimiter etait quote
	int				heredoc_fd; // fd pre-rempli du heredoc (pre-read en mode non interactif)
	int				redirect_stderr_to_out; // 1 si pattern "2>" detecte
	int				has_in_redir_error; // 1 si une redirection d'entree invalide a ete detectee
	char			*in_redir_first_error; // premier fichier d'entree en erreur
	struct s_cmd	*next;
} 	t_cmd;
void	pre_read_heredocs(t_cmd *cmd_list, t_env *env_list);
int		read_heredoc(char *delimiter, t_env *env_list, int quoted);

void	free_tokens(t_token *tokens);
t_token	*parse_line(char *line);
int		empty_line(char *line);
int		valid_line(char *line);
int		check_ampersand(char *line);
int		check_redir(char *line);
int		check_quote(char *line);
int		check_pipe_tokens(t_token *tokens);
t_token	*tokenize_line(char *line);
int		validate_tokens_syntax(t_token *tokens);
char	**tokens_to_array(t_token *list);
void    expand_token(t_cmd *cmd_list, t_env *env_list);
t_token *expand_tokens(t_token *tokens, t_env *env_list);
char 	*expand_variable(const char *str, t_env *env_list);
char    *expand_heredoc(const char *str, t_env *env_list);

void	exec_from_tokens(t_token *tokens, t_env **env_list, char **envp);

//test
void	print_cmds(t_cmd *cmd);
t_cmd   *parse_tokens(t_token *tokens);
void	free_cmds(t_cmd *cmd);

#endif
