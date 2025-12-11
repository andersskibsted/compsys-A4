#include "lib.h"
#include <stdint.h>

typedef int (*BinaryOp)(int, int);
typedef int (*UnaryOp)(int);

void print_result(int a, int b, BinaryOp func) {
  char buffer[10];
  int result = func(a, b);

  uns_to_str(buffer, result);
  print_string(buffer);
  print_string("\n");

}

void print_unary_result(int a, UnaryOp func) {
  char buffer[10];
  int result = func(a);

  uns_to_str(buffer, result);
  print_string(buffer);
  print_string("\n");
}

void print_signed_as_unsigned(int a) {

  char buffer[50];
  unsigned int unsigned_a = (unsigned int) a;
  uns_to_str(buffer, unsigned_a);
  print_string(buffer);
  print_string("\n");

}

int rem(int32_t a, int32_t b) {
  int32_t result;
  asm volatile(
    "rem %0, %1, %2"
    :   "=r" (result)
    :   "r" (a), "r" (b)
    );
  return result;
}

int sltiu(int a) {
  unsigned int result = 0;
  unsigned int unsigned_a = (unsigned int) a;
  asm volatile(
    "sltiu %0, %1, 100"
    :   "=r" (result)
    :   "r" (unsigned_a)
    );

  return result;
}

int slti(int a) {
  int result = 0;

  asm volatile(
    "slti %0, %1, -100"
    :   "=r" (result)
    :   "r" (a)
    );

  return result;
}

int sh_lh(int a) {
  signed short result = 0;
  char array[10];
  signed short short_a = (signed short) a;
  asm volatile(
    "sh %0, 0(%1)"
    :
    :   "r" (short_a), "r" (array)
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

int sh_lhu(int a) {
  short result = 0;
  char array[10];
  unsigned short unsigned_short_a = (unsigned short) a;
  asm volatile(
    "sh %0, 0(%1)"
    :
    :   "r" (unsigned_short_a), "r" (array)
    :   "memory"
    );
  asm volatile(
    "lhu %0, 0(%1)"
    :   "=r" (result)
    :   "r" (array)
    :
    );

  return result;
}

int mul(int a, int b) {
  int result = 0;

  asm volatile(
    "mul %0, %1, %2"
    :   "=r" (result)
    :   "r" (a), "r" (b)
    );
  return result;
}

int main(int argc, char** argv) {
  // Test REM
  char buffer[5];
  /* int rem_result = rem(42, 9); */
  print_string("42 % 9 = ");
  /* uns_to_str(buffer, rem_result); */
  /* print_string(buffer); */
  /* print_string("\n"); */

  print_result(42, 9, rem);

  /* rem_result = rem(-42, 9); */
  print_string("-42 % 9 = ");
  /* uns_to_str(buffer, rem_result); */
  /* print_string(buffer); */
  /* print_string("\n"); */
  print_result(-42, 9, rem);
  int minus6 = -6;
  unsigned int unsigned_minus_6 = (unsigned int) minus6;
  uns_to_str(buffer, unsigned_minus_6);
  print_string(buffer);
  print_string("\n");

  /* rem_result = rem(42, -9); */
  print_string("42 % -9 = ");
  print_result(42, -9, rem);
  /* uns_to_str(buffer, rem_result); */
  /* print_string(buffer); */
  /* print_string("\n"); */


  // Test SLTIU
  /* char buffer2[5]; */
  /* uint32_t sltiu_result = sltiu(8); */
  print_string("8 < 100 is ");
  /* uns_to_str(buffer, sltiu_result); */
  /* print_string(buffer); */
  /* print_string("\n"); */
  print_unary_result(8, sltiu);

  /* sltiu_result = sltiu(101); */
  print_string("101 < 100 is ");
  /* uns_to_str(buffer, sltiu_result); */
  /* print_string(buffer); */
  /* print_string("\n"); */
  print_unary_result(101, sltiu);

  // Test SLTI
  /* uint32_t slti_result = slti(-200); */
  print_string("-200 < -100 is ");
  /* uns_to_str(buffer, slti_result); */
  /* print_string(buffer); */
  /* print_string("\n"); */
  print_unary_result(-200, slti);

  // Test SH LH
  print_string("Storing halfword 0x5678 / 22136 and loading it to be: ");
  print_unary_result(22136, sh_lh);
  // Testing with word to be truncated
  print_string("Storing word 0x12345678 / 305419896 and loading it to be 22136: ");
  print_unary_result(305419896, sh_lh);
  // Testing with negative word
  print_string("Storing word -0x12345678 / -305419896 and loading it to be -22136: ");
  print_unary_result(-305419896, sh_lh);
  int minus22136 = -22136;
  unsigned int unsigned_minus_22136 = (unsigned int) minus22136;
  print_signed_as_unsigned(minus22136);
  /* uns_to_str(buffer, unsigned_minus_22136); */
  /* print_string(buffer); */
  /* print_string("\n"); */
  // Testing with negative halfword
  print_string("Storing word -22136 and loading it to be: ");
  print_unary_result(-22136, sh_lh);
  print_signed_as_unsigned(-22136);
  /* uns_to_str(buffer, unsigned_minus_22136); */
  /* print_string(buffer); */
  /* print_string("\n"); */


  // Test SH LHU
  print_string("Storing halfword 0x5678 / 22136 and loading it to be: ");
  print_unary_result(22136, sh_lhu);
  // Testing with word to be truncated
  print_string("Storing word 0x12345678 / 305419896 and loading it to be 22136: ");
  print_unary_result(305419896, sh_lhu);

  // Testing RV32M
  // Testing MUL
  print_string("12 * 56 = ");
  print_result(12, 56, mul);

  print_string("-12 * 56 = ");
  print_result(-12, 56, mul);
  print_signed_as_unsigned(-672);
  print_string("12 * -56 = ");
  print_result(12, -56, mul);
  print_signed_as_unsigned(-672);
  print_string("-12 * -56 = ");
  print_result(-12, -56, mul);

  return 0;
}
