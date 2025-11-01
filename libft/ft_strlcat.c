/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 21:18:33 by brdani            #+#    #+#             */
/*   Updated: 2025/11/01 10:53:12 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (dst[i] != '\0' && i < size)
		i++;
	j = i;
	while (src[i - j] != '\0' && i < size - 1)
	{
		dst[i] = src[i - j];
		i++;
	}
	if (j < size)
		dst[i] = '\0';
	return (j + ft_strlen(src));
}

char	*ft_strncat_free(char *s1, char c, int free_s1)
{
	char	*result;
	size_t	len;
	size_t	i;

	if (!s1)
	{
		result = ft_calloc(2, 1);
		if (!result)
			return (NULL);
		result[0] = c;
		return (result);
	}
	len = ft_strlen(s1);
	result = ft_calloc(len + 2, 1);
	if (!result)
		return (NULL);
	i = 0;
	while (i < len)
	{
		result[i] = s1[i];
		i++;
	}
	result[i] = c;
	if (free_s1)
		free(s1);
	return (result);
}