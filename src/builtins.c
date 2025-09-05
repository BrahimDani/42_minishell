#include "builtins.h"
#include "exec.h"
#include "minishell.h"
#include "parsing.h"
#include "struct.h"
#include "utils.h"


// regarde si c'est un builtin ou non
int is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (strcmp(cmd, "echo") == 0)
		return (1);
	if (strcmp(cmd, "cd") == 0)
		return (1);
	if (strcmp(cmd, "pwd") == 0)
		return (1);
	if (strcmp(cmd, "export") == 0)
		return (1);
	if (strcmp(cmd, "unset") == 0)
		return (1);
	if (strcmp(cmd, "env") == 0)
		return (1);
	if (strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int exec_builtin(char **tokens, t_env **env_list)
{
	(void)env_list;
	// if (strcmp(tokens[0], "echo") == 0)
	// 	return (ft_echo(tokens));
	// else if (strcmp(tokens[0], "cd") == 0)
	// 	return (ft_cd(tokens, env_list));
	if (strcmp(tokens[0], "pwd") == 0)
		return (ft_pwd());
	// else if (strcmp(tokens[0], "export") == 0)
	// 	return(ft_export(tokens, env_list));
	// else if (strcmp(tokens[0], "unset") == 0)
	// 	return(ft_unset(tokens, env_list));
	// else if (strcmp(tokens[0], "env") == 0)
	// 	return(ft_env(tokens, env_list));
	// else if (strcmp(tokens[0], "exit") == 0)
	// 	return(ft_exit(tokens, env_list));
	return (1);
}

int	ft_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}