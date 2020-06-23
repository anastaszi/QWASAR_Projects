/****************************************************/
/* Name:  my_blockchain.h                           */
/* Created by: Ksenia Anishenko & Anastasia Zimina  */
/*                                                  */
/* Date:    3/10/2020                               */
/* Updated: 3/21/2020                               */
/*                                                  */
/* QWASAR SILICON VALLEY 2020                       */
/****************************************************/

#ifndef MY_BLOCKCHAIN_H
# define MY_BLOCKCHAIN_H
# include <stdio.h>
# include <fcntl.h>
# include "my_lib.h"
# define ERR_NODE "this node already exists"
# define ERR_NNODE "node doesn't exist"
# define ERR_BLOCK "this block already exists"
# define ERR_NBLOCK "block doesn't exist"
# define ERR_MEM "no more resources available on the computer"
# define ERR_COM "command not found"
# define ERR_NNAME "bad node name"

typedef struct Node {
    int id;
    struct Node *next;
    t_list *blocks;
} t_node;

void readInput(int *sync, int *num, t_list **blocks, t_node **nodes);

void readBackup(char *file, int *sync, int* number, t_list **block, t_node **node);
void updateBackup(t_node *nodes, int sync, int numberOfNodes, char *file);

void printError(char *str);
void printOK();
void printInfo(t_node *head, int blocks);
void printPrompt(int sync, int number);

int addBlock(t_list **blocks, t_node** nodes, char *bid, int nid);
int addBlockLst(t_list **lst , char *bid);
int removeBlock(t_node **nodes, t_list **blocks, char *bid);
void manipulateBlocks(t_node **nodes, t_list **blocks, char **words);

void cleanAfterSplit(char **split);

int addToNodes(t_node **head, int id);
int addBlockAll(t_list **blocks, t_node **nodes, char *bid);
int removeFromNodes(t_node **head, int id);
void removeAllNodes(t_node **nodes);
int countNodes(t_node *node);
void manipulateNodes(t_node **nodes, char **words);


#endif
