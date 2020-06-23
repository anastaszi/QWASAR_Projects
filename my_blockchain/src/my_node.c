/****************************************************/
/* Name:  node.c                                    */
/* Created by: Ksenia Anishenko  & Anastasia Zimina */
/*                                                  */
/* Date:    3/10/2020                               */
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

t_node *searchNodes(t_node *head, int id) {

  t_node *temp = head;

  while(temp != NULL) {
    if (temp->id == id) {
      return temp;
    }
    temp = temp->next;
  }
  return NULL;
}


int addToNodes(t_node **head, int id) {
  t_node *currentNode = searchNodes(*head, id);
  t_node *newNode;

  if (currentNode) {
    return 0;
  }

  currentNode = *head;

  if ((newNode = (t_node *)malloc(sizeof(t_node)))) {
       newNode->id = id;
       newNode->next = NULL;
       newNode->blocks = NULL;

       if (*head == NULL) {
           *head = newNode;
       }
       else {
           while (currentNode->next) {
               currentNode = currentNode->next;
           }
           currentNode->next = newNode;
       }
       return 1;
  }
  return -1;
}

int removeFromNodes(t_node **head, int id) {

  t_node *currentNode = searchNodes(*head, id);

  if (!currentNode)
    return 0;

  t_node *temp = *head;
  t_node *prev;

  if (temp != NULL && temp->id == id) {
    *head = temp->next;
    my_lstdel(&(temp->blocks), my_del);
    free(temp);
    return 1;
  }

  while (temp != NULL && temp->id != id) {
    prev = temp;
    temp = temp->next;
  }

  if(temp == NULL) {
    return 0;
  }
  my_lstdel(&(temp->blocks), my_del);
  prev->next = temp->next;
  free(temp);
  return 1;
};

void removeAllNodes(t_node **nodes) {
    t_node *current = *nodes;
    t_node *next;

	while (current != NULL)
	{
		next = current->next;
		my_lstdel(&(current->blocks), my_del);
        free(current);
		current = next;
	}
	*nodes = NULL;
}

void printNodesId(t_node *node)  {
    while (node != NULL)  {
      printf("%d\n", node->id);
      node = node->next;
    }
}

int countNodes(t_node *node) {
  int count = 0;
  t_node *temp = node;
  while (temp)  {
    count++;
    temp = temp->next;
  }

  return count;
};

void manipulateNodes(t_node **nodes, char **words) {
    int result;
    int numWords = countWords(words);

    if (numWords >= 3) {
        if (!my_strcmp("add", words[1])) {
          if (my_isnum(words[2][0])) {
            result = addToNodes(nodes, my_atoi(words[2]));
            (result == 1) ? printOK() : printError((!result) ? ERR_NODE : ERR_MEM);
          } else {
            printError(ERR_NNAME);
          }
          return ;
        }
        else if (!my_strcmp("rm", words[1])) {
            int i = 2;
            if (!my_strcmp("*", words[2])) {
                removeAllNodes(nodes);
                printOK();
            }
            else
                while (i < numWords) {
                  if (my_isnum(words[i][0]))
                    (removeFromNodes(nodes, my_atoi(words[i]))) ? printOK() : printError(ERR_NNODE);
                  else
                    printError(ERR_NNAME);
                  i++;
                }
            return ;
        }
    }
    printError(ERR_COM);
}
