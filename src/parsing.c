#include "../includes/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void    print_tokens(char **tokens)
{
    int i;

    i = 0;
    while(tokens[i])
    {
        printf("token[%d] = %s\n", i, tokens);
        i++;
    }
}

char    *parse_line(char *line)
{
    return (ft_split(line, ' '));
}

int main(int ac, char **av)
{
    char    *line;
    char    **tokens;

    line = "ls -l /home";
    tokens = parse_line(line);
    print_tokens(tokens);
    return (0);
}