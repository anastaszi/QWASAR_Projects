/****************************************************/
/* Name:  my_bsq.c                                  */
/* Created by: Anastasia Zimina                     */
/*                                                  */
/* Date:    8/28/2020                               */
/* Updated: 9/09/2020                               */
/*                                                  */
/* QWASAR SILICON VALLEY 2020                       */
/****************************************************/

#include "my_bsq.h"

static void set_bit(unsigned int *a, int index, unsigned int bit){
  int elem, pos;

  elem = index / 32;
  pos = index % 32;
  bit = bit << pos;
  a[elem] = a[elem] | bit;
}

static int get_bit(unsigned int *a, int index) {
  int elem, pos;
  unsigned int mask;

  elem = index / 32;
  pos = index % 32;
  mask = 1 << pos;
  return ((mask & a[elem]) != 0);
}

static void free_matrix(int **matrix, int rows) {
  int i;
  if (matrix != NULL) {
    for (i = 0; i < rows; i++)
      free(matrix[i]);
    free(matrix);
  }
}

static void free_input(struct Matrix *this) {
  free_matrix((int **)this->input, this->height);
}

static void print(struct Matrix *this) {
  int i, j, current;
  int min_y = this->y;
  int max_y = this->y + this->squire;
  int min_x = this->x;
  int max_x = this->x + this->squire;
  if (this->width != 0 && this->input != NULL) {
    for (i = 0; i < this->height && this->width != -1; i++) {
      for (j = 0; j < this->width; j++) {
        current = get_bit(this->input[i], j);
        if (current != 0)
          printf("%c", OBSTACLE);
        else if (i >= min_y && i < max_y && j >= min_x && j < max_x)
          printf("%c", SQUARE);
        else
          printf("%c", EMPTY);
      }
      printf("\n");
    }
  }
  else
    printf("BAD INPUT. TRY AGAIN:)\n");
}

static void solve(struct Matrix *this) {
  int **matrix = NULL;
  int i, j;
  if (this->width != 0 && this->input != NULL) {
    matrix = malloc((this->height + 1) * sizeof(int*));
    matrix[0] = malloc((this->width + 1) * sizeof(int));
    for (i = 0; i < this->width + 1; i++)
      matrix[0][i] = 0;
    for (i = 1; i < (this->height + 1); i++) {
      matrix[i] = malloc((this->width + 1) * sizeof(int));
      matrix[i][0] = 0;
      for (j = 1; j < (this->width + 1); j++) {
        if (get_bit(this->input[i - 1], j - 1) != 0)
          matrix[i][j] = 0;
        else {
          matrix[i][j] = my_min(matrix[i][j - 1],my_min(matrix[i - 1][j - 1], matrix[i - 1][j])) + 1;
          if (matrix[i][j] > this->squire) {
            this->squire = matrix[i][j];
            this->x = j - this->squire;
            this->y = i - this->squire;
          }
        }
      }
    }
  }
  /*
  printf("(%d, %d) - width %d\n", this->x, this->y, this->squire);
  for (i = 0; i < this->height + 1; i++) {
      for (j = 0; j < this->width + 1; j++) {
        printf("%d", matrix[i][j]);
      }
      printf("\n");
  }*/
  free_matrix(matrix, this->height + 1);
}

static void fill(struct Matrix *this, char *file) {
  int i, fd, num_of_ints, line = 0;
  char* str = NULL;
  unsigned int **result = NULL;

  fd = open (file, O_RDONLY);
  str = my_readline(fd);
  this->height = my_atoi(str);
  free(str);

  while ((str = my_readline(fd))) {
    if (this->width == -1) {
      this->width = my_strlen(str);
      num_of_ints = this->width / 32 + 1;
      result = malloc(this->height * sizeof(int*));
    }
    result[line] = malloc(num_of_ints * sizeof(int));
    for (i = 0; i < num_of_ints; i++)
      result[line][i] = 0;
    for (i = 0; i < this->width; i++) {
      if (str[i] == OBSTACLE)
        set_bit(result[line], i, 1);
      else if (str[i] != EMPTY)
        this->width = 0; // catch bad character
    }
    line++;
    free(str);
  }

  this->input = result;
  // catch bad input + less than needed number of rows
  if (line != this->height || this->height == 0) {
    this->width = 0;
    this->height = line;
  }
  close(fd);
}

static struct Matrix new() {
  return (struct Matrix){
    .input = NULL,
    .height = -1,
    .width = -1,
    .x = -1,
    .y = -1,
    .squire = 0,
    .print = &print,
    .solve = &solve,
    .free_input = &free_input,
    .fill = &fill,
  };
}

const struct MatrixClass Matrix = {
  .new = &new,
};

int main(int ac, char **av){
  struct Matrix matrix = Matrix.new();
  if (ac == 2)
    matrix.fill(&matrix, av[1]);
  //matrix.print(&matrix);
  matrix.solve(&matrix);
  matrix.print(&matrix);
  matrix.free_input(&matrix);
  return 0;
}
