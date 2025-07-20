/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hialpagu <hialpagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:38:14 by hialpagu          #+#    #+#             */
/*   Updated: 2025/07/15 19:37:31 by hialpagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//token_word
t_token *token_word(const char *line, size_t *i)
{
    size_t  start;
    
    start = *i;
    while (line[*i]
        && line[*i] != ' ' && line[*i] != '\t' && line[*i] != '\n'
        && line[*i] != '|' && line[*i] != '<' && line[*i] != '>'
        && line[*i] != '\'' && line[*i] != '"')
        (*i)++;
    return (new_token(T_WORD, &line[start], *i - start));
}

//free_token
void    free_token(t_token *list)
{
    t_token *tmp;

    while (list)
    {
        tmp = list->next;
        free(list->str);
        free(list);
        list = tmp;
    }
}
//add_token
void    add_token(t_token **list, t_token *tok)
{
    t_token *current;
    
    if (!*list)
        *list = tok;
    else
    {
        current = *list;
        while (current->next)
            current = current->next;
        current->next = tok;
    }
}

//new_token
t_token	*new_token(t_token_type type, const char *start, size_t len)
{
	t_token *tok;
    size_t  i;

    i = 0;
    tok = malloc(sizeof(*tok));
    if(!tok)
        return (NULL);
    tok->str = malloc(len + 1);
    if (!tok->str)
    {
        free(tok);
        return (NULL);
    }
    while (i < len)
    {
        tok->str[i] = start[i];
        i++;
    }
    tok->str[i] = '\0';
    tok->type = type;
    tok->next = NULL;
    return (tok);  
}
