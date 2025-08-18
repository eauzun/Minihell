/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuzun <emuzun@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 11:00:00 by emuzun            #+#    #+#             */
/*   Updated: 2025/08/18 14:11:20 by emuzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_home_path(char **env)
{
	char	*home;

	home = get_env_value("HOME", env);
	if (!home)
	{
		if (write(2, "minishell: cd: HOME not set\n", 28) == -1)
			return (NULL);
		return (NULL);
	}
	return (ft_strdup(home));
}

static char	*get_oldpwd_path(char **env)
{
	char	*oldpwd;

	oldpwd = get_env_value("OLDPWD", env);
	if (!oldpwd)
	{
		if (write(2, "minishell: cd: OLDPWD not set\n", 30) == -1)
			return (NULL);
		return (NULL);
	}
	return (ft_strdup(oldpwd));
}

static char	*get_target_path(char **args, char **env)
{
	if (!args[1])
		return (get_home_path(env));
	if (ft_strcmp(args[1], "-") == 0)
		return (get_oldpwd_path(env));
	if (ft_strcmp(args[1], "~") == 0)
		return (get_home_path(env));
	return (ft_strdup(args[1]));
}

static int	update_pwd_vars(char ***env, char *old_pwd, char *new_pwd)
{
	int	result;

	result = 0;
	if (set_env_var(env, "OLDPWD", old_pwd) != 0)
		result = 1;
	if (set_env_var(env, "PWD", new_pwd) != 0)
		result = 1;
	return (result);
}

static void	print_pwd_if_dash(char *path, char *args_1)
{
	if (ft_strcmp(args_1, "-") == 0)
	{
		if (write(1, path, ft_strlen(path)) == -1)
			return ;
		if (write(1, "\n", 1) == -1)
			return ;
	}
}

int	builtin_cd(char **args, char ***env)
{
	char	*target_path;
	char	old_pwd[PATH_MAX];
	char	new_pwd[PATH_MAX];

	if (!getcwd(old_pwd, sizeof(old_pwd)))
		return (1);
	target_path = get_target_path(args, *env);
	if (!target_path)
		return (1);
	if (chdir(target_path) != 0)
	{
		perror("minishell: cd");
		free(target_path);
		return (1);
	}
	if (!getcwd(new_pwd, sizeof(new_pwd)))
	{
		free(target_path);
		return (1);
	}
	if (args[1])
		print_pwd_if_dash(new_pwd, args[1]);
	free(target_path);
	return (update_pwd_vars(env, old_pwd, new_pwd));
}