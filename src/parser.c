/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuzun <emuzun@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:00:00 by hialpagu          #+#    #+#             */
/*   Updated: 2025/07/21 18:11:08 by emuzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command	*parse_single_command(t_token **tokens)
{
	t_command	*cmd;
	t_token		*current;

	cmd = create_command();
	if (!cmd)
		return (NULL);
	current = *tokens;
	while (current && current->type != T_PIPE)
	{
		if (current->type >= T_REDIR_IN && current->type <= T_HEREDOC)
			parse_redirections(&current, cmd);
		else if (current->type == T_WORD)
		{
			add_argument(cmd, current->str);
			current = current->next;
		}
		else
			current = current->next;
	}
	*tokens = current;
	return (cmd);
}

static void	link_commands(t_command **head, t_command *new_cmd)
{
	t_command	*current;

	if (!*head)
		*head = new_cmd;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_cmd;
	}
}

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*head;
	t_command	*cmd;

	if (!tokens || !validate_syntax(tokens))
		return (NULL);
	head = NULL;
	while (tokens)
	{
		cmd = parse_single_command(&tokens);
		if (!cmd)
		{
			free_commands(head);
			return (NULL);
		}
		link_commands(&head, cmd);
		if (tokens && tokens->type == T_PIPE)
			tokens = tokens->next;
	}
	return (head);
}