# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/21 10:30:00 by kadrouin          #+#    #+#              #
<<<<<<< HEAD
#    Updated: 2025/08/27 18:30:47 by brdany           ###   ########.fr        #
=======
#    Updated: 2025/08/26 13:57:08 by kadrouin         ###   ########.fr        #
>>>>>>> 110107bc367a5fa24fa98fad3af87178e6cd444f
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
