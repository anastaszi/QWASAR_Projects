/****************************************************/
/* Name:  my_backup.c                               */
/* Created by: Anastasia Zimina                     */
/*                                                  */
/* Date:    3/16/2020                               */
/* Updated: 3/21/2020                               */
/*                                                  */
/* QWASAR SILICON VALLEY 2020                       */
/****************************************************/

#include "my_blockchain.h"


void updateBackup(t_node *nodes, int sync, int numberOfNodes, char *file) {
    int fd = open(file, O_WRONLY|O_CREAT|O_TRUNC, 0644);
    t_node *tempNode = nodes;
    t_list *tempBlock;
    if (fd < 0)
        printError("Problems with opening/creating a backup file. No backup was saved.");

    my_putnbr_fd(sync, fd);
    my_putchar_fd('\n', fd);
    my_putnbr_fd(numberOfNodes, fd);
    my_putchar_fd('\n', fd);
    while (tempNode) {
        my_putnbr_fd(tempNode->id, fd);
        tempBlock = tempNode->blocks;
        while (tempBlock) {
            my_putchar_fd(' ', fd);
            my_putstr_fd((char *)tempBlock->data, fd);
            tempBlock = tempBlock->next;
        }
        my_putchar_fd('\n', fd);
        tempNode = tempNode->next;
    }
    close(fd);
}


void readBackup(char *file, int *sync, int* number, t_list **block, t_node **node) {
    int fd = open(file, O_RDONLY);
    char **data;
    char *line;
    int id;

    if (fd == -1) {
        printError("File doesn't exist. No backup was loaded.");
        return;
    }

    if ((line = my_readline(fd))) {
        *sync = my_atoi(line);
        free(line);
    }
    if ((line = my_readline(fd))) {
        *number = my_atoi(line);
        free(line);
    }
    while ((line = my_readline(fd))) {
        int i = 1;
        data = my_strsplit(line);
        id = my_atoi(data[0]);
        addToNodes(node, id);
        while (data[i]) {
            addBlock(block, node, data[i], id);
            i++;
        }
        cleanAfterSplit(data);
        free(line);
    }
}
