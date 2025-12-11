#include "lib.h"
#include <stdint.h>

int rem(int32_t a, int32_t b) {
  int32_t result;
  asm volatile(
    "rem %0, %1, %2"
    :   "=r" (result)
    :   "r" (a), "r" (b)
    );
  return result;
}

int main(int argc, char** argv) {
  char buffer[30];
  int d = rem(42, 9);

  print_string("42 % 9 = ");
  uns_to_str(buffer, d);
  print_string(buffer);

  return 0;
}
