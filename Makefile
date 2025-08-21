# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: brdany <brdany@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/21 10:30:00 by kadrouin          #+#    #+#              #
<<<<<<< HEAD
#    Updated: 2025/08/21 13:18:02 by brdany           ###   ########.fr        #
=======
#    Updated: 2025/08/21 11:13:06 by kadrouin         ###   ########.fr        #
>>>>>>> 318c0928e3f3b3d3192d549663a0639c8f3bb7d2
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lreadline

SRCDIR = src
BUILTINSDIR = builtins
EXECDIR = exec
PARSINGDIR = parsing
INCDIR = includes
LIBFTDIR = libft
LIBFT = $(LIBFTDIR)/libft.a

SRCS = $(wildcard $(SRCDIR)/*.c) \
       $(wildcard $(BUILTINSDIR)/*.c) \
       $(wildcard $(EXECDIR)/*.c) \
       $(wildcard $(PARSINGDIR)/*.c)

OBJS = $(SRCS:.c=.o)

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(OBJS) -o $(NAME) $(LIBFT) $(LDFLAGS)

$(LIBFT):
	$(MAKE) -C $(LIBFTDIR)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) -I$(LIBFTDIR) -c $< -o $@

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFTDIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re
