#include "minishell.h"

int is_builtin(const char *cmd)
{
    return (!strcmp(cmd, "cd") || !strcmp(cmd, "echo") ||
            !strcmp(cmd, "pwd") || !strcmp(cmd, "exit") || 
            !strcmp(cmd, "export") || !strcmp(cmd, "unset") || 
            !strcmp(cmd, "env"));
}
int execute_builtin(t_command *cmd, char ***envp)
{
    (void)envp;
    if (!strcmp(cmd->args[0], "cd"))
        return (builtin_cd(cmd->args));
    else if (!strcmp(cmd->args[0], "echo"))
        return (builtin_echo(cmd->args));
    else if (!strcmp(cmd->args[0], "pwd"))
        return (builtin_pwd());
    else if (!strcmp(cmd->args[0], "exit"))
        return (builtin_exit(cmd->args));
    else if (!strcmp(cmd->args[0], "export"))
        return (builtin_export(cmd->args, envp));
    else if (!strcmp(cmd->args[0], "unset"))
        return (builtin_unset(cmd->args, envp));
    else if (!strcmp(cmd->args[0], "env"))
        return (builtin_env(*envp));
    return (1);
}

int builtin_cd(char **args)
{
    if(!args[1])
    {
        write(2, "minishell: cd: missing argument\n", 32);
        return (1);
    }
    if (chdir(args[1]) != 0)
    {
        perror("minishell: cd");
        return (1);
    }
    return (0);
}
int builtin_pwd(void)
{
    char    cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("%s\n", cwd);
    else
        perror("minishell: pwd");
    return (0);
}
int builtin_echo(char   **args)
{
    int i;
    int newline;

    i = 1;
    newline = 1;
    if (args[1] && strcmp(args[1], "-n") == 0)
    {
        newline = 0;
        i = 2;
    }
    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
    return (0);
}
static int	ft_atoi(const char *str)
{
	int		sign;
	int		result;
	size_t	i;

	i = 0;
	result = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = (result * 10) + (str[i] - 48);
		i++;
	}
	return (result * sign);
}
int builtin_exit(char **args)
{
    int exit_code;
    int i;

    i = 0;
    write(1, "exit\n", 5);
    if (!args[1])
        exit(0);
    while (args[1][i])
    {
        if (args[1][i] < '0' || args[1][i] > '9')
        {
            write(2, "minishell: exit: numeric argument required\n", 43);
            exit(255);
        }
        i++;
    }
    if (args[2])
    {
        write(2, "minishell: exit: too many arguments\n", 36);
        return (1);
    }
    exit_code = ft_atoi(args[1]) % 256;
    exit(exit_code);    
}
int builtin_env(char **envp)
{
    int i;

    i = 0;
    while (envp[i])
    {
        if (envp[i][0] && strchr(envp[i], '='))
            printf("%s\n", envp[i]);
        i++;
    }
    return (0);
}
static int  add_env(char *arg, char ***envp, int count)
{
    int i;
    char    **new_env;

    new_env = malloc(sizeof(char *) * (count + 2));
    if (!new_env)
        return (1);
    i = 0;
    while(i < count)
    {
        new_env[i] = strdup((*envp)[i]);
        if (!new_env[i])
            return (1);
        i++;
    }
    new_env[i++] = strdup(arg);
    new_env[i] = NULL;
    free_str_array(*envp);
    *envp = new_env;
    return (0);
}
static int export_var(char *arg, char ***envp)
{
    int i;
    int len;

    if(!strchr(arg, '='))
        return (0);
    len = strchr(arg, '=') - arg;
    i = 0;
    while((*envp)[i])
    {
        if (!strncmp((*envp)[i], arg, len) && (*envp)[i][len] == '=')
        {
            free((*envp)[i]);
            (*envp)[i] = strdup(arg);
            if (!(*envp)[i])
                return (1);
            return (0);
        }
        i++;
    }
    return(add_env(arg, envp, i));
}
int builtin_export(char **args, char ***envp)
{
    int i;

    i = 1;
    while (args[i])
    {
        if (export_var(args[i], envp))
            write(2, "minishell: export: memory error\n", 32);
        i++;
    }
    return (0);
}
static int remove_env_entry(char ***envp, int remove_index)
{
    int i = 0, j = 0;
    char **new_env;

    // envp uzunluğunu hesapla
    while ((*envp)[i])
        i++;

    // yeni diziyi oluştur (bir eksik girdi olacak)
    new_env = malloc(sizeof(char *) * i); // i = toplam, -1 silinen, +1 NULL
    if (!new_env)
        return (1);

    i = 0;
    while ((*envp)[i])
    {
        if (i != remove_index)
        {
            new_env[j] = strdup((*envp)[i]); // 🔒 güvenli kopyalama
            if (!new_env[j])
            {
                while (--j >= 0)
                    free(new_env[j]);
                free(new_env);
                return (1);
            }
            j++;
        }
        i++;
    }
    new_env[j] = NULL;

    // tüm eski dizi silinir
    free_str_array(*envp);
    *envp = new_env;
    return (0);
}

static int unset_var(char *key, char ***envp)
{
    int i;
    int len;

    i = 0;
    len = strlen(key);
    while((*envp)[i])
    {
        if (!strncmp((*envp)[i], key, len) && (*envp)[i][len] == '=')
            return (remove_env_entry(envp, i));
        i++;
    }
    return (0);
}
int builtin_unset(char **args, char ***envp)
{
    int i;

    i = 1;
    while(args[i])
    {
        if (unset_var(args[i], envp))
            return (1);
        i++;
    }
    return (0);
}
/*
minishell$ export FOO=bar
[WORD: "export"]
[WORD: "FOO=bar"]

== command ==
args[0]: export
args[1]: FOO=bar
===============

command not found: export
minishell(95004,0x200385f00) malloc: *** error for object 0x16bb535bc: pointer being freed was not allocated
minishell(95004,0x200385f00) malloc: *** set a breakpoint in malloc_error_break to debug
[1]    95004 abort      ./minishell
*/