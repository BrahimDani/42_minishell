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

/**
 * spawn_external - fork and execve an external program
 * Returns: child's exit status, or -1 on fork error
 */

static int	spawn_external(char *full_path, char **argv, char **envp, t_env *env_list)
{
	pid_t	pid;
	int		status;
	char	**new_envp;
	int		i;
	int		env_count;
	(void)envp;

	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("minishell: fork failed\n", 2);
		return (-1);
	}
	if (pid == 0)
	{
		// Ensure _ reflects the command path for the child
		set_env_value(&env_list, "_", full_path);
		// Build envp from env_list to include runtime changes (SHLVL, etc)
		t_env *cur = env_list;
		env_count = 0;
		while (cur)
		{
			env_count++;
			cur = cur->next;
		}
		new_envp = malloc(sizeof(char *) * (env_count + 1));
		if (!new_envp)
			exit(1);
		cur = env_list;
		i = 0;
		while (cur)
		{
			if (cur->value)
			{
				char *tmp = ft_strjoin(cur->key, "=");
				new_envp[i] = ft_strjoin(tmp, cur->value);
				free(tmp);
			}
			else
				new_envp[i] = ft_strdup(cur->key);
			cur = cur->next;
			i++;
		}
		new_envp[i] = NULL;
		execve(full_path, argv, new_envp);
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(full_path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		/* Free new_envp on exec failure to avoid leaks in child */
		i = 0;
		while (new_envp && new_envp[i])
		{
			free(new_envp[i]);
			i++;
		}
		free(new_envp);
		exit(126);
	}
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

int		exec_external(char **argv, char **envp, t_env **env_list)
{
	char	*full_path;
	int		ret;
	(void)env_list;

	// Si c'est un chemin absolu ou relatif (contient /), l'utiliser directement
	if (ft_strchr(argv[0], '/'))
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
		return (spawn_external(argv[0], argv, envp, *env_list));
	}
	
	// Sinon chercher dans PATH
	if (ft_strcmp(argv[0], ".") == 0 || ft_strcmp(argv[0], "..") == 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return (127);
	}
	full_path = find_command_path(argv[0], *env_list);
	if (!full_path)
	{
		/* Not found in PATH search */
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return (127);
	}
	/* Found in PATH but might not be executable: map to 126 if needed */
	if (access(full_path, X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
		free(full_path);
		return (126);
	}
	ret = spawn_external(full_path, argv, envp, *env_list);
	free(full_path);
	return (ret);
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

static int	handle_input_redir(t_cmd *cmd, t_env **env_list)
{
	int	fd_in;
	int	saved_stdin;

	if (!cmd->infile && !cmd->heredoc)
		return (-1);
	if (cmd->heredoc)
	{
		if (cmd->heredoc_fd >= 0)
			fd_in = cmd->heredoc_fd;
		else
			fd_in = read_heredoc(cmd->infile, *env_list, cmd->heredoc_quoted);
		if (fd_in < 0)
		{
			g_last_status = 1;
			return (-2);
		}
	}
	else
	{
		fd_in = open(cmd->infile, O_RDONLY);
		if (fd_in < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(cmd->infile);
			g_last_status = 1;
			return (-2);
		}
	}
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

static void	execute_single_cmd(t_cmd *cmd, t_env **env_list, char **envp)
{
	int	saved_stdin;
	int	saved_stdout;
	int	saved_stderr;

	if (cmd->has_in_redir_error && cmd->in_redir_first_error)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->in_redir_first_error);
		g_last_status = 1;
		return ;
	}
	if (cmd->has_out_redir_error && cmd->out_redir_first_error)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->out_redir_first_error);
		g_last_status = 1;
		return ;
	}
	saved_stdin = handle_input_redir(cmd, env_list);
	if (saved_stdin == -2)
		return ;
	saved_stdout = handle_output_redir(cmd);
	if (saved_stdout == -2)
	{
		restore_fds(saved_stdin, -1);
		return ;
	}
	saved_stderr = -1;
	if (cmd->redirect_stderr_to_out)
	{
		saved_stderr = dup(STDERR_FILENO);
		if (cmd->outfile)
			dup2(STDOUT_FILENO, STDERR_FILENO);
		else
			dup2(STDOUT_FILENO, STDERR_FILENO);
	}
	if (cmd->argv && cmd->argv[0])
	{
		g_last_status = run_command(cmd, env_list, envp);
		// Update _ with command path for external commands
		if (!is_builtin(cmd->argv[0]))
		{
			int i = 0;
			while (cmd->argv[i])
				i++;
			if (i > 0)
				set_env_value(env_list, "_", cmd->argv[i-1]);
		}
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

static void	execute_pipeline(t_cmd *cmd_list,
	t_env **env_list, char **envp, int n_cmds)
{
	int		pipes[n_cmds - 1][2];
	pid_t	pids[1024];
	int		i;
	t_cmd	*cmd;
	int		status;
	int		saved_in;
	int		saved_out;

	if (n_cmds - 1 > 0 && create_pipes(pipes, n_cmds) == -1)
		return ;
	cmd = cmd_list;
	i = 0;
	while (cmd && i < n_cmds)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			ft_putstr_fd("minishell: fork failed\n", 2);
			g_last_status = 1;
			close_pipes(pipes, n_cmds - 1);
			return ;
		}
		if (pids[i] == 0)
		{
			if (i > 0)
				dup2(pipes[i - 1][0], STDIN_FILENO);
			if (i < n_cmds - 1)
				dup2(pipes[i][1], STDOUT_FILENO);
			close_pipes(pipes, n_cmds - 1);
			if (cmd->has_in_redir_error && cmd->in_redir_first_error)
			{
				ft_putstr_fd("minishell: ", 2);
				perror(cmd->in_redir_first_error);
				exit(1);
			}
			if (cmd->has_out_redir_error && cmd->out_redir_first_error)
			{
				ft_putstr_fd("minishell: ", 2);
				perror(cmd->out_redir_first_error);
				exit(1);
			}
			saved_in = handle_input_redir(cmd, env_list);
			if (saved_in == -2)
				exit(1);
			saved_out = handle_output_redir(cmd);
			if (saved_out == -2)
				exit(1);
			if (cmd->argv && cmd->argv[0])
				status = run_command(cmd, env_list, envp);
			else
				status = 0;
			exit(status);
		}
		cmd = cmd->next;
		i++;
	}
	close_pipes(pipes, n_cmds - 1);
	i = 0;
	while (i < n_cmds)
	{
		waitpid(pids[i], &status, 0);
		if (i == n_cmds - 1 && WIFEXITED(status))
			g_last_status = WEXITSTATUS(status);
		i++;
	}
}
