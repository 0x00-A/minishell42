/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aigounad <aigounad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 15:31:17 by aigounad          #+#    #+#             */
/*   Updated: 2023/05/24 16:07:03 by aigounad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_c(t_execve_params *execve_params, t_env *env)
{
	execve(execve_params->path, execve_params->args, env->env);
	write(2, "minishell: ", 11);
	perror(execve_params->path);
	if (errno == EACCES)
	{
		exit(126);
	}
	else if (errno == ENOENT)
	{
		exit(127);
	}
}

void	ft_piping(t_list *cmd, t_fd *fd)
{
	if (cmd->next)
		if (pipe(fd->fd) == -1)
			perror("pipe");
}

void	dup_stdin_and_stdout(t_list *cmd, t_fd *fd)
{
	if (fd->old_fd != -1)
	{
		if (dup2(fd->old_fd, STDIN_FILENO) == -1)
			perror("dup2");
		if (close(fd->old_fd) == -1)
			perror("close");
	}
	if (cmd->next)
	{
		if (close(fd->fd[0]) == -1)
			perror("close");
		dup2(fd->fd[1], STDOUT_FILENO);
		if (close(fd->fd[1]) == -1)
			perror("close");
	}
}

// int	isFileDescriptorValid(int fd)
// {
//     return fcntl(fd, F_GETFD) != -1;
// }

void	dup_redirections(t_list *cmd)
{
	char	*file;
	int		fd;

	file = ((t_cmd *)(((t_token *)(cmd->content))->value))->file_out;
	if (file)
	{
		fd = ((t_cmd *)(((t_token *)(cmd->content))->value))->cmd_out;
		if (dup2(fd, STDOUT_FILENO) == -1)
			perror("dup2");
	}
	file = ((t_cmd *)(((t_token *)(cmd->content))->value))->file_in;
	if (file)
	{
		if (ft_strcmp(file, ".here_doc") == 0)
		{
			fd = open(".here_doc", O_RDONLY, 0666);
			if (fd == -1)
				perror("open");
			((t_cmd *)(((t_token *)(cmd->content))->value))->cmd_in = fd;
		}
		else
			fd = ((t_cmd *)(((t_token *)(cmd->content))->value))->cmd_in;
		if (dup2(fd, STDIN_FILENO) == -1)
			perror("dup222");
	}
}

char	**get_args(t_list *list)
{
	char	**args;
	size_t	size;
	t_list	*arg_list;
	char	*curr_arg;
	size_t	index;

	size = 1 + ((t_cmd *)((t_token *)(list->content))->value)->arg_count;
	args = malloc(sizeof(char *) * (size + 1));
	if (!args)
		ft_perror("minishell: malloc");
	arg_list = ((t_cmd *)((t_token *)(list->content))->value)->arg;
	index = 0;
	args[index++] = ((t_cmd *)((t_token *)(list->content))->value)->cmd;
	while (arg_list)
	{
		curr_arg = ((t_file *)(arg_list->content))->a_file;
		if (*curr_arg)
			args[index++] = curr_arg;
		arg_list = arg_list->next;
	}
	args[index] = 0;
	return (args);
}