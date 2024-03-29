################################################################################
### INIT
################################################################################

NAME			= ft_strace

SRC_PATH		= src

OBJ_PATH		= obj

INC_PATH		= inc

CC			= gcc
CFLAGS			= -Wall -Wextra -Werror -I $(INC_PATH) #-g3 -fsanitize=address

################################################################################
### OBJECTS
################################################################################

OBJ_NAME		= main.o						\
			  error.o						\
			  binary.o						\
			  filepath.o						\
			  trace.o						\
			  handler.o						\
			  systable_64.o						\
			  systable_32.o						\
			  signal.o						\

OBJ			= $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))			\

################################################################################
### RULES
################################################################################

.PHONY: all clean fclean re

all: $(OBJ_PATH) $(NAME)

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(OBJ_PATH)
	@rm -rf $(NAME)

re: fclean all
