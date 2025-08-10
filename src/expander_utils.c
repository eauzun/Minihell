/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuzun <emuzun@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 12:00:00 by hialpagu          #+#    #+#             */
/*   Updated: 2025/08/09 22:31:49 by emuzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*join_and_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (result);
}

static char	*expand_in_quotes(char *str, size_t *i, char **env, char quote)
{
	char	*result;
	char	*tmp;
	size_t	start;

	result = ft_strdup("");
	(*i)++;
	while (str[*i] && str[*i] != quote)
	{
		if (str[*i] == '$' && quote == '"')
		{
			tmp = handle_dollar(str, i, env);
			result = join_and_free(result, tmp);
		}
		else
		{
			start = *i;
			while (str[*i] && str[*i] != quote && 
				!(str[*i] == '$' && quote == '"'))
				(*i)++;
			tmp = ft_substr(str, start, *i - start);
			result = join_and_free(result, tmp);
		}
	}
	if (str[*i] == quote)
		(*i)++;
	return (result);
}

char	*expand_string(char *str, char **env)
{
	char	*result;
	char	*tmp;
	size_t	i;
	size_t	start;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			tmp = expand_in_quotes(str, &i, env, str[i]);
			result = join_and_free(result, tmp);
		}
		else if (str[i] == '$')
		{
			tmp = handle_dollar(str, &i, env);
			result = join_and_free(result, tmp);
		}
		else
		{
			start = i;
			while (str[i] && str[i] != '$' && str[i] != '\'' 
				&& str[i] != '"')
				i++;
			tmp = ft_substr(str, start, i - start);
			result = join_and_free(result, tmp);
		}
	}
	return (result);
}