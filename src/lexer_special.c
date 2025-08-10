/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_special.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuzun <emuzun@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:39:48 by hialpagu          #+#    #+#             */
/*   Updated: 2025/08/09 22:40:55 by emuzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//single_quote
t_token	*single_quote(const char *line, size_t *i)
{
	t_token	*tok;
	size_t	start;

	start = ++(*i);
	while (line[*i] && line[*i] != '\'')
		(*i)++;
	if (line[*i] != '\'')
	{
		if (write(2, "syntax error: unclosed quote\n", 29) == -1)
			return (NULL);
		return (NULL);
	}
	tok = new_token(T_WORD_SINGLE, &line[start], *i - start);
	(*i)++;
	return (tok);
}

//double_quote
t_token	*double_quote(const char *line, size_t *i)
{
	t_token	*tok;
	size_t	start;

	start = ++(*i);
	while (line[*i] && line[*i] != '"')
		(*i)++;
	if (line[*i] != '"')
	{
		if (write(2, "syntax error: unclosed quote\n", 29) == -1)
			return (NULL);
		return (NULL);
	}
	tok = new_token(T_WORD_DOUBLE, &line[start], *i - start);
	(*i)++;
	return (tok);
}

//redirections
t_token	*redirections(const char *line, size_t *i)
{
	t_token_type	symbol;
	size_t			start;
	
	start = *i;
	if ((line[*i] == '>' && line[*i + 1] == '>') 
		|| (line[*i] == '<' && line[*i + 1] == '<'))
	{
		if (line[*i] == '>')
			symbol = T_REDIR_APPEND;
		else
			symbol = T_HEREDOC;
		*i += 2;
	}
	else if (line[*i] == '>' || line[*i] == '<')
	{
		if (line[*i] == '>')
			symbol = T_REDIR_OUT;
		else
			symbol = T_REDIR_IN;
		*i += 1;
	}
	else
		return (NULL);
	return (new_token(symbol, &line[start], *i - start));
}

//is_pipe
t_token *is_pipe(const char *line, size_t *i)
{
    t_token_type    symbol;
    t_token         *tok;
    size_t          len;

    symbol = T_PIPE;
    len = 1;
    tok = new_token(symbol, &line[*i], len);
    *i += len;
    return (tok);
}