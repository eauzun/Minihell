/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuzun <emuzun@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:11:30 by emuzun            #+#    #+#             */
/*   Updated: 2025/08/13 16:20:54 by emuzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

int builtin_pwd(void)
{
    char *cwd;
    char buffer[PATH_MAX];
    
    cwd = getcwd(buffer, sizeof(buffer));
    if(!cwd)
    {
        perror("pwd");
        return(1);
    }
    if(write(1,cwd, ft_strlen(cwd)) == -1)
        return(1);
    if(write(1, "\n", 1) == -1)
        return(1);
    return(0);
}