#include "minishell.h"

static size_t  ft_strlen(const char *s)
{
    size_t  i;
    
    i = 0;
    while (s && s[i])
        i++;
    return (i);
}

static char    *ft_strdup(const char *s)
{
    size_t  len;
    size_t  i;
    char    *dup;

    i = 0;
    len = ft_strlen(s);
    dup = malloc(len + 1);
    if (!dup)
        return (NULL);
    while (i < len)
    {
        dup[i] = s[i];
        i++;
    }
    dup[len] = '\0';
    return (dup);
}
static char    *ft_strjoin_path(const char *dir, const char *cmd)
{
    size_t  dlen;
    size_t  clen;
    size_t  i;
    char    *result;

    i = 0;
    dlen = ft_strlen(dir);
    clen = ft_strlen(cmd);
    result = malloc(dlen + 1 + clen + 1);
    if (!result)
        return (NULL);
    while (*dir)
        result[i++] = *dir++;
    result[i++] = '/';
    while (*cmd)
        result[i++] = *cmd++;
    result[i] = '\0';    
    return (result);
}
static char    **ft_split(const char *s, char delim)
{
    int count = 1; 
    int i = 0; 
    int j = 0; 
    while (s[i])
        if (s[i++] == delim)
            count++;
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result)
        return (NULL);
    i = 0;
    while (s[i])
    {
        int start = i;
        while (s[i] && s[i] != delim)
            i++;
        result[j] = malloc(i - start + 1);
        if (!result[j])
            return (NULL);
        memcpy(result[j], &s[start], i - start);
        result[j][i - start] = '\0';
        if (s[i])
            i++;
        j++;
    }
    result[j] = NULL;
    return (result);
}
void    free_str_array(char **arr)
{
    int i;

    i = 0;
    if (!arr)
        return ;
    while (arr[i])
        free(arr[i++]);
    free(arr);
}
static char *check_paths_for_exec(char **paths, const char *cmd)
{
    int     i;
    char    *full_path;

    i = 0;
    while (paths[i])
    {
        full_path = ft_strjoin_path(paths[i], cmd);
        if (!full_path)
        {
            i++;
            continue ;
        }
        if (access(full_path, X_OK) == 0)
        {
            free_str_array(paths);
            return (full_path);
        }
        free(full_path);
        i++;
    }
    return (NULL);
}
char    *find_executable(const char *cmd)
{
    char    *path_env;
    char    **paths;
    char    *result;

    path_env = getenv("PATH");
    if (!path_env)
        return (NULL);
    paths = ft_split(path_env, ':');
    if (!paths)
        return (NULL);
    result = check_paths_for_exec(paths, cmd);
    if (!result)
        free_str_array(paths);
    return (result);
}
// sadece input redirection (< infile)
static int  redirect_input(t_command *cmd)
{
    int fd;

    if (!cmd->infile)
        return (0);
    fd = open(cmd->infile, O_RDONLY);
    if (fd < 0)
    {
        perror("minishell: open infile");
        return (1);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
    return (0);
}
// sadece output redirection (> ya da >>)
static int  redirect_output(t_command *cmd)
{
    int fd;
    int flags;

    if (!cmd->outfile)
        return (0);
    flags = O_WRONLY | O_CREAT;
    if (cmd->append)
        flags |= O_APPEND;
    else
        flags |= O_TRUNC;
    fd = open(cmd->outfile, flags, 0644);
    if (fd < 0)
    {
        perror("minishell: open outfile");
        return (1);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return (0);
}
// sadece çağrıları yapar
static int  redirect_io(t_command *cmd)
{
    if (redirect_input(cmd))
        return (1);
    if (redirect_output(cmd))
        return (1);
    return (0);    
}
static void write_heredoc_lines(int fd, const char *delimeter)
{
    char    *line;

    while (1)
    {
        line = readline("> ");
        if (!line)
            break ;
        if (strcmp(line, delimeter) == 0)
        {
            free(line);
            break ;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
}
static int	total(long c)
{
	int	i;

	i = 0;
	if (c < 0)
	{
		i++;
		c *= -1;
	}
	if (c == 0)
		return (1);
	while (c != 0)
	{
		c = c / 10;
		i++;
	}
	return (i);
}
char	*ft_itoa(int n)
{
	char	*arr;
	size_t	i;
	long	num;

	i = total(n);
	num = (long)n;
	arr = malloc(sizeof(char) * (total(num) + 1));
	if (arr == NULL)
		return (NULL);
	if (num == 0)
		arr[0] = '0';
	arr[i] = '\0';
	while (num != 0)
	{
		if (num < 0)
		{
			arr[0] = '-';
			num = -num;
		}
		arr[--i] = (num % 10) + 48;
		num = num / 10;
	}
	return (arr);
}
char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*arr;
	size_t	i;
	size_t	j;
	size_t	len;

	i = 0;
	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	arr = malloc(sizeof(char) * len);
	if (arr == NULL)
		return (NULL);
	while (s1[i])
	{
		arr[i] = s1[i];
		i++;
	}
	j = i;
	i = 0;
	while (s2[i])
	{
		arr[j] = s2[i];
		j++;
		i++;
	}
	arr[j] = '\0';
	return (arr);
}
static char *heredoc_filename(void)
{
    static int count;
    char    *num;
    char    *tmp;
    char    *res;

    //count = 0;
    num = ft_itoa(count++);
    if(!num)
        return (NULL);
    tmp = ft_strjoin(".heredoc_tmp_", num);
    free(num);
    if(!tmp)
        return (NULL);
    res = ft_strdup(tmp);
    free(tmp);
    return (res);
}
char *heredoc_input(const char *delimeter)
{
    int     fd;
    char    *filename;

    filename = heredoc_filename();
    if (!filename)
        return (NULL);
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        free(filename);
        return (NULL);
    }
    write_heredoc_lines(fd, delimeter);
    close(fd);
    return (filename);
}
static int  handle_heredoc(t_command *cmd)
{
    char    *tmpfile;

    if (!cmd->heredoc || !cmd->infile)
        return (0);
    tmpfile = heredoc_input(cmd->infile);
    if (!tmpfile)
    {
        perror("heredoc");
        return (1);
    }
    free(cmd->infile);
    cmd->infile = tmpfile;
    return (0);
}
static void child_process(t_command *cmd, char **envp, char *path)
{
    if (redirect_io(cmd))
        exit(1);
    execve(path, cmd->args, envp);
    perror("execve");
    exit(1);
}
void    execute_command(t_command *cmd, char ***envp)
{
    pid_t   pid;
    char    *path;

    if (!cmd || !cmd->args || !cmd->args[0])
        return ;
    if (is_builtin(cmd->args[0]))
    {
        int stdincpy = dup(STDIN_FILENO);
        int stdoutcpy = dup(STDOUT_FILENO);
        redirect_io(cmd);
        g_exit_status = execute_builtin(cmd, envp);
        dup2(stdincpy, STDIN_FILENO);
        dup2(stdoutcpy, STDOUT_FILENO);
        close(stdincpy);
        close(stdoutcpy);
        return ;
    }
    if (handle_heredoc(cmd))
        return ;
    path = find_executable(cmd->args[0]);
    if (!path)
        return ;
    pid = fork();
    if (pid == 0)
        child_process(cmd, *envp, path);
    else if (pid > 0)
    {
        int status;
        waitpid(pid, &status, 0);
        g_exit_status = WIFEXITED(status) ? WEXITSTATUS(status) : 1;
    }
    else
        perror("fork");
    free(path);
}
