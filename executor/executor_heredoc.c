#include "../minishell.h"

int	handle_quit(char *line, int *p)
{
	if (line[0] == '\0' && g_exit_code(-1) == 130)
	{
		free(line);
		close(p[0]);
		close(p[1]);
		return (1);
	}
	return (0);
}

char	*process_line(char *line, char **env, int flag)
{
	char	*tmp;

	if (flag)
	{
		tmp = expand_string(line, env);
		free(line);
		line = tmp;
	}
	return (line);
}

int	read_lines(t_command *cur, char **env, int *p)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (handle_quit(line, p))
			return (1);
		if (ft_strcmp(line, cur->infile) == 0)
		{
			free(line);
			break ;
		}
		line = process_line(line, env, cur->heredoc_expand);
		if (write(p[1], line, ft_strlen(line)) == -1
			|| write(p[1], "\n", 1) == -1)
		{
			free(line);
			break ;
		}
		free(line);
	}
	return (0);
}

int	exec_heredoc(t_command *cur, char **env)
{
	int	p[2];

	if (pipe(p) == -1)
	{
		perror("pipe");
		return (1);
	}
	if (read_lines(cur, env, p))
		return (1);
	close(p[1]);
	if (dup2(p[0], STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(p[0]);
		return (1);
	}
	close(p[0]);
	return (0);
}
