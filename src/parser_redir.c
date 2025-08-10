/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuzun <emuzun@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:00:00 by hialpagu          #+#    #+#             */
/*   Updated: 2025/08/09 22:28:15 by emuzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_filename(t_token **current)
{
	char	*filename;
	size_t	len;
	size_t	i;

	*current = (*current)->next;
	if (!*current || (*current)->type != T_WORD)
	{
		if (write(2, "syntax error near unexpected token\n", 35) == -1)
			return (NULL);
		return (NULL);
	}
	len = ft_strlen((*current)->str);
	filename = malloc(len + 1);
	if (!filename)
		return (NULL);
	i = 0;
	while (i < len)
	{
		filename[i] = (*current)->str[i];
		i++;
	}
	filename[i] = '\0';
	return (filename);
}

static void	handle_input_redir(t_token **current, t_command *cmd)
{
	if (cmd->infile)
		free(cmd->infile);
	cmd->infile = get_filename(current);
	cmd->heredoc = 0;
}

static void	handle_output_redir(t_token **current, t_command *cmd)
{
	if (cmd->outfile)
		free(cmd->outfile);
	cmd->outfile = get_filename(current);
	cmd->append = 0;
}

static void	handle_append_redir(t_token **current, t_command *cmd)
{
	if (cmd->outfile)
		free(cmd->outfile);
	cmd->outfile = get_filename(current);
	cmd->append = 1;
}

void	parse_redirections(t_token **current, t_command *cmd)
{
	t_token_type	type;

	type = (*current)->type;
	if (type == T_REDIR_IN)
		handle_input_redir(current, cmd);
	else if (type == T_REDIR_OUT)
		handle_output_redir(current, cmd);
	else if (type == T_REDIR_APPEND)
		handle_append_redir(current, cmd);
	else if (type == T_HEREDOC)
	{
		if (cmd->infile)
			free(cmd->infile);
		cmd->infile = get_filename(current);
		cmd->heredoc = 1;
	}
	if (*current)
		*current = (*current)->next;
}