/****************************************************/
/* Name:  blocks.c                                  */
/* Created by: Anastasia Zimina                     */
/*                                                  */
/* Date:    3/16/2020                               */
/* Updated: 3/21/2020                               */
/*                                                  */
/* QWASAR SILICON VALLEY 2020                       */
/****************************************************/

#include "my_blockchain.h"

static int countWords(char **words) {
    int count = 0;
    while (words[count])
        count++;
    return count;
}

int addBlockLst(t_list **lst , char *bid) {
    t_list *current = *lst;
    while (current) {
        if (!my_strcmp(bid, current->data))
            return -1;
        current = current->next;
    }
    if (my_lstpush_back(lst, bid, my_strlen(bid) + 1))
        return 1;
    return 0;
}

int addBlock(t_list **blocks, t_node** nodes, char *bid, int nid) {
    t_node *cnode = *nodes;

    while (cnode) {
        if (cnode->id == nid)
            break;
        cnode = cnode->next;
    }
    if (!cnode)
        return 0;
    addBlockLst(blocks, bid);
    return (addBlockLst(&cnode->blocks, bid));
}

int addBlockAll(t_list **blocks, t_node **nodes, char *bid) {
    t_node *current = *nodes;
    int result = 0;
    addBlockLst(blocks, bid);
    while (current) {
        result += addBlockLst(&(current->blocks), bid);
        current = current->next;
    }
    return result;
}

int removeBlockLst(t_list **lst, char *bid) {
    t_list *current = *lst;

    while (current) {
        if(!my_strcmp(bid, current->data)) {
            my_lstdelelem(lst, current);
            return 1;
        }
        current = current->next;
    }
    return 0;
}

int removeBlock(t_node **nodes, t_list **blocks, char *bid) {
    t_node *cnode = *nodes;
    int result = 0;


    while (cnode) {
        result += removeBlockLst(&(cnode->blocks), bid);
        cnode = cnode->next;
    }
    removeBlockLst(blocks, bid);
    return (result > 0);
}

void manipulateBlocks(t_node **nodes, t_list **blocks, char **words) {
    (void)nodes;
    int i;
    int numWords = countWords(words);
    if (numWords >= 3) {
        if (!my_strcmp("add", words[1]) && numWords > 3) {
            if (!my_strcmp("*", words[3]))
                (addBlockAll(blocks, nodes, words[2])) ? printOK() : printError(ERR_BLOCK);
            else {
                i = 3;
                while (i < numWords)
                {
                    int result = addBlock(blocks, nodes, words[2], my_atoi(words[i]));
                    (result == 1) ? printOK() : ((!result) ? printError(ERR_NNODE) : printError(ERR_BLOCK));
                    i++;
                }
            }
            return ;
        }
        else if (!my_strcmp("rm", words[1])) {
            i = 2;
            while (i < numWords) {
                (removeBlock(nodes, blocks, words[i])) ? printOK() : printError(ERR_NBLOCK);
                i++;
            }
            return ;
        }
    }
    printError(ERR_COM);
}
