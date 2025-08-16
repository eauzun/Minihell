NAME = minishell
DEBUG_NAME = debug_minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LIBS = -lreadline
RM = rm -f


SRC = src/main.c src/lexer.c src/signals.c src/libft_utils.c src/libft_utils2.c src/libft_itoa.c \
      src/lexer_special.c src/lexer_utils.c \
      src/parser.c src/parser_utils.c src/parser_redir.c src/parser_valid.c \
      src/expander.c src/expander_utils.c src/expander_variables.c src/expander_quote.c \
      src/builtin.c src/builtin_echo.c src/builtin_pwd.c src/builtin_env.c \
      src/builtin_exit.c src/builtin_export.c src/builtin_export_utils.c \
      src/builtin_export_utils2.c src/builtin_export_utils3.c

DEBUG_SRC = src/main_debug.c src/lexer.c src/signals.c src/libft_utils.c src/libft_utils2.c src/libft_itoa.c \
            src/lexer_special.c src/lexer_utils.c \
            src/parser.c src/parser_utils.c src/parser_redir.c src/parser_valid.c \
            src/expander.c src/expander_utils.c src/expander_variables.c src/expander_quote.c \
            src/debug.c src/builtin.c src/builtin_echo.c src/builtin_pwd.c src/builtin_env.c \
            src/builtin_exit.c src/builtin_export.c src/builtin_export_utils.c \
            src/builtin_export_utils2.c src/builtin_export_utils3.c

OBJ = $(SRC:.c=.o)
DEBUG_OBJ = $(DEBUG_SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBS)

debug: $(DEBUG_NAME)

$(DEBUG_NAME): $(DEBUG_OBJ)
	$(CC) $(CFLAGS) -o $(DEBUG_NAME) $(DEBUG_OBJ) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) $(DEBUG_OBJ)

fclean: clean
	$(RM) $(NAME) $(DEBUG_NAME)

re: fclean all

.PHONY: all debug clean fclean re