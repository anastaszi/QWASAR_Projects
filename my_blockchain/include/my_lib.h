/****************************************************/
/* Name:  my_lib.h                                  */
/* Created by: Anastasia Zimina                     */
/*                                                  */
/* Date:    3/10/2020                               */
/* Updated: 3/21/2020                               */
/*                                                  */
/* QWASAR SILICON VALLEY 2020                       */
/****************************************************/

#ifndef MY_LIB_H
# define MY_LIB_H
# include <unistd.h>
# include <stdlib.h>
# include <stddef.h>

# define READLINE_READ_SIZE 512

typedef struct s_list {
    void *data;
    size_t dataSize;
    struct s_list *next;
} t_list;

t_list *my_lstnew(void *data, size_t dataSize);
t_list* my_lstpush_back(t_list **lst, void *data, size_t dataSize);
void my_lstdelelem(t_list **lst, t_list *elem);
void my_lstdelone(t_list **lst, void (*del)(void *, size_t size));
void my_lstprint(t_list *head);
void my_lstdel(t_list **head, void (*del)(void *, size_t size));
t_list *my_lstdub(t_list *src);
size_t my_lstsize(t_list *a);

void my_strcat(char* dst, const char *src, size_t size);
int my_strchr(char*str, char a, size_t size);
size_t my_strlen(const char *str);
char	*my_strsub(char const *s, size_t start, size_t len);
int my_atoi(const char *str);
char** my_strsplit(char *str);
size_t my_strwordlenght(char *str);
size_t my_strnumwords(char *str);
size_t my_strcmp(const char* a, const char *b);
char *my_strnadd(char *dst, char *src, size_t len);

int my_isspace(char c);
int my_isnum(char c);

void *my_memalloc(size_t dataSize);
void my_memset(void *data, int c, size_t size);
void my_memcpy(void *dst, const void *src, size_t dataSize);
void my_memdel(void **data);
void my_del(void *data, size_t size);

char *my_readline(int fd);
void	my_putnbr_fd(int n, int fd);
void	my_putnbr(int n);
void	my_putstr_fd(char const *s, int fd);
void    my_putstr(char const *s);
void	my_putchar_fd(int c, int fd);
void    my_putchar(int c);
#endif
