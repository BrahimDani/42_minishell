/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_split_count.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 20:55:26 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/03 20:47:21 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/parsing.h"

int	count_leading_spaces(char *str)
{
	int	count;

	count = 0;
	while (str[count] == ' ')
		count++;
	return (count);
}

int	count_trailing_spaces(char *str, int len)
{
	int	count;

	count = 0;
	while (count < len && str[len - 1 - count] == ' ')
		count++;
	return (count);
}
