/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat_free_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 00:00:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/03 19:57:28 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*handle_null_s1(char c)
{
	char	*result;

	result = ft_calloc(2, 1);
	if (!result)
		return (NULL);
	result[0] = c;
	return (result);
}

static void	copy_and_append(char *result, char *s1, size_t len, char c)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		result[i] = s1[i];
		i++;
	}
	result[i] = c;
}

char	*ft_strncat_free(char *s1, char c, int free_s1)
{
	char	*result;
	size_t	len;

	if (!s1)
		return (handle_null_s1(c));
	len = ft_strlen(s1);
	result = ft_calloc(len + 2, 1);
	if (!result)
		return (NULL);
	copy_and_append(result, s1, len, c);
	if (free_s1)
		free(s1);
	return (result);
}
