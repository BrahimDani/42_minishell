/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:02:10 by kadrouin          #+#    #+#             */
/*   Updated: 2026/01/03 19:54:41 by kadrouin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

typedef enum e_signal_state
{
	SIG_DEFAULT = 0,
	SIG_IGNORE,
	SIG_STANDARD,
	SIG_RECORD
}				t_sig;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_shell
{
	int				last_status;
	int				cmd_mode;
	int				signal_record;
	char			**envp;
}					t_shell;

// Forward declaration to allow exec.h to reference t_cmd
typedef struct s_cmd	t_cmd;

#endif
