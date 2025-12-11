#include "lib.h"

int main(int argc, char** argv) {
  char buffer[20];
  int a = 5 * 5;
  print_string("5 * 5 = ");
  uns_to_str(buffer, a);
  print_string(buffer);

  return 0;
}
