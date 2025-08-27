/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:07:23 by kadrouin          #+#    #+#             */
/*   Updated: 2025/08/26 14:07:23 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
#define PARSING_H

#include "../includes/minishell.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void	print_tokens(char **tokens);
char	**parse_line(char *line);
char	*find_command_path(char *command);

#endif
