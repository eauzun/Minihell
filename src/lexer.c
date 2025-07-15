#include "minishell.h"

//token utils
//yeni token oluşturmak için
static t_token *new_token(t_token_type type, const char *start, size_t len)
{
    t_token *tok;
    size_t  i;

    i = 0;
    tok = malloc(sizeof *tok);
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

//hata basmak için
static void syntax_error(const char *msg)
{
    printf("%s", msg);
    //write(2, msg, ft_strlen(msg));
}
// linked liste eklemek için
void    add_token(t_token **list, t_token *tok)
{
    t_token *current;
    
    if (!*list)
        *list = tok;
    else
    {
        current = *list;
        //listenin sonuna ilerle
        while (current->next)
            current = current->next;
        // en sona yeni token'ı ekle
        current->next = tok;
    }
}
//hata çıkarsa vs freelemiz lazım
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
//boşlukları atla
static void skip_whitespace(const char *line, size_t *i)
{
    while (line[*i] == ' ' || line[*i] == '\t' || line[*i] == '\n')
        (*i)++;
}

static t_token  *single_quote(const char *line, size_t *i)
{
    t_token *tok;
    size_t  start;
    
    start = ++(*i);
    while (line[*i] && line[*i] != '\'')
        (*i)++;
    if (line[*i] != '\'')
    {
        syntax_error("minishell: syntax error: unclosed '\n");
        return (NULL);
    }
    tok = new_token(T_WORD, &line[start], *i - start);
    (*i)++;
    return (tok);
}

static t_token  *double_quote(const char *line, size_t *i)
{
    t_token *tok;
    size_t  start;
    
    start = ++(*i);
    while (line[*i] && line[*i] != '"')
        (*i)++;
    if (line[*i] != '"')
    {
        syntax_error("minishell: syntax error: unclosed \"\n");
        return (NULL);
    }
    tok = new_token(T_WORD, &line[start], *i - start);
    (*i)++;
    return (tok);
}
static t_token *redirections(const char *line, size_t *i)
{
    t_token_type    symbol;
    t_token         *tok;
    size_t          len;

    if ((line[*i] == '>' && line[*i + 1] == '>') || (line[*i] == '<' && line[*i + 1] == '<'))
    {
        if (line[*i] == '>')
            symbol = T_REDIR_APPEND;
        else
            symbol = T_HEREDOC;
        len = 2;
    }
    else if (line[*i] == '>' || line[*i] == '<')
    {
        if (line[*i] == '>')
            symbol = T_REDIR_OUT;
        else
            symbol = T_REDIR_IN;
        len = 1;
    }
    else
        return (NULL);
    tok = new_token(symbol, &line[*i], len);
    *i += len;
    return (tok);
}
static t_token *is_pipe(const char *line, size_t *i)
{
    t_token_type    symbol;
    t_token         *tok;
    size_t          len;

    symbol = T_PIPE;
    len = 1;
    tok = new_token(symbol, &line[*i], len);
    *i += len;
    return (tok);
}
// normal kelime oku
static t_token *token_word(const char *line, size_t *i)
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

t_token *tokenize(const char *line)
{
    t_token *tokens;
    t_token *tok;
    size_t  i;

    tokens = NULL;
    i = 0;
    while (line[i])
    {
        skip_whitespace(line, &i);
        if (!line[i])
            break ;
        if (line[i] == '\'')
            tok = single_quote(line, &i);
        else if (line[i] == '"')
            tok = double_quote(line, &i);
        else if (line[i] == '<' || line[i] == '>')
            tok = redirections(line, &i);
        else if (line[i] == '|')
            tok = is_pipe(line, &i);
        else
            tok = token_word(line, &i);
        if(!tok)
        {
            free_token(tokens);
            return (NULL);
        }
        add_token(&tokens, tok);
    }
    return (tokens);
}
// echo $HOME 'literal $HOME' "expand $HOME" | grep foo < input.txt > output.txt >> append.log << EOF
// ls -l|grep ".c"$ | sort <unsorted.txt > sorted.txt >> log.txt
// echo "hello  world" 'goodbye    world ' simple_word
// echo "unterminated