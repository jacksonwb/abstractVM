# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/03/03 12:06:08 by jbeall            #+#    #+#              #
#    Updated: 2019/05/25 20:52:14 by jbeall           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#==================================== GENERAL =================================#

NAME = abstractVM

CPPFLAGS = -Wall -Wextra -Werror -std=c++11 #-Ofast #-fsanitize=address

SRC_DIR = ./src/

#=================================== SOURCES ==================================#

LIST = main \
AVM \
Lexer \
Operand

SRC = $(addsuffix .cpp, $(addprefix $(SRC_DIR), $(LIST)))

#=================================== OBJECTS ==================================#

OBJ_DIR = .obj/
OBJ = $(addsuffix .o, $(addprefix $(OBJ_DIR), $(LIST)))
DEP = $(OBJ:%.o=%.d)

#=================================== HEADERS ==================================#

#INC_DIR = ./src/

#INC = -I $(INC_DIR)

#==================================== COLORS ==================================#

#COLORS
COM_COLOR   = \033[92m
NO_COLOR    = \033[m

#TEXT
COM_STRING  = "$(NAME) compilation successful"
CLEAN_OBJ	= "cleaned $(NAME) objects"
CLEAN_NAME	= "cleaned $(NAME) binary"

#===================================== RULES ==================================#

all: obj $(NAME)

$(NAME): $(OBJ)
	@echo "linking..."
	@clang++ $(CPPFLAGS) $^ -o $@
	@echo "$(COM_COLOR) $(COM_STRING) $(NO_COLOR)"

-include $(DEP)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@printf "compiling: %s\n" $<
	@clang++ $(CPPFLAGS) $(INC) -MMD -g -c $< -o $@

obj:
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(COM_COLOR)$(CLEAN_OBJ)$(NO_COLOR)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(COM_COLOR)$(CLEAN_NAME)$(NO_COLOR)"

re: fclean all
