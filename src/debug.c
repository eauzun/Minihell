/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuzun <emuzun@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:00:00 by hialpagu          #+#    #+#             */
/*   Updated: 2025/07/21 18:17:39 by emuzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_token *tokens)
{
	t_token	*current;
	char	*types[6];

	types[0] = "WORD";
	types[1] = "PIPE";
	types[2] = "REDIR_IN";
	types[3] = "REDIR_OUT";
	types[4] = "REDIR_APPEND";
	types[5] = "HEREDOC";
	current = tokens;
	printf("\n=== TOKENS ===\n");
	while (current)
	{
		printf("Type: %-12s | Value: '%s'\n", types[current->type], 
			current->str);
		current = current->next;
	}
	printf("\n");
}

void	print_commands(t_command *cmd)
{
	int	i;
	int	cmd_num;

	cmd_num = 1;
	printf("\n=== COMMANDS ===\n");
	while (cmd)
	{
		printf("\n[Command %d]\n", cmd_num++);
		printf("Args: ");
		i = 0;
		while (cmd->args && cmd->args[i])
		{
			printf("'%s' ", cmd->args[i]);
			i++;
		}
		printf("\n");
		if (cmd->infile)
			printf("Input: '%s' (heredoc: %s)\n", cmd->infile, 
				cmd->heredoc ? "yes" : "no");
		if (cmd->outfile)
			printf("Output: '%s' (append: %s)\n", cmd->outfile, 
				cmd->append ? "yes" : "no");
		cmd = cmd->next;
	}
	printf("\n");
}