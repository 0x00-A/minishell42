/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsaktiwy <hsaktiwy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 13:26:20 by aigounad          #+#    #+#             */
/*   Updated: 2023/05/24 14:24:19 by hsaktiwy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env *g_env_s(t_env *env)
{
	static t_env *g_env;
	if (env == NULL)
		return (g_env);
	else
		g_env = env;
	return (NULL);
}

static t_holder	*ft_lstnewholder(char *key_val, char *pt_equal)
{
    // char		**res;
    t_holder	*holder;

	holder = (t_holder *)malloc(sizeof(t_holder));
	if (!holder)
		return (perror("malloc"), NULL);
	if (key_val)
	{
		holder->key = ft_substr(key_val, 0,pt_equal - key_val);
		if (!pt_equal)
			holder->value = NULL;
		else
			holder->value = ft_substr(key_val, pt_equal - key_val + 1, ft_strlen(pt_equal + 1));
		return (holder);
	}
	else
		return (free(holder), holder = NULL, NULL);
}

t_list	*ft_lst_list_holder(char **env)
{
	t_list		*list;
	int			i;
	t_holder	*holder;
	char 		*pt_equal;

	i = -1;
	list = NULL;
	while (env[++i])
	{
		pt_equal = ft_strchr(env[i],'=');
		holder = ft_lstnewholder(env[i], pt_equal);
		if (holder)
			ft_lstadd_back(&list, ft_lstnew(holder));
	}
	return (list);
}

void	ft_declare_envs(t_env *env)
{
	char	path[4096];

	ft_setenv(&env, "OLDPWD", NULL);
	getcwd(path, 4096);
	ft_setenv(&env, "PWD", path);
	ft_setenv(&env, "_", "minishell");
}

void	ft_unset_oldpwd(t_env **env)
{
	ssize_t	index;

	ft_unset_env_list(&((*env)->l_env), "OLDPWD");
	index = get_env_index((*env)->env, "OLDPWD");
	if (index >= 0)
		ft_unset_env_table((*env)->env, index);
	ft_setenv(env, "OLDPWD", NULL);
}

void	ft_change_shlvl(t_env **env)
{
	char	*shlvl;
	int		level;
	char	*tmp;

	level = 0;
	shlvl = ft_getenv(*env, "SHLVL");
	if (!shlvl)
		ft_setenv(env, "SHLVL", "1");
	else
	{
		level = ft_atoi(shlvl);
		level++;
		if (level > 999)
		{
			write(2, "minishell: warning: shell level (", 33);
			ft_putnbr_fd(level, 2);
			write(2, ") too high, resetting to 1\n", 27);
			level = 1;
		}
		else if (level < 1)
			level = 0;
		tmp = ft_itoa(level);
		ft_setenv(env, "SHLVL", tmp);
		free(tmp);
	}
}

t_env	*ft_init_env(char **env)
{
	t_env	*env_l;
	size_t	size;

	env_l = (t_env *)malloc(sizeof(t_env));
	if (!env_l)
		return (perror("malloc"), NULL);
	env_l->l_env = NULL;
	if (!env || !*env)
	{
		env_l->env = malloc(sizeof(char *));
		*(env_l->env) = NULL;
		ft_declare_envs(env_l);
	}
	else
	{
		size = ft_t_strlen(env);
		env_l->env = ft_t_strdup(env, size);

		env_l->l_env = ft_lst_list_holder(env);
		ft_unset_oldpwd(&env_l);
	}
	ft_change_shlvl(&env_l);
	g_env_s(env_l);
	return (env_l);
	
}

// void leak_func()
// {
// 	system("leaks a.out");
// }

// int main(int argc, char **arg, char **env)
// {
// 	t_env *list;
// 	atexit(leak_func);
// 	list = ft_init_env(env);
// 	if (list)
// 	{
// 		int i;

// 		i = -1;
// 		while(list->env[++i])
// 			printf("%s\n", list->env[i]);
// 		t_list *linked = list->l_env;
// 		t_holder *holder;
// 		while (linked)
// 		{
// 			holder = linked->content;
// 			printf("Key = %s : Value = %s\n", holder->key, holder->value);
// 			linked = linked->next;
// 		}
// 		printf("\n\n\n");
// 		printf("res of getenv PATH: %s\n", ft_getenv(list, "PATH"));
// 		printf("res of getenv a: %s\n", ft_getenv(list, "a"));
// 		ft_setenv(&list, "a", "HAHAHA");
// 		printf("res of setenv a: %s\n", ft_getenv(list, "a"));
		
// 	}
// 	ft_free_env(&list);
// 	return (0);
// }