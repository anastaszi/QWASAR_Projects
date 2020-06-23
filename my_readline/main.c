#include "my_readline.c"
#include <stdio.h>
#include <fcntl.h>

#define READLINE_READ_SIZE 11
void test_1() {
  int fd;
  char* str;

  printf("<1>\n");
  system("echo 'line1' >> file.txt");
  system("echo 'line2' >> file.txt");
  system("echo 'line3' >> file.txt");
  system("echo 'line4' >> file.txt");
  system("echo 'line5' >> file.txt");
  system("echo 'line6' >> file.txt");
  system("echo 'line7' >> file.txt");
  fd = open ("file.txt", O_RDONLY);
  while ((str = my_readline(64564)) != NULL) {
    printf("my_readline: [%s]\n", str);
  }
  close(fd);
  printf("Output should be line1 -> line7\n");
  printf("</1>\n");
}

int main() {
  test_1();
  //
  //  Yes it's also working with stdin :-)
  //  printf("%s", my_readline(0));
  //
  return 0;
}
