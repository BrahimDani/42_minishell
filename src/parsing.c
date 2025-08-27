#include "../includes/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void    print_tokens(char **tokens)
{
	int i;

	i = 0;
	if (!tokens)
		return ;
	while(tokens[i])
	{
		printf("token[%d] = %s\n", i, tokens[i]);
		i++;
	}
}

char **parse_line(char *line)
{
	if (!line | !*line)
		return (NULL);
	return (ft_split(line, ' '));
}

void	free_token(char **tokens)
{
	int i;

	i = 0;
	if (!tokens)
		return ;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}