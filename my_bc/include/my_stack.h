#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

struct Stack {
  void **array;
  size_t *dataSize_array;
  size_t stack_size;
  int last_elem;

  void (* const increase_size)(struct Stack *this);
  void (* const add)(struct Stack *this, void *data, size_t dataSize);
  void (* const pop)(struct Stack *this);
  void *(* const top)(struct Stack *this);
  int (* const empty)(struct Stack *this);
};

extern const struct StackClass {
  struct Stack (*new)(int);
} Stack;
