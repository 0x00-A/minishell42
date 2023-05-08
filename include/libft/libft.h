#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>

typedef struct	s_list
{
	void		    *content;
	struct s_list     *next;	
}t_list;

//list 
int	    ft_lstsize(t_list *lst);
t_list	*ft_lstnew(void *content);
void	ft_lstclear(t_list **lst, void (*del)(void*));
void	ft_lstadd_back(t_list **lst, t_list *new);
char	*ft_strjoin(char *s1, char *s2);
void	ft_lstadd_front(t_list **lst, t_list *new);\
void	ft_lstadd_in_index(t_list **list, t_list *new, int index);
void	ft_lstdelete_index(t_list **list, int index, void (*del)(t_list **));
void	ft_lstfree_node(t_list **node);

// split
char	*ft_strdup(const char *s1);
size_t	ft_strlen(const char *s);
char	**ft_split(char const *s, char c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
void	*ft_memset(void *b, int c, size_t len);
void	*ft_realloc(void *ptr, size_t size);
void	ft_strncat(char *s1, const char *s2, size_t n);
char	**ft_t_strdup(char **str);
int		ft_t_strlen(char **str);
// character checker
int		ft_isdigit(int c);
int		ft_isalpha(int c);
int		ft_isalnum(int c);
int		ft_isprint(int c);
int		iswhitespace(char c);
int		str_iswhitespaced(char *str);
//
int	ft_strcmp(const char *s1, const char *s2);
// get next line 
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 5
# endif
char	*get_next_line(int fd);
size_t	ft_strlen_nln(char *str);
void	*ft_calloc(size_t count);
void	clean(char *s);
void	*free_rest(char **rest);

#endif