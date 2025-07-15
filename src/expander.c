#include "minishell.h"

char *get_var_value(const char *name, char **envp)
{
    if (strcmp(name, "?") == 0)
        return ft_itoa(g_exit_status);

    int i = 0;
    size_t len = strlen(name);
    while (envp[i])
    {
        if (strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
            return strdup(&envp[i][len + 1]); // değeri döndür
        i++;
    }
    return NULL;
}


char *expand_variables(const char *input, char **envp)
{
    char *result = calloc(1, 1); // başlangıç boş string
    size_t i = 0;

    while (input[i])
    {
        if (input[i] == '$')
        {
            i++;
            size_t start = i;
            if (input[start] == '?') // sadece $?
                i++;
            else
                while (input[i] && (isalnum(input[i]) || input[i] == '_'))
                    i++;

            size_t len = i - start;
            char *varname = strndup(&input[start], len);
            char *value = get_var_value(varname, envp);
            free(varname);

            if (value)
            {
                char *tmp = ft_strjoin(result, value);
                free(result);
                result = tmp;
                if (strcmp(&input[start], "?") == 0)
                    free(value); // ft_itoa ile geldiyse free!
            }
        }
        else
        {
            char tmp[2] = {input[i], 0};
            char *new_result = ft_strjoin(result, tmp);
            free(result);
            result = new_result;
            i++;
        }
    }
    return result;
}
