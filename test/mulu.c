#include "lib.h"

int main(int argc, char** argv) {
  char buffer[20];
  unsigned int a = 6;
  unsigned int b = 7;
  unsigned int c = a * b;
  print_string("unsigned 6 * 7 = ");
  uns_to_str(buffer, a);
  print_string(buffer);

  return 0;
}
