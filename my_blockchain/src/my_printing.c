/****************************************************/
/* Name:  my_helper.c                               */
/* Created by: Ksenia Anishenko & Anastasia Zimina  */
/*                                                  */
/* Date:    3/15/2020                               */
/* Updated: 3/21/2020                               */
/*                                                  */
/* QWASAR SILICON VALLEY 2020                       */
/****************************************************/

#include "my_blockchain.h"

void printInfo(t_node *head, int blocks) {
    t_node *current = head;

    while (current) {
        printf("\033[1;34m%d\033[0m", current->id);
        if (blocks) {
            t_list *block = current->blocks;
            while (block) {
                printf(" %s", (char *)block->data);
                block = block->next;
            }
        }
        printf("\n");
        current = current->next;
    }
}

void printError(char *str) {
    printf("\033[1;31mnok:\033[0m %s\n", str);
}

void printOK() {
    printf("\033[1;32mok\033[0m\n");
}

void printPrompt(int sync, int number) {
    (sync) ? my_putstr("\033[1;33m[s") : my_putstr("\033[1;33m[-");
    my_putnbr(number);
    my_putstr("]>\033[0m ");
}
