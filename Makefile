NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LIBS = -lreadline
RM = rm -f
SRC = src/main.c src/lexer.c src/signals.c src/libft_utils.c src/lexer_special.c src/lexer_utils.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re