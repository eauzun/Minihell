/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuzun <emuzun@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 12:00:00 by hialpagu          #+#    #+#             */
/*   Updated: 2025/08/09 22:41:04 by emuzun           ###   ########.fr       */
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

t_token	*expand_tokens(t_token *tokens, char **env)
{
	t_token	*current;
	char	*new_str;

	current = tokens;
	while (current)
	{
		if (current->type == T_WORD_SINGLE)
			new_str = expand_single_quote(current->str);
		else if (current->type == T_WORD_DOUBLE)
			new_str = expand_double_quote(current->str, env);
		else if (current->type == T_WORD)
			new_str = expand_regular_word(current->str, env);
		else
		{
			current = current->next;
			continue ;
		}
		if (!new_str)
			return (NULL);
		free(current->str);
		current->str = new_str;
		current = current->next;
	}
	return (tokens);
}