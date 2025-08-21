/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brdani <brdani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 22:38:37 by brdani            #+#    #+#             */
/*   Updated: 2024/10/19 14:38:43 by brdani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *hystk, const char *ndl, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (ndl[0] == '\0')
		return ((char *)hystk);
	while (hystk[i] != '\0' && i < len)
	{
		j = 0;
		if (hystk[i] == ndl[0])
		{
			while (hystk[i + j] == ndl[j] && (i + j) < len)
			{
				if (ndl[j + 1] == '\0')
					return ((char *)hystk + i);
				j++;
			}
		}
		i++;
	}
	return (NULL);
}
