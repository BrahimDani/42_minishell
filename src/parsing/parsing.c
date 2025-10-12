/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboxuser <vboxuser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:03:26 by kadrouin          #+#    #+#             */
/*   Updated: 2025/10/12 14:46:36 by vboxuser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

char	**parse_line(char *line)
{
	if (!line || !*line)
		return (NULL);
	if (empty_line(line))
		return (NULL);
	if (!check_quote(line) || !check_redir(line) || !check_ampersand (line))
	{
		g_last_status = 2;
		return (NULL);
	}
	if (!tokenize_line(line))
		return (NULL);
	return (ft_split(line, ' '));
}
