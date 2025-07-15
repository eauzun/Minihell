#include "minishell.h"
// kaç tane word olduğunu sayıyor arg sayısı için
static int  token_count(t_token *token)
{
    int count;

    count = 0;
    while (token && token->type == T_WORD)
    {
        count++;
        token = token->next;
    }
    return (count);
}
// bellek ayırıyor arg dizisini hazırlıyor
static t_command    *init_command_structure(t_token *token)
{
    t_command   *cmd;
    int         argc;

    cmd = calloc(1, sizeof(t_command));
    if (!cmd)
        return (NULL);
    argc = token_count(token);
    cmd->args = calloc(argc + 1, sizeof(char *));
    if (!cmd->args)
    {
        free(cmd);
        return (NULL);
    }
    return (cmd);
}
// <, >, <<, >> işliyor ve cmd'ye yerleştiriyor
static void handle_redirection(t_command *cmd, t_token **token)
{
    int is_heredoc;

    if(((*token)->type == T_REDIR_IN || (*token)->type == T_HEREDOC)
         && (*token)->next)
    {
        is_heredoc = (*token)->type == T_HEREDOC;
        *token = (*token)->next;
        cmd->infile = strdup((*token)->str);
        if (is_heredoc)
            cmd->heredoc = 1;
    }
    else if ((*token)->type == T_REDIR_OUT && (*token)->next)
    {
        cmd->append = 0;
        *token = (*token)->next;
        cmd->outfile = strdup((*token)->str);
    }
    else if ((*token)->type == T_REDIR_APPEND && (*token)->next)
    {
        cmd->append = 1;
        *token = (*token)->next;
        cmd->outfile = strdup((*token)->str);
    }
}
// argümanları (word) cmd->args dizisine koyacak
// redirection varsa handle_redirection()'a gönderecek
// pipe (|) görürse recursive olarak parse_command() fonksiyonunu çağıracak
static void fill_command_fields(t_command *cmd, t_token **tok, char **envp)
{
    int i;
    t_token *t;
    char    *expanded;

    i = 0;
    t = *tok;
    while (t)
    {
        if (t->type == T_WORD)
        {
            expanded =expand_variables(t->str, envp);
            if (!expanded)
                expanded = strdup("");
            cmd->args[i++] = expanded;
        }
        else if (t->type == T_REDIR_IN || t->type == T_REDIR_OUT
            || t->type == T_REDIR_APPEND || t->type == T_HEREDOC)
            handle_redirection(cmd, &t);
        else if (t->type == T_PIPE)
        {
            t = t->next;
            cmd->next = parse_command(t, envp);
            break ;
        }
        t = t->next;
    }
}
t_command   *parse_command(t_token *token, char **envp)
{
    t_command *cmd;

    cmd = init_command_structure(token);
    if (!cmd)
        return (NULL);
    fill_command_fields(cmd, &token, envp);
    return (cmd);
}
void    free_command(t_command *cmd)
{
    int i;
    t_command   *next;

    while (cmd)
    {
        next = cmd->next;
        if (cmd->args)
        {
            i = 0;
            while (cmd->args[i])
            {
                free(cmd->args[i]);
                i++;
            }
            free(cmd->args);
        }
        if (cmd->infile)
            free(cmd->infile);
        if (cmd->outfile)
            free(cmd->outfile);
        free(cmd);
        cmd = next;
    }
}
