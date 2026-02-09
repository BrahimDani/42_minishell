/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 18:45:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/08 16:17:55 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static const char	*skip_ws_sign(const char *str, int *sign)
{
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			*sign = -1;
		str++;
	}
	return (str);
}

long long	ft_atoll(const char *str)
{
	unsigned long long	res;
	unsigned long long	limit;
	int					sign;
	int					digit;

	res = 0;
	sign = 1;
	str = skip_ws_sign(str, &sign);
	limit = (unsigned long long)LLONG_MAX + (sign < 0);
	while (*str >= '0' && *str <= '9')
	{
		digit = *str - '0';
		if (res > (limit - (unsigned long long)digit) / 10ULL)
			return (0);
		res = (res * 10ULL) + (unsigned long long)digit;
		str++;
	}
	if (sign < 0)
	{
		if (res == (unsigned long long)LLONG_MAX + 1ULL)
			return (LLONG_MIN);
		else if (res > (unsigned long long)LLONG_MAX + 1ULL)
			return (0);
	}
	return (sign * (long long)res);
}
