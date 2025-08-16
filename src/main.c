/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuzun <emuzun@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:00:04 by hialpagu          #+#    #+#             */
/*   Updated: 2025/08/16 18:49:02 by emuzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;

static char	**get_env_data(char **env)
{
	int		i;
	char	**arr;

	i = 0;
	while (env[i])
		i++;
	arr = malloc(sizeof(char *) * (i + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (env[i])
	{
		arr[i] = ft_strdup(env[i]);
		if (!arr[i])
		{
			while (--i >= 0)
				free(arr[i]);
			free(arr);
			return (NULL);
		}
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

static void	free_env_data(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

static void	execute_command_line(char *line, char ***env)
{
	t_token		*tokens;
	t_command	*commands;

	tokens = lexer_init(line);
	if (tokens)
	{
		tokens = expand_tokens(tokens, *env);
		if (tokens)
		{
			commands = parse_tokens(tokens);
			if (commands)
			{
				if (is_builtin(commands->args[0]))
					g_exit_status = execute_builtin(commands, env);
				else
					g_exit_status = 0;
				free_commands(commands);
			}
		}
		free_token(tokens);
	}
}

static void	minishell_loop(char **env)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (line[0])
		{
			add_history(line);
			execute_command_line(line, &env);
		}
		free(line);
	}
}

int	main(int ac, char **av, char **env)
{
	char	**cpy_env;

	if (ac != 1 || av[1] != NULL)
	{
		if (write(2, "minishell: no arguments allowed\n", 32) == -1)
			return (1);
		return (1);
	}
	cpy_env = get_env_data(env);
	if (!cpy_env)
	{
		if (write(2, "minishell: environment copy failed\n", 35) == -1)
			return (1);
		return (1);
	}
	set_signals();
	minishell_loop(cpy_env);
	free_env_data(cpy_env);
	return (g_exit_status);
}