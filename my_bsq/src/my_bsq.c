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

static unsigned int **create_matrix(int height, int width) {
  unsigned int **matrix;
  int row, col;

  matrix = malloc(height * sizeof(int*));
  for (row = 0; row < height; row++) {
    matrix[row] = malloc(width * sizeof(int));
    for (col = 0; col < width; col++)
      matrix[row][col] = 0;
  }
  return matrix;
}

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

static void free_matrix(unsigned int **matrix, int rows) {
  int i;
  if (matrix != NULL) {
    for (i = 0; i < rows; i++)
      free(matrix[i]);
    free(matrix);
  }
}

static void free_input(struct Matrix *this) {
  free_matrix(this->input, this->height);
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
  unsigned int **matrix = NULL;
  int row, col, current_value;

  if (this->width != 0 && this->input != NULL) {
    matrix = create_matrix(this->height + 1, this->width + 1);
    for (row = 1; row < (this->height + 1); row++) {
      for (col = 1; col < (this->width + 1); col++) {
        if (get_bit(this->input[row - 1], col - 1) == 0) {
          current_value = my_min(matrix[row][col - 1],my_min(matrix[row - 1][col - 1], matrix[row - 1][col])) + 1;
          matrix[row][col] = current_value;
          if (current_value > this->squire) {
            this->squire = current_value;
            this->x = col - this->squire;
            this->y = row - this->squire;
          }
        }
      }
    }
  }
  free_matrix(matrix, this->height + 1);
}

static void fill(struct Matrix *this, char *file) {
  int col, fd, num_of_ints, line = 0;
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
      result = create_matrix(this->height, num_of_ints);
    }
    for (col = 0; col < this->width; col++) {
      if (str[col] == OBSTACLE)
        set_bit(result[line], col, 1);
      else if (str[col] != EMPTY)
        this->width = 0; // catch bad character
    }
    line++;
    free(str);
  }

  this->input = result;
  // catch bad input + less than needed number of rows
  if (line != this->height || this->height == 0)
    this->width = 0;
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
