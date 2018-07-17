NAME		= ft_script

CC			= gcc
CFLAGS		= -Wall -Werror -Wextra -ltermcap
INC_FLAGS	= -I libft/inc -I inc

# -g for lldb
DEBUG		= -g

# Cosmetics Section:
OK_COLOR	= \033[38;5;105;1m
OBJ_COLOR	= \033[38;5;208m
COM_COLOR	= \033[38;5;200m
LIB_COLOR	= \033[1;32m
NO_COLOR    = \033[m

COM_STRING	= "Compiling:"
# End Cosmetics.

LIBS		= libft/libft.a \

# Add header files here: (use full relative path to Makefile)
INCLUDES	= libft/inc/libft.h \
			  inc/ft_script.h \

# Add source files here: (files assumed to be in ./src folder)
FILES		= ft_script.c \
			  roles.c \
			  parse_flags.c

SRC			= $(addprefix src/, $(FILES))

# ---------------------------------------------------------------------------- #

all: $(NAME)

$(NAME): $(LIBS) $(SRC) $(INCLUDES)
	@printf "%b" "$(LIB_COLOR)Compiling: $(OK_COLOR)$@\n$(NO_COLOR)"
	@$(CC) $(DEBUG) $(CFLAGS) $(INC_FLAGS) -o $(NAME) $(SRC) $(LIBS)

$(LIBS):
	@make -C libft

clean:
	@printf "Cleaning up $(NAME)...\n"

fclean: clean
	@rm -f $(NAME)
	@make -C libft fclean

re: fclean all

norm:
	@printf "%b" "$(LIB_COLOR)$(NAME): Checking Norm...\n$(NO_COLOR)"
	@norminette $(SRC) $(INCLUDES)
	@make -C libft norm

.PHONY: clean fclean all re
