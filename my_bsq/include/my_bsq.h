/****************************************************/
/* Name:  my_bsq.h                                  */
/* Created by: Anastasia Zimina                     */
/*                                                  */
/* Date:    8/28/2020                               */
/* Updated: 8/28/2020                               */
/*                                                  */
/* QWASAR SILICON VALLEY 2020                       */
/****************************************************/

#ifndef MY_BSQ_H
# define MY_BSQ_H
# define EMPTY '.'
# define OBSTACLE 'o'
# define SQUARE 'x'
# define READLINE_READ_SIZE 20

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

struct Matrix {
  unsigned int **input;
  int height;
  int width;
  int x;
  int y;
  int squire;
  void (* const print)(struct Matrix *this);
  void (* const solve)(struct Matrix *this);
  void (* const free_input)(struct Matrix *this);
  void (* const fill)(struct Matrix *this, char *file);
};

extern const struct MatrixClass {
  struct Matrix (*new)();
} Matrix;

// LIB FUNCTIONS

char *my_readline(int fd);
int my_atoi(const char *str);
size_t my_strlen(const char *str);
int my_min(int a, int b);
#endif
