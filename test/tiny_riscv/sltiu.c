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

int sltiu(uint32_t a, uint32_t imm) {
  uint32_t result;
  asm volatile(
    "sltiu %0, %1, %2"
    :   "=r" (result)
    :   "r" (a), "r" (imm)
    );

  return result;
}

int slti(int32_t a, int32_t b) {
  int32_t result;

  asm volatile(
    "slti %0, %1, %2"
    :   "=r" (result)
    :   "r" (a), "r" (b)
    );
  return result;
}

int sh_lh(int16_t a) {
  int16_t result;
  char array[10];
  asm volatile(
    "sh %0, 0(%1)"
    :
    :   "r" (a), "r" (array)
    :   "memory"
    );
  asm volatile(
    "lh %0, 0(%1)"
    :   "=r" (result)
    :   "r" (array)
    :
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
