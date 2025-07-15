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
# include <ctype.h>
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
    t_token_type    type;
    char            *str;
    struct s_token         *next;
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



void    add_token(t_token **list, t_token *tok);
void    free_token(t_token *list);
t_token *tokenize(const char *line);
t_command   *parse_command(t_token *token, char **envp);
void    free_command(t_command *cmd);
char    *find_executable(const char *cmd);
void    execute_command(t_command *cmd, char ***envp);
char *heredoc_input(const char *delimeter);
void execute_pipeline(t_command *cmd, char **envp);
int is_builtin(const char *cmd);
int execute_builtin(t_command *cmd, char ***envp);
int builtin_cd(char **args);
int builtin_pwd(void);
int builtin_echo(char   **args);
int builtin_exit(char **args);
int builtin_env(char **envp);
int builtin_export(char **args, char ***envp);
int builtin_unset(char **args, char ***envp);
void    free_str_array(char **arr);
char	*ft_itoa(int n);
char	*ft_strjoin(char const *s1, char const *s2);
char *expand_variables(const char *input, char **envp);
char *get_var_value(const char *name, char **envp);

#endif