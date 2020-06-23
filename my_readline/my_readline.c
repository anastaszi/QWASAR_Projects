/****************************************************/
/* Name:  my_readline.c                                  */
/* Created by: Anastasia Zimina                     */
/*                                                  */
/* Date:    3/20/2020                               */
/* Updated: 5/24/2020                               */
/*                                                  */
/* QWASAR SILICON VALLEY 2020                       */
/****************************************************/
# include <unistd.h>
# include <stdlib.h>

# define READLINE_READ_SIZE 512

typedef struct s_list {
    void *data;
    size_t dataSize;
    struct s_list *next;
} t_list;

// LIB_C FUNCTIONS till line 371
void my_memset(void *data, int c, size_t size)
{
    unsigned char *temp;
    temp = data;

    while (size > 0) {
        *temp++ = (unsigned char)c;
        size--;
    }
}

void my_memcpy(void *dst, const void *src, size_t dataSize) {
    const unsigned char *tsrc = src;
    unsigned char *tdst = dst;

    size_t i = 0;
    while (i < dataSize) {
        tdst[i] = tsrc[i];
        i++;
    }
}

// STR FUNCTIONS

int my_strchr(char*str, char a, size_t size) {
    size_t i = 0;
    while (i < size) {
        if (str[i] == a)
            return i;
        i++;
    }
    return -1;
}

size_t my_strlen(const char *str) {
    size_t len = 0;
    if (str != NULL)
        while (*str++)
            len++;
    return len;
}

char *my_strnadd(char *dst, char *src, size_t len) {
    size_t len_dst = my_strlen(dst);
    size_t len_src = my_strlen(src);
    char *result = NULL;
    int i = 0;
    size_t j = 0;

    if (dst == NULL && src == NULL)
        return NULL;
    if ((result = malloc(sizeof(char) * (len_dst + len + 1)))) {
        while (j < len_dst) {
            result[i] =dst[j];
            j++;
            i++;
        }
        j = 0;
        while (j < len && j < len_src) {
            result[i] = src[j];
            i++;
            j++;
        }
        result[i] = '\0';
        if (dst != NULL)
            free(dst);
        return result;
    }
    return NULL;
}

// MY_READLINE FUNCTIONS

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
