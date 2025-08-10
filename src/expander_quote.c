/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuzun <emuzun@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 21:20:46 by emuzun            #+#    #+#             */
/*   Updated: 2025/08/09 22:33:04 by emuzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

int has_quotes(char *str)
{
    int i;

    i = 0;

    while(str[i])
    {
        if(str[i] == '\'' || str[i] == '"')
            return(1);
        i++;
    }
    return(0);
}

static void shift_string(char *str, int pos)
{
    int i;

    i = pos;
    while(str[i])
    {
        str[i] = str[i + 1];
        i++;
    }
}

void remove_quotes(char *str)
{
    int i;
    char quote;

    i = 0;
    while(str[i])
    {
        if (str[i] == '\'' || str[i] == '"')
        {
            quote = str[i];
            shift_string(str,i);
            while(str[i] && str[i] != quote)
                i++;
            if(str[i] == quote)
                shift_string(str, i);
        }
        else
            i++;
    }
}