/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsaktiwy <hsaktiwy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 17:03:39 by hsaktiwy          #+#    #+#             */
/*   Updated: 2023/05/05 16:58:46 by hsaktiwy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void handleHereDoc(t_list **tokens, t_env *env, char *input, int *index)
{
	t_token *token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return;
		
	*index += 2;
	token->type = HERE_DOC;
	token->value = get_file(env, &input[*index], index);
	ft_lstadd_back(tokens, ft_lstnew(token));
}

void handleInRedirect(t_list **tokens, t_env *env, char *input, int *index)
{
	t_token *token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return;
		
	++(*index);
	token->type = IN_REDIRECT;
	token->value = get_file(env, &input[*index], index);
	ft_lstadd_back(tokens, ft_lstnew(token));
}

void handleAppendRedirect(t_list **tokens, t_env *env, char *input, int *index)
{
	t_token *token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return;
		
	*index += 2;
	token->type = APPEND_REDIRECT;
	token->value = get_file(env, &input[*index], index);
	ft_lstadd_back(tokens, ft_lstnew(token));
}

void handleOutRedirect(t_list **tokens, t_env *env, char *input, int *index)
{
	t_token *token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return;
		
	++(*index);
	token->type = OUT_REDIRECT;
	token->value = get_file(env, &input[*index], index);
	ft_lstadd_back(tokens, ft_lstnew(token));
}

void handlePipe(t_list **tokens, int *index, int *cmd)
{
	t_token *token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return;
	token->type = PIPE;
	token->value = NULL;
	ft_lstadd_back(tokens, ft_lstnew(token));
	(*index)++;
	*cmd = 0;
}

int lexer(t_list **tokens, char *input, t_env *env)
{
	int i;
	int	cmd;

	i = 0;
	cmd = 0;
	while (input[i])
	{
		if (input[i] == '<' && input[i + 1] && input[i + 1] == '<')
			handleHereDoc(tokens, env, input, &i);
		else if (input[i] == '<' && input[i + 1] != '<')
			handleInRedirect(tokens, env, input, &i);
		else if (input[i] == '>' && input[i + 1] && input[i + 1] == '>')
			handleAppendRedirect(tokens, env, input, &i);
		else if (input[i] == '>' && input[i + 1] != '>')
			handleOutRedirect(tokens, env, input, &i);
		else if (input[i] == '|')
			handlePipe(tokens, &i, &cmd);
		else if (cmd == 0)
		{
			handleCommand(tokens, env, input, &i);
			cmd = 1;
		}
		else
			handleArg(tokens, env, input, &i);
		if (iswhitespace(input[i]))
			i++;
	}
	return 0;
}
