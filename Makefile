# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ple-stra <ple-stra@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/29 15:36:23 by ple-stra          #+#    #+#              #
#    Updated: 2022/08/18 08:47:13 by ple-stra         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= pipex

SRCS_DIR	= srcs
SRCS		= pipex.c\
 path.c processes.c\
 errors.c
SRCSBNS_DIR	= srcs_bonus
SRCSBNS		= pipex_bonus.c\
 path_bonus.c processes_bonus.c pipes_utils_bonus.c\
 errors_bonus.c
BUILD_DIR	= build
OBJ_DIR		= $(BUILD_DIR)/objs
OBJBNS_DIR	= $(BUILD_DIR)/objs_bonus
OBJ			= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
OBJBNS		= $(addprefix $(OBJBNS_DIR)/, $(SRCSBNS:.c=.o))
INC			= -I./includes -I./$(LIBFT_DIR)/includes -I./
INCBNS		= -I./includes_bonus -I./$(LIBFT_DIR)/includes -I./

LIBFT_DIR	= libft
LIBFT		= $(LIBFT_DIR)/build/libft.a
LIBFT_FLAGS	= -L$(LIBFT_DIR)/build -lft

CC			= cc
CFLAGS		= -Wall -Wextra
LFLAGS		= $(LIBFT_FLAGS)
ifneq (nWerror, $(filter nWerror,$(MAKECMDGOALS)))
	CFLAGS	+= -Werror
endif
ifeq (sanitize, $(filter sanitize,$(MAKECMDGOALS)))
	CFLAGS 	+= -fsanitize=address
endif
ifeq (debug, $(filter debug,$(MAKECMDGOALS)))
	CFLAGS	+= -D KDEBUG=1
endif

GIT_SUBM	= $(shell \
 sed -nE 's/path = +(.+)/\1\/.git/ p' .gitmodules | paste -s -)
RM			= rm -rf

all			: $(NAME)

bonus		: $(GIT_SUBM) $(LIBFT) $(OBJBNS)
			$(CC) $(CFLAGS) $(INCBNS) -o $(NAME) $(OBJBNS) $(LFLAGS)

$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@
$(OBJBNS_DIR)/%.o: $(SRCSBNS_DIR)/%.c
	@mkdir -p $(OBJBNS_DIR)
	$(CC) $(CFLAGS) $(INCBNS) -c $< -o $@

$(GIT_SUBM): %/.git: .gitmodules
	@git submodule init
	@git submodule update --remote $*

$(LIBFT)	:
ifeq (,$(wildcard $(LIBFT)))
			@echo "building libft..."
			@$(MAKE) -sC $(LIBFT_DIR) all
endif

rmlibft		:
			@echo "deleting libft build..."
			@$(MAKE) -sC $(LIBFT_DIR) fclean

$(NAME)		: $(GIT_SUBM) $(LIBFT) $(OBJ)
			$(CC) $(CFLAGS) $(INC) -o $(NAME) $(OBJ) $(LFLAGS)
			
clean		:
			$(RM) $(OBJ_DIR)
			$(RM) $(OBJBNS_DIR)

fclean		:
			$(RM) $(BUILD_DIR)
			$(RM) $(NAME)

fcleanall	: rmlibft
			$(RM) $(BUILD_DIR)
			$(RM) $(NAME)

re			: fclean all
rebonus		: fclean bonus

nWerror		:
			@echo "WARN: Compiling without Werror flag!"
sanitize	:
			@echo "WARN: Compiling with fsanitize flag!"
debug:
			@echo "WARN: debug is enabled"

.PHONY: \
 all bonus clean fclean fcleanall re rebonus rmlibft\
 nWerror sanitize debug