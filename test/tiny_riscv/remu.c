#include "lib.h"

int main(int argc, char** argv) {
  char buffer[20];
  unsigned int a = 42;
  unsigned int b = 9;
  unsigned int c = a % b;
  print_string("42 % 9 = ");
  uns_to_str(buffer, c);
  print_string(buffer);

  return 0;
}
