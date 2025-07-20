#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>


extern int  g_exit_status;

typedef enum e_token_type
{
    T_WORD,
    T_PIPE,
    T_REDIR_IN,
    T_REDIR_OUT,
    T_REDIR_APPEND,
    T_HEREDOC,
}   t_token_type;

typedef struct s_token
{
    t_token_type	type;
    char            *str;
    struct s_token	*next;
}   t_token;

typedef struct s_command
{
    char                **args;      // komut ve argümanlar örn. ls, -l, NULL
    char                *infile;     // input redirection ör. in.txt
    char                *outfile;    // output redirection ör. out.txt
    int                 append;      // 1 => >>, 0 => >
    int                 heredoc;     // 1 => <<, 0 => <
    struct s_command    *next;       // pipe durumunda sonraki komut
}   t_command;

//signals
void	set_signals(void);
//lexer-tokenizing
t_token	*lexer_init(const char *line);
t_token	*single_quote(const char *line, size_t *i);
t_token	*double_quote(const char *line, size_t *i);
t_token	*redirections(const char *line, size_t *i);
t_token *is_pipe(const char *line, size_t *i);
t_token *token_word(const char *line, size_t *i);
void    free_token(t_token *list);
void    add_token(t_token **list, t_token *tok);
t_token	*new_token(t_token_type type, const char *start, size_t len);

//libft utils
size_t	ft_strlen(const char *s);

#endif