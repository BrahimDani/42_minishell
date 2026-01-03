# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kadrouin <kadrouin@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/21 10:30:00 by kadrouin          #+#    #+#              #
#    Updated: 2026/01/03 19:30:30 by kadrouin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lreadline

SRCDIR = src
COREDIR = $(SRCDIR)/core
UTILSDIR = $(SRCDIR)/utils
BUILTINSDIR = $(SRCDIR)/builtins
EXECDIR = $(SRCDIR)/exec
PARSINGDIR = $(SRCDIR)/parsing
INCDIR = includes
LIBFTDIR = libft
LIBFT = $(LIBFTDIR)/libft.a

SRCS = $(wildcard $(COREDIR)/*.c) \
       $(wildcard $(UTILSDIR)/*.c) \
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
