#include "../minishell.h"

int	is_word(t_token *token)
{
	return (token && token->type >= T_WORD && token->type <= T_WORD_DOUBLE);
}

char	*gather_word(t_token **cur)
{
	char	*acc;
	char	*tmp;
	t_token	*t;

	t = *cur;
	acc = ft_strdup("");
	if (!acc)
		return (NULL);
	while (is_word(t))
	{
		if (t == *cur || t->sep_ws == 0)
		{
			tmp = ft_strjoin(acc, t->str);
			free(acc);
			if (!tmp)
				return (NULL);
			acc = tmp;
			t = t->next;
		}
		else
			break ;
	}
	*cur = t;
	return (acc);
}

t_command	*parse_single_command(t_token **tokens)
{
	t_command	*cmd;
	t_token		*cur;
	char		*merged;

	cmd = create_command();
	if (!cmd)
		return (NULL);
	cur = *tokens;
	while (cur && cur->type != T_PIPE)
	{
		if (cur->type >= T_REDIR_IN && cur->type <= T_HEREDOC)
			parse_redirections(&cur, cmd);
		else if (is_word(cur))
		{
			merged = gather_word(&cur);
			add_argument(cmd, merged);
			free(merged);
		}
		else
			cur = cur->next;
	}
	*tokens = cur;
	return (cmd);
}

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*head;
	t_command	*cmd;

	head = NULL;
	if (!tokens || validate_syntax(tokens))
		return (NULL);
	while (tokens)
	{
		cmd = parse_single_command(&tokens);
		if (!cmd)
		{
			free_commands(head);
			return (NULL);
		}
		link_commands(&head, cmd);
		if (tokens && tokens->type == T_PIPE)
			tokens = tokens->next;
	}
	return (head);
}
