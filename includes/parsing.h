/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:07:23 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/03 20:41:24 by kadrouin         ###   ########.fr       */
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
}t_field_ctx;

typedef struct s_split_meta
{
	int		leading;
	int		trailing;
	int		len;
}t_split_meta;

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
void	expand_token(t_cmd *cmd_list, t_env *env_list);
t_token	*expand_tokens(t_token *tokens, t_env *env_list);
char	*expand_variable(const char *str, t_env *env_list);
char	*expand_variable_mode(const char *str, t_env *env_list,
		t_quote_mode mode);
char	*append_var_value(char *result, const char **p, t_env *env_list);
char	*expand_heredoc(const char *str, t_env *env_list);
int	count_fields(char **split);
void	free_split_words(char **split);
t_token	*handle_empty_result(t_token *cur, t_token **head, t_token *prev);
t_token	*handle_spaces_only(t_token *cur, t_token **head, t_token *prev);
t_token	*append_field_token(t_token *last, char *word);
void	handle_single_field(t_token *cur, t_token *prev, char *word,
		t_split_meta *meta);
t_token	*handle_multi_fields(t_token *cur, t_token *prev,
		char **split, t_split_meta *meta);
int	count_leading_spaces(char *str);
int	count_trailing_spaces(char *str, int len);
t_token	*process_fields(t_field_ctx *ctx, char **split, t_split_meta *meta);
void	mark_heredoc_tokens(t_token *tokens);

void	exec_from_tokens(t_token *tokens, t_env **env_list, char **envp);
void	print_cmds(t_cmd *cmd);
t_cmd	*parse_tokens(t_token *tokens);
void	free_cmds(t_cmd *cmd);

#endif
