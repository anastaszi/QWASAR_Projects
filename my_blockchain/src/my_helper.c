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

void cleanAfterSplit(char **split) {
    int i = 0;
    while (split[i]) {
        free(split[i]);
        i++;
    }
    free(split);
}

void synchronize(t_node **nodes, t_list *blocks) {
    t_node *current = *nodes;

    while (current) {
        my_lstdel(&(current->blocks), my_del);
        current->blocks = my_lstdub(blocks);
        current = current->next;
    }
}

void updateStatus(t_node **nodes, t_list **blocks, int *s, int *number) {
    int sync = 1;
    *number = countNodes(*nodes);

    t_node *tempNode = *nodes;
    while (tempNode) {

        t_list *tempBlocks = tempNode->blocks;
        sync *= (my_lstsize(*blocks) == my_lstsize(tempBlocks));
        while (tempBlocks) {
            sync *= (addBlockLst(blocks, (char *)tempBlocks->data) == -1);
            tempBlocks = tempBlocks->next;
        }
        tempNode = tempNode->next;
    }
    *s = sync;
}

void readInput(int *sync, int *num, t_list **blocks, t_node **nodes) {
    char *command;
    char **words;
    int run = 1;

    while (run) {
        printPrompt(*sync, *num);
        command = my_readline(0);
        words = my_strsplit(command);
        free(command);
        if (words) {
          if (!my_strcmp(words[0], "node")) {
              manipulateNodes(nodes, words);
              updateStatus(nodes, blocks, sync, num);
          } else if (!my_strcmp(words[0], "block")) {
              manipulateBlocks(nodes, blocks, words);
              updateStatus(nodes, blocks, sync, num);
          } else if (!my_strcmp(words[0], "ls")) {
              printInfo(*nodes, !my_strcmp(words[1], "-l"));
              printOK();
          } else if (!my_strcmp(words[0], "sync")) {
              synchronize(nodes, *blocks);
              updateStatus(nodes, blocks, sync, num);
              printOK();
          } else if (!my_strcmp(words[0], "quit")) {
              run = 0;
          } else {
            printError(ERR_COM);
          }
          cleanAfterSplit(words);
      }
      else {
        printError(ERR_COM);
      }
    }
}
