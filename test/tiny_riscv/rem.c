#include "lib.h"

int main(int argc, char** argv) {
  char buffer[20];
  signed int a = 42;
  signed int b = -9;
  signed int c = a % b;
  signed int d = c + 10;
  print_string("42 % -9 + 10 = ");
  uns_to_str(buffer, (unsigned int)d);
  print_string(buffer);

  return 0;
}
