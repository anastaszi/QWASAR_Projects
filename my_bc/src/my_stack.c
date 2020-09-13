#include "my_stack.h"

static void increase_size(struct Stack *this) {
  size_t current_size = this->stack_size;
  size_t counter;
  void **array;
  size_t *dataSize_array;

  this->stack_size = current_size * 2;
  array = malloc(sizeof(void*) * this->stack_size);
  dataSize_array = malloc(sizeof(size_t) * this->stack_size);
  for( counter = 0; counter < this->stack_size; counter++) {
    if (counter < current_size) {
      array[counter] = this->array[counter];
      dataSize_array[counter] = this->dataSize_array[counter];
    } else {
      array[counter] = NULL;
      dataSize_array[counter] = 0;
    }
  }
  free(this->array);
  free(this->dataSize_array);
  this->array = array;
  this->dataSize_array = dataSize_array;
}

static void add(struct Stack *this, void *data, size_t dataSize) {
  if (this->last_elem == (int)this->stack_size - 1)
    this->increase_size(this);
  this->last_elem++;
  this->array[this->last_elem] = data;
  this->dataSize_array[this->last_elem] = dataSize;
}

static void pop(struct Stack *this, void (*del)(void *, size_t size)) {
  if (this->last_elem >= 0 && del != NULL) {
    del(this->array[this->last_elem], this->dataSize_array[this->last_elem]);
    this->last_elem--;
  }
}

static void *top(struct Stack *this) {
  if (this->last_elem != -1)
    return this->array[this->last_elem];
  return NULL;
}

static int empty(struct Stack *this) {
  return (this->last_elem == -1);
}

static void clean(struct Stack *this) {
  (void) this;
}

static struct Stack new(int size) {
  int counter;
  void **array = malloc(sizeof(void*) * size);
  size_t *dataSize_array = malloc(sizeof(size_t) * size);

  for (counter = 0; counter < size; counter++) {
    array[counter] = NULL;
    dataSize_array[counter] = 0;
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
    .clean = &clean,
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
  stack.pop(&stack, NULL);
  printf("%d\n", stack.empty(&stack));
  printf("%d\n", *(int*)stack.top(&stack));
  return 0;
}
