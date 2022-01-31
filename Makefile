# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ple-stra <ple-stra@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/29 15:36:23 by ple-stra          #+#    #+#              #
#    Updated: 2022/01/31 01:31:09 by ple-stra         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= pipex

SRCS_DIR	= srcs
SRCS		= pipex.c
BUILD_DIR	= build
OBJ_DIR		= $(BUILD_DIR)/objs
OBJ			= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
INC			= -I./includes -I./libft/includes

LIBFT_DIR	= libft
LIBFT_A		= $(LIBFT_DIR)/build/libft.a

CC			= gcc

CFLAGS		= $(INC) -Wall -Wextra
ifneq (nWerror, $(filter nWerror,$(MAKECMDGOALS)))
	CFLAGS	+= -Werror
endif
ifeq (sanitize, $(filter sanitize,$(MAKECMDGOALS)))
	CFLAGS 	+= -fsanitize=address
endif

RM			= rm -rf

all			: $(NAME)

bonus		: all

$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_A)	: check_submo
ifeq (,$(wildcard $(LIBFT_A)))
			@echo "building libft..."
			@$(MAKE) -sC $(LIBFT_DIR) all
endif

rmlibft		:
			@echo "deleting libft build..."
			@$(MAKE) -sC $(LIBFT_DIR) fclean

$(NAME)		: $(LIBFT_A) $(OBJ)
			$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
			
clean		:
			$(RM) $(OBJ_DIR)

fclean		:
			$(RM) $(BUILD_DIR)
			$(RM) $(NAME)


fcleanall	: rmlibft
			$(RM) $(BUILD_DIR)
			$(RM) $(NAME)

re			: fclean all

reall		: fcleanall all

check_submo	:
			@if git submodule status $(LIBFT_DIR) | egrep -q '^[-]'; then \
				echo "Initializing git module $(LIBFT_DIR)..."; \
				git submodule init; \
				git submodule update; \
			fi

nWerror		:
			@echo "WARN: Compiling without Werror flag!"
sanitize	:
			@echo "WARN: Compiling with fsanitize flag!"

.PHONY: \
 all clean fclean fcleanall re reall rmlibft check_submo\
 nWerror sanitize