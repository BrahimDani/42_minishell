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

/**
 * spawn_external - fork and execve an external program
 * Returns: child's exit status, or -1 on fork error
 */

static int	spawn_external(char *full_path, char **argv, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("minishell: fork failed\n", 2);
		return (-1);
	}
	if (pid == 0)
	{
		execve(full_path, argv, envp);
		ft_putstr_fd("minishell: execve failed\n", 2);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (-1);
}

int		exec_external(char **argv, char **envp)
{
	char	*full_path;
	int		ret;

	// Si c'est un chemin absolu ou relatif (contient /), l'utiliser directement
	if (ft_strchr(argv[0], '/'))
	{
		if (access(argv[0], X_OK) == 0)
			return (spawn_external(argv[0], argv, envp));
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return (127);
	}
	
	// Sinon chercher dans PATH
	full_path = find_command_path(argv[0]);
	if (!full_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return (127);
	}
	ret = spawn_external(full_path, argv, envp);
	free(full_path);
	return (ret);
}

/**
 * run_command - dispatch a parsed command (builtin or external)
 * Uses the same behavior as the previous main.c implementation.
 */
int	run_command(t_cmd *cmd, t_env **env_list, char **envp)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	if (is_builtin(cmd->argv[0]))
	{
		if (ft_strcmp(cmd->argv[0], "exit") == 0)
			return (ft_exit(cmd->argv));
		return (exec_builtin(cmd->argv, env_list));
	}
	return (exec_external(cmd->argv, envp));
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
		fd_in = read_heredoc(cmd->infile, *env_list);
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

static void	execute_single_cmd(t_cmd *cmd, t_env **env_list, char **envp)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = handle_input_redir(cmd, env_list);
	if (saved_stdin == -2)
		return ;
	saved_stdout = handle_output_redir(cmd);
	if (saved_stdout == -2)
	{
		restore_fds(saved_stdin, -1);
		return ;
	}
	if (cmd->argv && cmd->argv[0])
		g_last_status = run_command(cmd, env_list, envp);
	else
		g_last_status = 0;
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
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

char	*find_command_path(char *cmd)
{
	char	**paths;
	char	*full_path;
	int		i;

	paths = get_path_env("PATH");
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
