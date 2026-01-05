/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:07:23 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/05 05:45:11 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../includes/minishell.h"

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_SEMICOLON,
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

typedef struct s_field_ctx
{
	t_token		**head;
	t_token		*cur;
	t_token		**prev;
}	t_field_ctx;

typedef struct s_split_meta
{
	int	leading;
	int	trailing;
	int	len;
}	t_split_meta;

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
typedef struct s_cmd
{
	char			**argv;
	char			*infile;
	char			*outfile;
	char			*errfile;
	int				append;
	int				err_append;
	int				heredoc;
	int				heredoc_quoted;
	int				heredoc_fd;
	int				redirect_stderr_to_out;
	int				has_in_redir_error;
	char			*in_redir_first_error;
	int				has_out_redir_error;
	char			*out_redir_first_error;
	struct s_cmd	*next;
}	t_cmd;
void			pre_read_heredocs(t_cmd *cmd_list, t_env *env_list);
int				read_heredoc(char *delimiter, t_env *env_list, int quoted);
char			*process_delimiter(char *delimiter, int *should_expand,
					int quoted, t_env *env_list);
char			*read_heredoc_line(void);
int				create_tmpfile(void);
int				process_line_expanded(char *line, int fd, char *delim,
					t_env *env);
int				process_line_raw(char *line, int fd, char *delim);
void			read_heredoc_content(int fd, char *delim, int expand,
					t_env *env);
void			pre_read_one(t_cmd *cmd, t_env *env_list);

int				create_tmpfile(void);
int				process_line_expanded(char *line, int fd, char *delim,
					t_env *env);
int				process_line_raw(char *line, int fd, char *delim);
void			read_heredoc_content(int fd, char *delim, int expand,
					t_env *env);
void			pre_read_one(t_cmd *cmd, t_env *env_list);

int				is_ifs_char(char c);
int				count_ifs_words(char *str);
char			**split_on_ifs(char *str);
void			init_word_split(t_word_split *ws, char *value);
void			free_split_result(t_word_split *ws);
t_token			*handle_word_split(t_token **tokens, t_token *current,
					t_token *prev, t_token *next);
t_token			*extract_until_semicolon(t_token **tokens);

t_token			*new_token(char *value, t_token_type type);
t_token			*new_token_no_expand(char *value, t_token_type type);
t_token			*new_token_quoted(char *value, t_token_type type);
void			add_token_back(t_token **lst, t_token *new);
t_token_type	get_type(char *word);
char			*handle_quoted_word(char *line, int *i, char quote_char);
char			*handle_word(char *line, int *i);
int				skip_spaces(char *line, int *i);
void			handle_normal_word(char *line, int *i, t_token **token,
					int space);
int				handle_double_redir(char *line, int *i, t_token **token,
					int space);
int				handle_fd_redir(char *line, int *i, t_token **token, int space);
int				handle_simple_operator(char *line, int *i, t_token **token,
					int space);
int				handle_quotes(char *line, int *i, t_token **token, int space);
void			free_tokens(t_token *tokens);
t_token			*parse_line(char *line);
int				empty_line(char *line);
int				valid_line(char *line);
int				check_ampersand(char *line);
int				check_redir(char *line);
int				check_quote(char *line);
int				check_pipe_tokens(t_token *tokens);
t_token			*tokenize_line(char *line);
int				validate_tokens_syntax(t_token *tokens);
char			**tokens_to_array(t_token *list);
void			expand_token(t_cmd *cmd_list, t_env *env_list);
t_token			*expand_tokens(t_token *tokens, t_env *env_list);
char			*expand_variable(const char *str, t_env *env_list);
char			*expand_variable_mode(const char *str, t_env *env_list,
					t_quote_mode mode);
char			*append_var_value(char *result, const char **p,
					t_env *env_list);
char			*expand_heredoc(const char *str, t_env *env_list);
char			*handle_escape_double(char *result, const char **p);
char			*handle_escape_none(char *result, const char **p);
int				count_fields(char **split);
void			free_split_words(char **split);
t_token			*handle_empty_result(t_token *cur, t_token **head,
					t_token *prev);
t_token			*handle_spaces_only(t_token *cur, t_token **head,
					t_token *prev);
t_token			*append_field_token(t_token *last, char *word);
void			handle_single_field(t_token *cur, t_token *prev, char *word,
					t_split_meta *meta);
t_token			*handle_multi_fields(t_token *cur, t_token *prev,
					char **split, t_split_meta *meta);
int				count_leading_spaces(char *str);
int				count_trailing_spaces(char *str, int len);
t_token			*process_fields(t_field_ctx *ctx, char **split,
					t_split_meta *meta);
void			mark_heredoc_tokens(t_token *tokens);

void			init_cmd_fields(t_cmd *cmd);
void			add_cmd_to_list(t_cmd **head, t_cmd *new);
t_cmd			*create_new_cmd(t_cmd **head);
void			add_argument(t_cmd *cmd, char *value);
int				aggregate_quoted(t_token *start);
char			*join_adjacent_words(t_token **token);
int				check_redir_syntax(t_token *t);
void			handle_redir_in(t_cmd *cmd, char *joined);
void			handle_prev_outfile(t_cmd *cmd);
void			handle_redir_out(t_cmd *cmd, char *joined, t_token_type type);
void			handle_stderr_redir(t_cmd *cmd, char *joined, int is_append);
void			handle_heredoc_redir(t_cmd *cmd, char *joined, int quoted);
int				handle_redirection(t_cmd *cmd, t_token **token);
int				is_all_digits(char *str);
int				is_stderr_redirect(t_token *token, t_cmd *current,
					t_token **next);
void			handle_word_token(t_token **tokens, t_cmd *current);
int				handle_redir_token(t_token **tokens, t_cmd *current,
					t_cmd *head);
int				process_token(t_token **tokens, t_cmd **current, t_cmd *head);

void			exec_from_tokens(t_token *tokens, t_env **env_list,
					char **envp);
void			print_cmds(t_cmd *cmd);
t_cmd			*parse_tokens(t_token *tokens);
void			free_cmds(t_cmd *cmd);

#endif
