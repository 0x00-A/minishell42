/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsaktiwy <hsaktiwy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 10:01:57 by aigounad          #+#    #+#             */
/*   Updated: 2023/05/24 15:00:27 by hsaktiwy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	ft_perror(char *msg)
{
	perror(msg);
	exit(1);
}

int	is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

int	exec_builtin(t_cmd *cmd, t_list *list)
{
	if (!ft_strcmp(cmd->cmd, "echo"))
		return (ft_echo(cmd));
	if (!ft_strcmp(cmd->cmd, "cd"))
		return (ft_cd(cmd));
	if (!ft_strcmp(cmd->cmd, "pwd"))
		return (ft_pwd(cmd));
	if (!ft_strcmp(cmd->cmd, "export"))
		return (ft_export(cmd));
	if (!ft_strcmp(cmd->cmd, "env"))
		return (ft_env(cmd));
	if (!ft_strcmp(cmd->cmd, "unset"))
		return (ft_unset(cmd));
	if (!ft_strcmp(cmd->cmd, "exit"))
		return (ft_exit(cmd, list));
	return (0);
}

static void	free_tab(char **pp)
{
	int	i;

	i = 0;
	while (pp[i])
	{
		free(pp[i++]);
	}
	free(pp);
}

void	append_filename(char *filename, char *path, char *des)
{
	int		j;
	int		k;

	j = -1;
	while (path[++j])
		des[j] = path[j];
	if (des[j - 1] != '/')
		des[j++] = '/';
	k = 0;
	while (filename[k])
		des[j++] = filename[k++];
	des[j] = 0;
}

char	*findcmd(char *filename)
{
	char	**paths;
	char	path[4096];
	int		i;

	if (is_builtin(filename) || ft_strchr(filename, '/')
		|| !ft_getenv(g_env_s(NULL), "PATH"))
		return (ft_strdup(filename));
	paths = ft_split(ft_getenv(g_env_s(NULL), "PATH"), ':');
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		if (!*(paths[i]))
			continue ;
		append_filename(filename, paths[i], path);
		if (access(path, 0) == 0)
			return (free_tab(paths), ft_strdup(path));
	}
	free_tab(paths);
	return (NULL);
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

void	dup_stdin_and_stdout(t_list *cmd, t_fd *fd)
{
	// dup stdin
	if (fd->old_fd != -1)
	{
		if (dup2(fd->old_fd, STDIN_FILENO) == -1)
			perror("dup2");
		if (close(fd->old_fd) == -1)
			perror("close");
	}
	// dup stdout
	if (cmd->next)
	{
		if (close(fd->fd[0]) == -1)
			perror("close");
		dup2(fd->fd[1], STDOUT_FILENO);
		if (close(fd->fd[1]) == -1)
			perror("close");
	}
}
int isFileDescriptorValid(int fd) {
    return fcntl(fd, F_GETFD) != -1;
}

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

void	save_cmd(t_execve_params *ep, t_env *env)
{
	char	**args;
	if (!ep->args || !*(ep->args))
		ft_setenv(&env, "_", ep->path);
	else
	{
		args = ep->args;
		while (*args)
		{
			if (!*(args + 1))
				ft_setenv(&env, "_", *args);
			args++;
		}
	}
}

void	exec_c(t_execve_params *execve_params, t_env *env)
{
	execve(execve_params->path, execve_params->args, env->env);
	write(2, "minishell: ", 11);
	perror(execve_params->path);
	if (errno == EACCES) //The filename argument is a Dir and permission denied
	{
		exit(126);
	}
	else if (errno == ENOENT) //No such file od directory
	{
		exit(127);
	}
}

void	execb2(t_list *cmd, t_list *list) //leak
{
	int	status;

	if (is_builtin(((t_cmd *)(((t_token *)(cmd->content))->value))->cmd))
	{
		status = exec_builtin((t_cmd *)(((t_token *)(cmd->content))->value), list);
		exit(status);
	}
}

int	execb1(t_list *cmd, t_list *list, int *get_exit, t_execve_params *ep)
{
	size_t	n_commands;
	char	*cmd_name;
	t_cmd	*command;

	n_commands = ft_lstsize(list);
	cmd_name = ((t_cmd *)(((t_token *)(cmd->content))->value))->cmd;
	if (is_builtin(cmd_name) && n_commands == 1)
	{
		free(ep->args);
		free(ep->path);
		command = (t_cmd *)(((t_token *)(cmd->content))->value);
		g_exit_status = exec_builtin(command, list); //exec in parrent
		*get_exit = 0;
		return (1);
	}
	return (0);
}

void	close_pipe(t_list *cmd, t_fd *fd)
{
	if (cmd->next)
		if (close(fd->fd[1]) == -1)
			perror("close");
	if (fd->old_fd > 0)
		if (close(fd->old_fd) == -1)
			perror("close");
}

void	wait_4_last_command(t_list *cmd, pid_t pid)
{
	if (!(cmd->next))
		if (waitpid(pid, &g_exit_status, 0) == -1)
			perror("waitpid");
}

void	command_not_found(t_list *cmd, int *get_exit)
{
	char	*p;

	p = ((t_cmd*)((t_token*)(cmd->content))->value)->cmd;
	if (write(2, "minishell: ", 11) == -1
		|| write(2, p, ft_strlen(p)) == -1
		|| write(2, ": command not found\n", 20) == -1)
		perror("write");
	*get_exit = 0;
	g_exit_status = 127;
}

void	close_open_fds(t_list *list)
{
	int	fd;

	while (list)
	{
		if (((t_cmd *)(((t_token *)(list->content))->value))->cmd_in != 0)
		{
			fd = ((t_cmd *)(((t_token *)(list->content))->value))->cmd_in;
			if (close(fd) == -1)
				perror("close");
		}
		if (((t_cmd *)(((t_token *)(list->content))->value))->cmd_out != 1)
		{
			fd = ((t_cmd *)(((t_token *)(list->content))->value))->cmd_out;
			if (close(fd) == -1)
				perror("close");
		}
		list = list->next;
	}
}

void	ft_piping(t_list *cmd, t_fd *fd)
{
	if (cmd->next)
		if (pipe(fd->fd) == -1)
			perror("pipe");
}

void	ft_forking(t_list *cmd, t_list *list, t_fd *fd, t_execve_params *ep)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		perror("fork");
	if (pid == 0)
	{
		dup_stdin_and_stdout(cmd, fd);
		dup_redirections(cmd);
		execb2(cmd, list);
		close_open_fds(list);
		exec_c(ep, g_env_s(NULL));
	}
	wait_4_last_command(cmd, pid);
}

void	execute_2(t_list *cmd, t_list *list, int *get_exit, t_fd *fd)
{
	t_execve_params	ep;

	ft_piping(cmd, fd);
	if (((t_cmd *)(((t_token *)(cmd->content))->value))->cmd)
	{
		ep.path = findcmd(((t_cmd *)((t_token *)(cmd->content))->value)->cmd);
		ep.args = get_args(cmd);
		save_cmd(&ep, g_env_s(NULL));
		if (!ep.path)
			return (free(ep.args), command_not_found(cmd, get_exit));
		if (execb1(cmd, list, get_exit, &ep))
			return ;
		ft_forking(cmd, list, fd, &ep);
		free(ep.path);
		free(ep.args);
	}
	close_pipe(cmd, fd);
}

void	get_name_of_signal(int sig)
{
	if (sig == 2)
		printf("\n");
	if (sig == 3)
		printf("Quit: 3\n");
	if (sig == 6)
		printf("Aborted\n");
	if (sig == 7)
		printf("Bus error\n");
	if (sig == 8)
		printf("Floating point exception\n");
	if (sig == 9)
		printf("\n");
	if (sig == 10)
		printf("User-defined signal 1\n");
	if (sig == 11)
		printf("Segmentation fault: 11\n");
	if (sig == 12)
		printf("User-defined signal 2\n");
	if (sig == 13)
		printf("Broken pipe\n");
	if (sig == 14)
		printf("Alarm clock\n");
	if (sig == 15)
		printf("Terminated\n");
}

void	get_exit_status()
{
	int	sig;

	if (WIFEXITED(g_exit_status))
		g_exit_status = WEXITSTATUS(g_exit_status);
	else if (WIFSIGNALED(g_exit_status))
	{
		sig = WTERMSIG(g_exit_status);
		g_exit_status = sig + 128;
		get_name_of_signal(sig);
	}
}

void	execute(t_list *list)
{
	t_list *curr_cmd;
	int		get_exit;
	t_fd	fd;

	curr_cmd = list;
	get_exit = 1;
	fd.fd[0] = -1;
	fd.fd[1] = -1;
	g_exit_status = 0;
	while (curr_cmd)
	{
		g_cmd_executing(1);
		fd.old_fd = fd.fd[0];
		execute_2(curr_cmd, list, &get_exit, &fd);
		curr_cmd = curr_cmd->next;
	}
	while (wait(NULL) > -1)
		;
	g_cmd_executing(0);
	close_open_fds(list); 
	if (get_exit)
		get_exit_status();
	ft_lstclear(&list, NULL);
}