/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuzun <emuzun@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 12:00:00 by hialpagu          #+#    #+#             */
/*   Updated: 2025/08/16 18:55:42 by emuzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_single_quote(char *str)
{
	return (ft_strdup(str));
}

static char	*expand_double_quote(char *str, char **env)
{
	return (expand_string(str, env));
}

static char	*expand_regular_word(char *str, char **env)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (expand_string(str, env));
		i++;
	}
	return (ft_strdup(str));
}

static char	*get_expanded_str(t_token *token, char **env)
{
	if (token->type == T_WORD_SINGLE)
		return (expand_single_quote(token->str));
	else if (token->type == T_WORD_DOUBLE)
		return (expand_double_quote(token->str, env));
	else if (token->type == T_WORD)
		return (expand_regular_word(token->str, env));
	return (NULL);
}

t_token	*expand_tokens(t_token *tokens, char **env)
{
	t_token	*current;
	char	*new_str;

	current = tokens;
	while (current)
	{
		new_str = get_expanded_str(current, env);
		if (new_str)
		{
			free(current->str);
			current->str = new_str;
		}
		else if (current->type <= T_WORD_DOUBLE)
			return (NULL);
		current = current->next;
	}
	return (tokens);
}