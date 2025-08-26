/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:03:26 by kadrouin          #+#    #+#             */
/*   Updated: 2025/08/26 14:03:52 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    print_tokens(char **tokens)
{
    int i;

    i = 0;
    while(tokens[i])
    {
        printf("token[%d] = %s\n", i, tokens[i]);
        i++;
    }
}

char    **parse_line(char *line)
{
    return (ft_split(line, ' '));
}
