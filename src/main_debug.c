/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_debug.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuzun <emuzun@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 21:32:31 by emuzun            #+#    #+#             */
/*   Updated: 2025/08/13 15:24:06 by emuzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"


int	g_exit_status = 0;

static char	**get_env_data(char **env)
{
	int		i;
	int		j;
	char	**arr;

	i = 0;
	while (env[i])
		i++;
	arr = malloc(sizeof(char *) * (i + 1));
	if (!arr)
		return (NULL);
	arr[i] = NULL;
	while (i--)
	{
		j = 0;
		while (env[i][j])
			j++;
		arr[i] = malloc(sizeof(char) * (j + 1));
		if (!arr[i])
			return (NULL);
		while (j--)
			arr[i][j] = env[i][j];
		j = ft_strlen(env[i]);
		arr[i][j] = '\0';
	}
	return (arr);
}

static void	execute_builtin_test(t_command *cmd, char **env)
{
	int	exit_code;

	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (is_builtin(cmd->args[0]))
	{
		printf("\n🔧 EXECUTING BUILTIN: %s\n", cmd->args[0]);
		printf("Output:\n");
		exit_code = execute_builtin(cmd, &env);
		printf("\n");
		g_exit_status = exit_code;
		printf("Exit Status: %d\n", exit_code);
	}
}

void	minishell_debug(char *line, char **env)
{
	t_token		*tokens;
	t_command	*commands;

	g_exit_status = 0;
	set_signals();
	printf("🐚 MINISHELL DEBUG MODE\n");
	printf("✅ Built-ins: echo, pwd, env, cd, export, unset, exit\n");
	printf("✅ Variables: $USER, $HOME, $?\n\n");
	while (1)
	{
		line = readline("debug$ ");
		if (!line)
		{
			printf("exit\n");
			exit(0);
		}
		if (line[0])
			add_history(line);
		if (ft_strcmp(line, "exit") == 0)
		{
			free(line);
			exit(0);
		}
		tokens = lexer_init(line);
		if (tokens)
		{
			printf("\n🔸 BEFORE EXPANSION:\n");
			print_tokens(tokens);
			tokens = expand_tokens(tokens, env);
			if (tokens)
			{
				printf("🔹 AFTER EXPANSION:\n");
				print_tokens(tokens);
				commands = parse_tokens(tokens);
				if (commands)
				{
					printf("📋 PARSED COMMANDS:\n");
					print_commands(commands);
					execute_builtin_test(commands, env);
					free_commands(commands);
				}
			}
			free_token(tokens);
		}
		free(line);
		printf("\n");
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
		if (write(2, "error\n", 6) == -1)
			exit(1);
		exit(1);
	}
	minishell_debug(line, cpy_env);
	return (0);
}