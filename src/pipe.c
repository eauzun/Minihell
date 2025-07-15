#include "minishell.h"

static void handle_heredoc_input(t_command *cmd)
{
    char *tmp;

    if (cmd->heredoc && cmd->infile)
    {
        tmp = heredoc_input(cmd->infile);
        free(cmd->infile);
        cmd->infile = tmp;
    }
}
static void setup_stdio_redirects(t_command *cmd, int fd_in, int fd_out)
{
    int in, out, flags;

    if (cmd->infile)
    {
        in = open(cmd->infile, O_RDONLY);
        dup2(in, STDIN_FILENO);
        close(in);
    }
    else if (fd_in != -1)
        dup2(fd_in, STDIN_FILENO);
    if (cmd->outfile)
    {
        flags = O_WRONLY | O_CREAT;
        if (cmd->append)
            flags |= O_APPEND;
        else
            flags |= O_TRUNC;
        out = open(cmd->outfile, flags, 0644);
        dup2(out, STDOUT_FILENO);
        close(out);
    }
    else if (fd_out != -1)
        dup2(fd_out, STDOUT_FILENO);
}
static void child_pipe_process(t_command *cmd, char **envp, int fd_in, int fd_out)
{
    char *path;

    handle_heredoc_input(cmd);
    setup_stdio_redirects(cmd, fd_in, fd_out);
    path = find_executable(cmd->args[0]);
    if (!path)
        exit(127);
    execve(path, cmd->args, envp);
    perror("execve");
    exit(1);
}
static void parent_pipe_update(int *in, int *fd, t_command *cmd, pid_t pid)
{
    int status;

    if (*in != -1)
        close(*in);
    if (cmd->next)
        close(fd[1]);
    if(cmd->next)
        *in = fd[0];
    else
        *in = -1;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        g_exit_status = WEXITSTATUS(status);
    else
        g_exit_status = 1;
}
static void    execute_single_command(t_command *cmd, char **envp, int in, int *fd)
{
    pid_t   pid;
    
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return ;
    }
    if (pid == 0)
    {
        if(fd)
            child_pipe_process(cmd, envp, in, fd[1]);
        else
            child_pipe_process(cmd, envp, in, -1);
        exit(EXIT_FAILURE);
    }
    else
        parent_pipe_update(&in, fd, cmd, pid);
}

void execute_pipeline(t_command *cmd, char **envp)
{
    int fd[2];
    int in;

    in = -1;
    while (cmd)
    {
        if(cmd->next && pipe(fd) < 0)
        {
            perror("pipe");
            return ;
        }
        if (cmd->next)
            execute_single_command(cmd, envp, in, fd);
        else
            execute_single_command(cmd, envp, in, NULL);
        if (cmd->next)
        {
            close(fd[1]);
            if (in != -1)
                close(in);
            in = fd[0];
        }
        cmd = cmd->next;
    }
}
