/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuzun <emuzun@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 17:00:00 by emuzun            #+#    #+#             */
/*   Updated: 2025/08/13 19:21:59 by emuzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static long long	ft_atoll(char *str)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	sign = 1;
	i = 0;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

int	builtin_exit(char **args)
{
	long long	exit_code;

	if (write(1, "exit\n", 5) == -1)
		return (1);
	if (!args[1])
		exit(g_exit_status);
	if (!is_numeric(args[1]))
	{
		if (write(2, "minishell: exit: ", 17) == -1)
			return (1);
		if (write(2, args[1], ft_strlen(args[1])) == -1)
			return (1);
		if (write(2, ": numeric argument required\n", 28) == -1)
			return (1);
		exit(2); 
	}
	if (args[2])
	{
		if (write(2, "minishell: exit: too many arguments\n", 36) == -1)
			return (1);
		return (1);
	}
	exit_code = ft_atoll(args[1]);
	exit((int)(exit_code % 256));
}
