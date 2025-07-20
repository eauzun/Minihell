/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hialpagu <hialpagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:00:04 by hialpagu          #+#    #+#             */
/*   Updated: 2025/07/20 15:41:46 by hialpagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
static void skip_whitespace(const char *line, size_t *i)
{
    while (line[*i] == ' ' || line[*i] == '\t' || line[*i] == '\n')
        (*i)++;
}

static t_token *handle_special(const char *line, size_t *i)
{
	if (line[*i] == '\'')
		return (single_quote(line, i));
	else if (line[*i] == '"')
		return (double_quote(line, i));
	else if (line[*i] == '<' || line[*i] == '>')
		return (redirections(line, i));
	else if (line[*i] == '|')
		return (is_pipe(line, i));
	return (NULL);
}

t_token	*lexer_init(const char *line)
{
	t_token	*tokens;
	t_token	*tok;
	size_t	i;

	tokens = NULL;
	i = 0;
	while (line[i])
	{
		skip_whitespace(line, &i);
		if (!line[i])
			break ;
		if (line[i] == '\'' || line[i] == '"' || line[i] == '<' 
			|| line[i] == '>' || line[i] == '|')
			tok = handle_special(line, &i);
		else
			tok = token_word(line, &i);
		if (!tok)
		{
			free_token(tokens);
			return (NULL);
		}
		add_token(&tokens, tok);
	}
	return (tokens);
}
