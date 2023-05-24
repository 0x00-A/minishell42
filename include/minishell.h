/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aigounad <aigounad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 15:53:21 by hsaktiwy          #+#    #+#             */
/*   Updated: 2023/05/21 17:47:22 by aigounad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_SHELL_H
# define MINI_SHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <limits.h>
# include <string.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <errno.h>
# include <signal.h>
# include "structers.h"
# include "environment.h"
# include  "parser.h"
# include "lexer.h"
# include "builtins.h"
# include "execution.h"

// global variable
extern int	g_exit_status;



//global functions
int		g_cmd_executing(pid_t newpid);
int		g_heredoc_executing(int i);
int		g_stdin_fd(int i);
t_list *g_token_l(t_list *tokens);
char	*g_input_line(char *input);
int		g_pipe_count(int i);
#endif
