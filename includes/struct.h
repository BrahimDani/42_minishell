/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:02:10 by kadrouin          #+#    #+#             */
/*   Updated: 2025/11/30 18:04:20 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

typedef struct s_env
{
    char        *key;
    char        *value;
    struct s_env *next;
}               t_env;

// Forward declaration to allow exec.h to reference t_cmd
typedef struct s_cmd t_cmd;

#endif