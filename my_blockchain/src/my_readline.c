/****************************************************/
/* Name:  my_readline.c                             */
/* Created by: Anastasia Zimina                     */
/*                                                  */
/* Date:    3/10/2020                               */
/* Updated: 3/14/2020                               */
/*                                                  */
/* QWASAR SILICON VALLEY 2020                       */
/****************************************************/

#include "my_lib.h"

int countSize(t_list *lst) {
    t_list *current = lst;
    size_t lineSize = 0;

    while(current) {
        lineSize += current->dataSize;
        current = current->next;
    }
    return lineSize;
}

void create_left(char *temp, size_t size, int index, char *left) {
    size_t i = 0;
    while (index + i< size) {
        left[i] = temp[index + i];
        i++;
    }
}

char *str_from_left(char *str, int index) {
    char *line = (char *)malloc(sizeof(char) * (index + 1));
    int i = 0;
    my_memset(line, '\0', index + 1);
    my_memcpy(line, str, (size_t)index);
    while (index + 1 + i < READLINE_READ_SIZE) {
       str[i] = str[i + index + 1];
       i++;
    }
    while (i < READLINE_READ_SIZE) {
        str[i] = 0;
        i++;
    }
    return (line);
}

char *my_readline(int fd) {
    char *result = NULL;
    static char left[READLINE_READ_SIZE] = "";
    char temp[READLINE_READ_SIZE];
    int charCount = 0;
    int nl;

    if (fd == -1)
        return NULL;

    if (my_strlen(left)) {
        if ((nl = my_strchr(left, '\n', my_strlen(left))) != -1)
            return (str_from_left(left, nl));
        result = my_strnadd(result, left, my_strlen(left));
        my_memset(&left, 0, READLINE_READ_SIZE);
    }

    while ((charCount = read(fd, temp, READLINE_READ_SIZE)) > 0) {
        if ((nl = my_strchr(temp, '\n',charCount)) != -1)
        {
            result = my_strnadd(result, temp, nl);
            create_left(temp, charCount, nl + 1, left);
            break;
        }
        else
            result = my_strnadd(result, temp, (size_t)charCount);
        my_memset(&temp, 0, READLINE_READ_SIZE);
    }
    return result;
}
