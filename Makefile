# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/05 18:31:30 by bposa             #+#    #+#              #
#    Updated: 2024/05/27 17:56:44 by bposa            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= fractol

SRCS	=	fractol.c \
			validation.c \
			rendering.c \
			utils.c

CC		= cc

CFLAGS	= -Wall -Wextra -Werror

LIBDIR	= MLX42/

LIBS	= $(LIBDIR)build/libmlx42.a -ldl -lglfw -pthread -lm -L "$$HOME/.brew/opt/glfw/lib/"

HEADERS	= -I $(LIBDIR)include

all:	$(LIBDIR)build/libmlx42.a $(NAME)

$(NAME): $(SRCS) fractol.h
	$(CC) $(CFLAGS) -O3 $(SRCS) $(LIBS) $(HEADERS) -o $(NAME)

$(LIBDIR)build/libmlx42.a:
	cmake $(LIBDIR) -B $(LIBDIR)build && make -C $(LIBDIR)build -j4
	
clean:

fclean: clean
	rm -f $(NAME)
	rm -f $(LIBDIR)/build/libmlx42.a

re: fclean all

.PHONY: all re clean fclean