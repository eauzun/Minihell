NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LIBS = -lreadline
RM = rm -f
SRC = src/main.c src/lexer.c src/parser.c src/executor.c src/pipe.c src/builtin.c src/expander.c
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