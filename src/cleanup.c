#include "../minishell.h"

static void free_env_array(char ***env)
{
    int i;

    if (!env || !*env)
        return;
    i = 0;
    while ((*env)[i])
    {
        free((*env)[i]);
        i++;
    }
    free(*env);
    *env = NULL;
}

void    ms_cleanup(char ***env)
{
    free_env_array(env);
    rl_clear_history();
}
