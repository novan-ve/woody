NAME = woody_woodpacker
ASM_NAME = payload

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include
ASM_DIR = asm

SRC = encrypt.c \
	  file.c \
	  free.c \
	  main.c \
	  parse.c \
	  payload.c \
	  validate.c

ASM_SRC = payload.s

HEADER = woody.h

LIBFT = libft/libft.a

OBJ := $(SRC:%.c=./$(OBJ_DIR)/%.o)
SRC := $(SRC:%=./$(SRC_DIR)/%)

ASM_OBJ := $(ASM_SRC:%.s=./$(OBJ_DIR)/$(ASM_DIR)/%.o)
ASM_SRC := $(ASM_SRC:%=./$(ASM_DIR)/%)
ASM_NAME := $(ASM_NAME:%=./$(ASM_DIR)/%)

HEADER := $(HEADER:%=./$(INC_DIR)/%)

FLAGS = -Wall -Werror -Wextra -g

all: $(NAME) $(ASM_NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER) | $(OBJ_DIR)/$(ASM_DIR)
	$(CC) $(FLAGS) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(OBJ_DIR)/$(ASM_DIR)/%.o: $(ASM_DIR)/%.s
	nasm -f elf64 $< -o $@

$(NAME): $(OBJ)
	$(MAKE) -s -C libft
	$(CC) $^ -o $@ $(LIBFT)

$(ASM_NAME): $(ASM_OBJ)
	ld -o $@ $^

$(OBJ_DIR)/$(ASM_DIR):
	mkdir -p $@

clean:
	$(RM) $(OBJ) $(ASM_OBJ)
	$(MAKE) -C libft clean

fclean:
	$(RM) $(OBJ) $(ASM_OBJ) $(NAME) $(ASM_NAME)
	$(MAKE) -C libft fclean

re: fclean
	$(MAKE)
