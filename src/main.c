#include "minishell.h"

int g_exit_status = 0;

static char **copy_env(char **env)
{
    int count;
    int i;
    char    **new_env;

    count = 0;
    i = 0;
    while(env[count])
        count++;
    new_env = malloc(sizeof(char *) * (count + 1));
    if (!new_env)
        return (NULL);
    while(i < count)
    {
        new_env[i] = strdup(env[i]);
        i++;
    }
    new_env[count] = NULL;
    return (new_env);
}

void sigint_handler(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
    g_exit_status = 130; // Ctrl-C exit code
}

void setup_signals(void)
{
    struct sigaction sa;

    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);

    signal(SIGQUIT, SIG_IGN); // Ctrl-\ yok say
}


int main(int ac, char **av, char **env)
{
    char    *line;
    t_token *tokens;
    t_command *cmd;
    char    **envp;

    envp = copy_env(env);
    (void)ac;
    (void)av;
    line = NULL;
    setup_signals();
    while (1)
    {
        line = readline("minishell$ ");
        if (!line) // ctrl+D
            break;
        if (line[0])
            add_history(line);
        tokens = tokenize(line);
        if (!tokens)
            continue;
        cmd = parse_command(tokens, envp);
        if (cmd && cmd->next)
            execute_pipeline(cmd, envp);
        else
            execute_command(cmd, &envp);
        free_command(cmd);
        free_token(tokens);
        free(line);
    }
    rl_clear_history();
    return (0);
}
