/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_variables.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuzun <emuzun@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 12:00:00 by hialpagu          #+#    #+#             */
/*   Updated: 2025/08/09 22:32:44 by emuzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(char *name, char **env)
{
	int		i;
	size_t	len;

	if (!name || !env)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

static char	*get_var_name(char *str, size_t *i)
{
	size_t	start;
	size_t	len;

	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	len = *i - start;
	if (len == 0)
		return (NULL);
	return (ft_substr(str, start, len));
}

static char	*get_var_value(char *var_name, char **env)
{
	char	*value;

	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(g_exit_status));
	value = get_env_value(var_name, env);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

char	*handle_dollar(char *str, size_t *i, char **env)
{
	char	*var_name;
	char	*var_value;

	(*i)++;
	if (!str[*i] || str[*i] == ' ' || str[*i] == '"' || str[*i] == '\'')
		return (ft_strdup("$"));
	var_name = get_var_name(str, i);
	if (!var_name)
		return (ft_strdup("$"));
	var_value = get_var_value(var_name, env);
	free(var_name);
	return (var_value);
}