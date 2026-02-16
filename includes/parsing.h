/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:07:23 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/16 03:35:02 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../includes/minishell.h"

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_APPEND,
	T_HEREDOC,
}	t_token_type;

typedef enum e_quote_mode
{
	QM_NONE,
	QM_DOUBLE,
	QM_SINGLE
}	t_quote_mode;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	int				no_expand;
	int				was_quoted;
	int				space_before;
	struct s_token	*next;
}	t_token;

typedef struct s_expand_ctx
{
	t_token	**tokens;
	t_token	*prev;
	t_token	*next;
	t_env	*env_list;
}	t_expand_ctx;

typedef struct s_word_split
{
	char	**split_words;
	int		field_count;
	int		leading;
	int		trailing;
	int		len;
	int		i;
}	t_word_split;

typedef struct s_heredoc_ctx
{
	t_env	*env;
	t_shell	*sh;
}	t_heredoc_ctx;

typedef struct s_heredoc_run
{
	t_heredoc_ctx	ctx;
	int				fd;
	int				saved_stdin;
	int				should_expand;
	char			path[50];
}	t_heredoc_run;

typedef struct s_cmd
{
	char			**argv;
	char			*infile;
	char			*outfile;
	int				append;
	int				heredoc;
	int				heredoc_quoted;
	int				heredoc_fd;
	int				has_in_redir_error;
	char			*in_redir_first_error;
	int				has_out_redir_error;
	char			*out_redir_first_error;
	struct s_cmd	*next;
}	t_cmd;

int				pre_read_heredocs(t_cmd *cmd_list, t_env *env_list,
					t_shell *sh);
int				read_heredoc(char *delimiter, t_env *env_list, int quoted,
					t_shell *sh);
char			*process_delimiter(char *delimiter, int *should_expand,
					int quoted);
char			*read_heredoc_line(void);
int				create_tmpfile(char *path, int size);
int				process_line_expanded(char *line, int fd, char *delim,
					t_heredoc_ctx *ctx);
int				process_line_raw(char *line, int fd, char *delim);
char			*init_heredoc_run(t_heredoc_run *run, char *delimiter,
					int quoted, t_shell *sh);
int				read_heredoc_content(int fd, char *delim, int expand,
					t_heredoc_ctx *ctx);
int				pre_read_one(t_cmd *cmd, t_env *env_list, t_shell *sh);

int				is_ifs_char(char c);
int				count_ifs_words(char *str);
char			**split_on_ifs(char *str);
void			init_word_split(t_word_split *ws, char *value);
void			free_split_result(t_word_split *ws);
t_token			*handle_word_split(t_token **tokens, t_token *current,
					t_token *prev, t_token *next);

t_token			*new_token(char *value, t_token_type type);
t_token			*new_token_no_expand(char *value, t_token_type type);
t_token			*new_token_quoted(char *value, t_token_type type);
void			add_token_back(t_token **lst, t_token *new);
t_token_type	get_type(char *word);
char			*handle_quoted_word(char *line, int *i, char quote_char);
char			*handle_word(char *line, int *i);
int				skip_spaces(char *line, int *i);
int				handle_normal_word(char *line, int *i, t_token **token,
					int space);
int				handle_double_redir(char *line, int *i, t_token **token,
					int space);
int				handle_simple_operator(char *line, int *i, t_token **token,
					int space);
int				handle_quotes(char *line, int *i, t_token **token, int space);
void			free_tokens(t_token *tokens);
t_token			*parse_line(char *line, t_shell *sh);
int				empty_line(char *line);
int				valid_line(char *line);
int				check_ampersand(char *line, t_shell *sh);
int				check_redir(char *line);
int				check_quote(char *line);
int				check_pipe_tokens(t_token *tokens, t_shell *sh);
t_token			*tokenize_line(char *line);
int				validate_tokens_syntax(t_token *tokens, t_shell *sh);
t_token			*expand_tokens(t_token *tokens, t_env *env_list, t_shell *sh);
char			*expand_variable_mode(const char *str, t_env *env_list,
					t_quote_mode mode, t_shell *sh);
char			*append_var_value(char *result, const char **p,
					t_env *env_list, t_shell *sh);
char			*expand_heredoc(const char *str, t_env *env_list, t_shell *sh);
void			mark_heredoc_tokens(t_token *tokens);

void			init_cmd_fields(t_cmd *cmd);
void			add_cmd_to_list(t_cmd **head, t_cmd *new);
t_cmd			*create_new_cmd(t_cmd **head);
int				add_argument(t_cmd *cmd, char *value);
int				aggregate_quoted(t_token *start);
char			*join_adjacent_words(t_token **token);
int				handle_word_token(t_token **tokens, t_cmd *current);
int				check_redir_syntax(t_token *t, t_shell *sh);
void			handle_redir_in(t_cmd *cmd, char *joined);
void			handle_prev_outfile(t_cmd *cmd);
void			handle_redir_out(t_cmd *cmd, char *joined, t_token_type type);
void			handle_heredoc_redir(t_cmd *cmd, char *joined, int quoted);

void			exec_from_tokens(t_token *tokens, t_env **env_list,
					t_shell *sh);
t_cmd			*parse_tokens(t_token *tokens, t_shell *sh);
void			free_cmds(t_cmd *cmd);
void			close_parent_pipe_ends(int pipes[][2], int idx, int n_cmds);
void			close_parent_heredocs(t_cmd *cmd_list);

#endif
