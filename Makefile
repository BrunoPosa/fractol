# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/05 18:31:30 by bposa             #+#    #+#              #
#    Updated: 2024/05/19 19:14:54 by bposa            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= fractol

SRCS	=	graphics.c

CC		= cc

CFLAGS	= -Wall -Wextra -Werror

LIBDIR	= MLX42/

LIBS	= $(LIBDIR)build/libmlx42.a -ldl -lglfw -pthread -lm -L "$$HOME/.brew/opt/glfw/lib/"

HEADERS	= -I $(LIBDIR)include

# OBJCS	= $(SRCS:.c=.o)

all:	$(LIBDIR)build/libmlx42.a $(NAME)

$(NAME): $(SRCS)
	$(CC) $(CFLAGS) -O3 $(SRCS) $(LIBS) $(HEADERS) -o $(NAME)

$(LIBDIR)build/libmlx42.a:
	cmake $(LIBDIR) -DDEBUG=1 -B $(LIBDIR)build && make -C $(LIBDIR)build -j4
# Use w/o DEBUG=1 flag for final
# cmake $(LIBDIR) -B $(LIBDIR)build && make -C $(LIBDIR)build -j4

# %.o: %.c
# 	$(CC) $(CFLAGS) -Imlx -c $< -o $@
	
clean:

fclean: clean
	rm -f $(NAME)
	rm -f $(LIBDIR)/build/libmlx42.a

re: fclean all

.PHONY: all re clean fclean



# ===========================================================

# LIBMLX	:= ./lib/MLX42

# HEADERS	:= -I ./include -I $(LIBMLX)/include
# LIBS	:= $(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm
# OBJS	:= ${SRCS:.c=.o}

# all: libmlx $(NAME)

# libmlx:
# 	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

# %.o: %.c
# 	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS) && printf "Compiling: $(notdir $<)"

# $(NAME): $(OBJS)
# 	@$(CC) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)

# clean:
# 	@rm -rf $(OBJS)
# 	@rm -rf $(LIBMLX)/build

# fclean: clean
# 	@rm -rf $(NAME)

# re: clean all

# .PHONY: all, clean, fclean, re, libmlx