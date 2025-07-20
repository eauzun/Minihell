#include "minishell.h"

int	g_exit_status = 0;

static char    **get_env_data(char **env)
{
    int     i;
    int     j;
    char    **arr;

    i = 0;
    while (env[i])
        i++;
    arr = malloc(sizeof(char *) * (i + 1));
    if(!arr)
        return (NULL);
    arr[i] = NULL;
    while (i--)
    {
        j = 0;
        while (env[i][j])
            j++;
        arr[i] = malloc(sizeof(char) * (j + 1));
        if(!arr[i])
            return (NULL);
        while (j--)
            arr[i][j] = env[i][j];
		j = ft_strlen(env[i]);
		arr[i][j] = '\0';
    }
    return (arr);
}

void	minishell(char	*line, char **env)
{
	//t_token	*tokens;

	(void)env;
	g_exit_status = 0;
	set_signals();
	while (1)
	{
		line = readline("minishell$ ");
		if (!line) // ctrl + D olunca buraya giriyor
		{
			printf("exit\n");
			exit(0);
		}
		if (line[0])
			add_history(line);
		lexer_init(line);



		free(line);
	}
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	char	**cpy_env;

	line = NULL;
	if (ac != 1 || av[1] != NULL)
		exit(1);
	cpy_env = get_env_data(env);
	if (!cpy_env)
	{
		write (2, "error\n", 6);
		free(cpy_env);
	}
	minishell(line, cpy_env);
	return (0);
}
