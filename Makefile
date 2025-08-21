# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kadrouin <kadrouin@student.42angouleme.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/21 10:30:00 by kadrouin          #+#    #+#              #
#    Updated: 2025/08/21 10:37:48 by kadrouin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lreadline

SRCDIR = src
BUILTINSDIR = builtins
EXECDIR = exec
PARSINGDIR = parsing
INCDIR = includes


SRCS = $(wildcard $(SRCDIR)/*.c) \
       $(wildcard $(BUILTINSDIR)/*.c) \
       $(wildcard $(EXECDIR)/*.c) \
       $(wildcard $(PARSINGDIR)/*.c)

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
