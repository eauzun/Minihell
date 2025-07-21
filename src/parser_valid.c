/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_valid.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuzun <emuzun@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:00:00 by hialpagu          #+#    #+#             */
/*   Updated: 2025/07/21 18:13:15 by emuzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_pipe_syntax(t_token *current)
{
	if (!current->next || current->next->type == T_PIPE)
	{
		write(2, "syntax error near unexpected token `|'\n", 39);
		return (0);
	}
	return (1);
}

static int	check_redir_syntax(t_token *current)
{
	if (!current->next || current->next->type != T_WORD)
	{
		if (!current->next)
			write(2, "syntax error near unexpected token `newline'\n", 46);
		else if (current->next->type == T_PIPE)
			write(2, "syntax error near unexpected token `|'\n", 39);
		else
			write(2, "syntax error near unexpected token\n", 35);
		return (0);
	}
	return (1);
}

static int	check_first_token(t_token *tokens)
{
	if (tokens->type == T_PIPE)
	{
		write(2, "syntax error near unexpected token `|'\n", 39);
		return (0);
	}
	return (1);
}

int	validate_syntax(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (0);
	if (!check_first_token(tokens))
		return (0);
	current = tokens;
	while (current)
	{
		if (current->type == T_PIPE)
		{
			if (!check_pipe_syntax(current))
				return (0);
		}
		else if (current->type >= T_REDIR_IN && current->type <= T_HEREDOC)
		{
			if (!check_redir_syntax(current))
				return (0);
		}
		current = current->next;
	}
	return (1);
}