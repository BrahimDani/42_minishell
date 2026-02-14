/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 15:51:00 by kadrouin          #+#    #+#             */
/*   Updated: 2026/02/14 15:51:00 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ms_status_get(t_shell *sh)
{
	if (!sh)
		return (0);
	return (sh->last_status);
}

void	ms_status_set(t_shell *sh, int status)
{
	if (!sh)
		return ;
	sh->last_status = status;
}
