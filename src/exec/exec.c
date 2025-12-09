/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:11:54 by kadrouin          #+#    #+#             */
/*   Updated: 2025/08/26 14:11:54 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <errno.h>
#include <string.h>
#include <fcntl.h>

static char	*make_env_string(t_env *node)
{
	char	*tmp;
	char	*result;

	if (node->value)
	{
		tmp = ft_strjoin(node->key, "=");
		result = ft_strjoin(tmp, node->value);
		free(tmp);
		return (result);
	}
	else
		return (ft_strdup(node->key));
}

static char	**build_envp_from_list(t_env *env_list)
{
	t_env	*cur;
	char	**new_envp;
	int		env_count;
	int		i;

	cur = env_list;
	env_count = 0;
	while (cur)
	{
		env_count++;
		cur = cur->next;
	}
	new_envp = malloc(sizeof(char *) * (env_count + 1));
	if (!new_envp)
		return (NULL);
	cur = env_list;
	i = 0;
	while (cur)
	{
		new_envp[i] = make_env_string(cur);
		cur = cur->next;
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

static void	free_envp_array(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

static void	exec_error_exit(char *full_path, char **new_envp)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(full_path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	free_envp_array(new_envp);
	exit(126);
}

static void	child_process(char *full_path, char **argv, t_env *env_list)
{
	char	**new_envp;

	set_env_value(&env_list, "_", full_path);
	new_envp = build_envp_from_list(env_list);
	if (!new_envp)
		exit(1);
	execve(full_path, argv, new_envp);
	exec_error_exit(full_path, new_envp);
}

static int	spawn_external(char *full_path, char **argv, char **envp, t_env *env_list)
{
	pid_t	pid;
	int		status;

	(void)envp;
	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("minishell: fork failed\n", 2);
		return (-1);
	}
	if (pid == 0)
		child_process(full_path, argv, env_list);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (-1);
}

static char **split_path_from_env(t_env *env_list)
{
	char *path = get_env_value(env_list, "PATH");
	if (path == NULL)
	{
		/* When PATH is unset, bash still resolves bare names using a default
		 * that effectively includes the current directory. Use CWD + common paths. */
		path = ".:/bin:/usr/bin";
		return ft_split(path, ':');
	}
	if (path[0] == '\0')
	{
		/* PATH explicitly set to empty: no lookup */
		return NULL;
	}
	return ft_split(path, ':');
}

static int	exec_absolute_path(char **argv, char **envp, t_env *env_list)
{
	if (access(argv[0], F_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (127);
	}
	if (access(argv[0], X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (126);
	}
	return (spawn_external(argv[0], argv, envp, env_list));
}

static void	print_cmd_error(char *cmd, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
}

static int	check_special_dirs(char *cmd)
{
	if (ft_strcmp(cmd, ".") == 0 || ft_strcmp(cmd, "..") == 0)
	{
		print_cmd_error(cmd, "command not found");
		return (1);
	}
	return (0);
}

static int	exec_from_path(char **argv, char **envp, t_env *env_list)
{
	char	*full_path;
	int		ret;

	if (check_special_dirs(argv[0]))
		return (127);
	full_path = find_command_path(argv[0], env_list);
	if (!full_path)
	{
		print_cmd_error(argv[0], "command not found");
		return (127);
	}
	if (access(full_path, X_OK) != 0)
	{
		print_cmd_error(argv[0], "Permission denied");
		free(full_path);
		return (126);
	}
	ret = spawn_external(full_path, argv, envp, env_list);
	free(full_path);
	return (ret);
}

int	exec_external(char **argv, char **envp, t_env **env_list)
{
	if (ft_strchr(argv[0], '/'))
		return (exec_absolute_path(argv, envp, *env_list));
	else
		return (exec_from_path(argv, envp, *env_list));
}

/**
 * run_command - dispatch a parsed command (builtin or external)
 * Uses the same behavior as the previous main.c implementation.
 */
int	run_command(t_cmd *cmd, t_env **env_list, char **envp)
{
	if (!cmd || !cmd->argv)
		return (0);
	if (!cmd->argv[0])
		return (0);
	if (cmd->argv[0][0] == '\0')
	{
		ft_putstr_fd("minishell: : command not found\n", 2);
		return (127);
	}
	if (ft_strcmp(cmd->argv[0], "env") == 0 && cmd->argv[1] != NULL)
	{
		return (exec_external(cmd->argv, envp, env_list));
	}
	if (is_builtin(cmd->argv[0]))
	{
		if (ft_strcmp(cmd->argv[0], "exit") == 0)
			return (ft_exit(cmd->argv));
		return (exec_builtin(cmd->argv, env_list));
	}
	return (exec_external(cmd->argv, envp, env_list));
}

/*
 * Redirection helpers and single-command execution
 * (moved from main.c to centralize exec logic)
 */

static int	handle_heredoc_input(t_cmd *cmd, t_env *env_list)
{
	int	fd_in;

	if (cmd->heredoc_fd >= 0)
		fd_in = cmd->heredoc_fd;
	else
		fd_in = read_heredoc(cmd->infile, env_list, cmd->heredoc_quoted);
	if (fd_in < 0)
	{
		g_last_status = 1;
		return (-2);
	}
	return (fd_in);
}

static int	handle_file_input(char *infile)
{
	int	fd_in;

	fd_in = open(infile, O_RDONLY);
	if (fd_in < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(infile);
		g_last_status = 1;
		return (-2);
	}
	return (fd_in);
}

static int	handle_input_redir(t_cmd *cmd, t_env **env_list)
{
	int	fd_in;
	int	saved_stdin;

	if (!cmd->infile && !cmd->heredoc)
		return (-1);
	if (cmd->heredoc)
		fd_in = handle_heredoc_input(cmd, *env_list);
	else
		fd_in = handle_file_input(cmd->infile);
	if (fd_in < 0)
		return (fd_in);
	saved_stdin = dup(STDIN_FILENO);
	dup2(fd_in, STDIN_FILENO);
	close(fd_in);
	return (saved_stdin);
}

static int	handle_output_redir(t_cmd *cmd)
{
	int	fd_out;
	int	saved_stdout;

	if (!cmd->outfile)
		return (-1);
	if (cmd->append)
		fd_out = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd_out = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->outfile);
		g_last_status = 1;
		return (-2);
	}
	saved_stdout = dup(STDOUT_FILENO);
	dup2(fd_out, STDOUT_FILENO);
	close(fd_out);
	return (saved_stdout);
}

static void	restore_fds(int saved_stdin, int saved_stdout)
{
	if (saved_stdin >= 0)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout >= 0)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}

// No longer used; redirections are handled by legacy functions

static int	check_redir_errors(t_cmd *cmd)
{
	if (cmd->has_in_redir_error && cmd->in_redir_first_error)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->in_redir_first_error);
		g_last_status = 1;
		return (1);
	}
	if (cmd->has_out_redir_error && cmd->out_redir_first_error)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->out_redir_first_error);
		g_last_status = 1;
		return (1);
	}
	return (0);
}

static int	setup_redirections(t_cmd *cmd, t_env **env_list,
				int *saved_in, int *saved_out)
{
	*saved_in = handle_input_redir(cmd, env_list);
	if (*saved_in == -2)
		return (-1);
	*saved_out = handle_output_redir(cmd);
	if (*saved_out == -2)
	{
		restore_fds(*saved_in, -1);
		return (-1);
	}
	return (0);
}

static int	setup_stderr_redir(t_cmd *cmd)
{
	int	saved_stderr;

	saved_stderr = -1;
	if (cmd->redirect_stderr_to_out)
	{
		saved_stderr = dup(STDERR_FILENO);
		dup2(STDOUT_FILENO, STDERR_FILENO);
	}
	return (saved_stderr);
}

static void	update_underscore_var(t_cmd *cmd, t_env **env_list)
{
	int	i;

	if (!is_builtin(cmd->argv[0]))
	{
		i = 0;
		while (cmd->argv[i])
			i++;
		if (i > 0)
			set_env_value(env_list, "_", cmd->argv[i - 1]);
	}
}

static void	execute_single_cmd(t_cmd *cmd, t_env **env_list, char **envp)
{
	int	saved_stdin;
	int	saved_stdout;
	int	saved_stderr;

	if (check_redir_errors(cmd))
		return ;
	if (setup_redirections(cmd, env_list, &saved_stdin, &saved_stdout) == -1)
		return ;
	saved_stderr = setup_stderr_redir(cmd);
	if (cmd->argv && cmd->argv[0])
	{
		g_last_status = run_command(cmd, env_list, envp);
		update_underscore_var(cmd, env_list);
	}
	else
		g_last_status = 0;
	if (saved_stderr >= 0)
	{
		dup2(saved_stderr, STDERR_FILENO);
		close(saved_stderr);
	}
	restore_fds(saved_stdin, saved_stdout);
}

static void	execute_pipeline(t_cmd *cmd_list,
	t_env **env_list, char **envp, int n_cmds);

void	exec_cmd_list(t_cmd *cmd_list, t_env **env_list, char **envp)
{
	t_cmd	*current;

	current = cmd_list;
	if (count_cmds(cmd_list) == 1)
	{
		execute_single_cmd(current, env_list, envp);
		return ;
	}
	execute_pipeline(cmd_list, env_list, envp, count_cmds(cmd_list));
	return ;
}

char	*join_path_cmd(char *path, char *cmd)
{
	char	*tmp;
	char	*full_path;

	tmp = ft_strjoin(path, "/");
	if (!tmp)
		return (NULL);
	full_path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full_path);
}

static char	*test_path(char *path, char *cmd)
{
	char	*full_path;

	full_path = join_path_cmd(path, cmd);
	if (!full_path)
		return (NULL);
	/* Return the first existing candidate; caller will check X_OK */
	if (access(full_path, F_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

char	*find_command_path(char *cmd, t_env *env_list)
{
	char	**paths;
	char	*full_path;
	int		i;

	paths = split_path_from_env(env_list);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = test_path(paths[i], cmd);
		if (full_path)
		{
			free_split(paths);
			return (full_path);
		}
		i++;
	}
	free_split(paths);
	return (NULL);
}

static void	setup_child_pipes(int pipes[][2], int i, int n_cmds)
{
	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < n_cmds - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
	close_pipes(pipes, n_cmds - 1);
}

static int	handle_child_redirs(t_cmd *cmd, t_env **env_list)
{
	int	saved_in;
	int	saved_out;

	if (cmd->has_in_redir_error && cmd->in_redir_first_error)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->in_redir_first_error);
		return (-1);
	}
	if (cmd->has_out_redir_error && cmd->out_redir_first_error)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->out_redir_first_error);
		return (-1);
	}
	saved_in = handle_input_redir(cmd, env_list);
	if (saved_in == -2)
		return (-1);
	saved_out = handle_output_redir(cmd);
	if (saved_out == -2)
		return (-1);
	return (0);
}

static void	execute_pipeline_child(t_cmd *cmd, t_env **env_list,
				char **envp, int pipes[][2], int i, int n_cmds)
{
	int	status;

	setup_child_pipes(pipes, i, n_cmds);
	if (handle_child_redirs(cmd, env_list) == -1)
		exit(1);
	if (cmd->argv && cmd->argv[0])
		status = run_command(cmd, env_list, envp);
	else
		status = 0;
	exit(status);
}

static void	wait_all_children(pid_t *pids, int n_cmds)
{
	int	i;
	int	status;

	i = 0;
	while (i < n_cmds)
	{
		waitpid(pids[i], &status, 0);
		if (i == n_cmds - 1 && WIFEXITED(status))
			g_last_status = WEXITSTATUS(status);
		i++;
	}
}

static int	fork_and_check(int pipes[][2], int n_cmds)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("minishell: fork failed\n", 2);
		g_last_status = 1;
		close_pipes(pipes, n_cmds - 1);
	}
	return (pid);
}

static void	execute_pipeline(t_cmd *cmd_list,
	t_env **env_list, char **envp, int n_cmds)
{
	int		pipes[n_cmds - 1][2];
	pid_t	pids[1024];
	int		i;
	t_cmd	*cmd;

	if (n_cmds - 1 > 0 && create_pipes(pipes, n_cmds) == -1)
		return ;
	cmd = cmd_list;
	i = 0;
	while (cmd && i < n_cmds)
	{
		pids[i] = fork_and_check(pipes, n_cmds);
		if (pids[i] < 0)
			return ;
		if (pids[i] == 0)
			execute_pipeline_child(cmd, env_list, envp, pipes, i, n_cmds);
		cmd = cmd->next;
		i++;
	}
	close_pipes(pipes, n_cmds - 1);
	wait_all_children(pids, n_cmds);
}
