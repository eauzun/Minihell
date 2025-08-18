/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuzun <emuzun@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:00:00 by emuzun            #+#    #+#             */
/*   Updated: 2025/08/18 14:54:23 by emuzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



static char	**remove_env_var(char **env, char *name)
{
	char	**new_env;
	int		size;
	int		i;
	int		j;
	int		target_pos;

	target_pos = find_env_var(env, name);
	if (target_pos == -1)
		return (copy_env_array(env));
	size = get_env_size(env);
	new_env = malloc(sizeof(char *) * size);
	if (!new_env)
		return (NULL);
	i = 0;
	j = 0;
	while (i < size)
	{
		if (i != target_pos)
		{
			new_env[j] = ft_strdup(env[i]);
			if (!new_env[j])
			{
				free_env_array(new_env);
				return (NULL);
			}
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	return (new_env);
}

static int	unset_single_var(char *name, char ***env)
{
	char	**new_env;

	if (!is_valid_identifier(name))
	{
		if (write(2, "minishell: unset: `", 19) == -1)
			return (1);
		if (write(2, name, ft_strlen(name)) == -1)
			return (1);
		if (write(2, "': not a valid identifier\n", 26) == -1)
			return (1);
		return (1);
	}
	new_env = remove_env_var(*env, name);
	if (!new_env)
		return (1);
	free_env_array(*env);
	*env = new_env;
	return (0);
}

int	builtin_unset(char **args, char ***env)
{
	int	i;
	int	exit_status;
	int	temp_status;

	if (!args || !env || !*env)
		return (1);
	if (!args[1])
		return (0);
	exit_status = 0;
	i = 1;
	while (args[i])
	{
		temp_status = unset_single_var(args[i], env);
		if (temp_status != 0)
			exit_status = temp_status;
		i++;
	}
	return (exit_status);
}