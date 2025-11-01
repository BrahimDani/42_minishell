/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 09:09:40 by brdani            #+#    #+#             */
/*   Updated: 2025/11/01 10:53:19 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*join;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	i = ft_strlen(s1);
	j = ft_strlen(s2);
	join = ft_calloc(1, i + j + 1);
	if (!join)
		return (NULL);
	ft_strlcpy(join, s1, i + 1);
	ft_strlcat(join, s2, i + j + 1);
	return (join);
}


char	*ft_strjoin_free(char *s1, char *s2, int free_flags)
{
	char	*result;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	result = ft_strjoin(s1, s2);
	if (free_flags & 1)
		free(s1);
	if (free_flags & 2)
		free(s2);
	return (result);
}


