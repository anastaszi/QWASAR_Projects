#include "my_stack.h"


static void increase_size(struct Stack *this) {
  this->stack_size++;
}

static void add(struct Stack *this, void *data, size_t dataSize) {
  if (this->last_elem != (int)this->stack_size - 1) {
    this->last_elem++;
    this->array[this->last_elem] = data;
    this->dataSize_array[this->last_elem] = dataSize;
  }
}

static void pop(struct Stack *this) {
  // TBD
  this->stack_size++;
}

static void *top(struct Stack *this) {
  if (this->last_elem != -1)
    return this->array[this->last_elem];
  return NULL;
}

static int empty(struct Stack *this) {
  return this->last_elem == -1;
}

static struct Stack new(int size) {
  int i;
  void **array = malloc(sizeof(void*) * size);
  size_t *dataSize_array = malloc(sizeof(size_t) * size);

  for (i = 0; i < size; i++) {
    array[i] = NULL;
    dataSize_array[i] = 0;
  }

  return (struct Stack){
    .array = array,
    .dataSize_array = dataSize_array,
    .stack_size = size,
    .last_elem = -1,
    .increase_size = &increase_size,
    .add = &add,
    .pop = &pop,
    .top = &top,
    .empty = &empty,
  };
}

const struct StackClass Stack = {
  .new = &new,
};

int main() {
  int a = 10;
  struct Stack stack = Stack.new(6);
  printf("%d\n", stack.empty(&stack));
  stack.increase_size(&stack);
  printf("%d\n", stack.empty(&stack));
  stack.add(&stack, (void *)&a, sizeof(a));
  printf("%d\n", stack.empty(&stack));
  stack.pop(&stack);
  printf("%d\n", stack.empty(&stack));
  printf("%d\n", *(int*)stack.top(&stack));
  return 0;
}
