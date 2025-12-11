#include "lib.h"

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
// RV32M
// MUL
int mul(int a, int b) {
  int result = 0;

  asm volatile(
    "mul %0, %1, %2"
    :   "=r" (result)
    :   "r" (a), "r" (b)
    );
  return result;
}

int mulh(int a, int b) {
  int result = 0;

  asm volatile(
    "mulh %0, %1, %2"
    :   "=r" (result)
    :   "r" (a), "r" (b)
    );
  return result;
}

int mulhu(int a, int b) {
  int result = 0;

  asm volatile(
    "mulhu %0, %1, %2"
    :   "=r" (result)
    :   "r" (a), "r" (b)
    );
  return result;
}


int mulhsu(int a, int b) {
  int result = 0;

  asm volatile(
    "mulhsu %0, %1, %2"
    :   "=r" (result)
    :   "r" (a), "r" (b)
    );
  return result;
}

int div(int a, int b) {
  int result = 0;

  asm volatile(
    "div %0, %1, %2"
    :   "=r" (result)
    :   "r" (a), "r" (b)
    );

  return result;
}

int divu(int a, int b) {
  unsigned int result = 0;

  asm volatile(
    "divu %0, %1, %2"
    :   "=r" (result)
    :   "r" (a), "r" (b)
    );

  return result;
}

int rem(int a, int b) {
  int result;
  asm volatile(
    "rem %0, %1, %2"
    :   "=r" (result)
    :   "r" (a), "r" (b)
    );
  return result;
}


int remu(int a, int b) {
  unsigned int result;
  asm volatile(
    "remu %0, %1, %2"
    :   "=r" (result)
    :   "r" (a), "r" (b)
    );
  return result;
}

int main(int argc, char** argv) {
  // Test REM
  /* char buffer[5]; */


  // Test SLTIU
  print_string("8 < 100 is ");
  print_unary_result(8, sltiu);

  print_string("101 < 100 is ");
  print_unary_result(101, sltiu);

  // Test SLTI
  print_string("-200 < -100 is ");
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
  /* int minus22136 = -22136; */
  /* unsigned int unsigned_minus_22136 = (unsigned int) minus22136; */
  print_signed_as_unsigned(-22136);
  // Testing with negative halfword
  print_string("Storing word -22136 and loading it to be: ");
  print_unary_result(-22136, sh_lh);
  print_signed_as_unsigned(-22136);


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

  // Testing MULH
  print_string("Testing MULH\n");
  print_string("0xFFFFFFF * 100000 = ");
  print_result(0xFFFFFFF, 100000, mul);
  print_string("0xFFFFFFF * 100000 = ");
  print_result(0xFFFFFFF, 100000, mulh);

  print_string("0xFFFFFFF * -100000 = ");
  print_result(0xFFFFFFF, -100000, mul);
  print_string("0xFFFFFFF * -100000 = ");
  print_result(0xFFFFFFF, -100000, mulh);

  print_string("-0xFFFFFFF * -100000 = ");
  print_result(-0xFFFFFFF, -100000, mul);
  print_string("-0xFFFFFFF * -100000 = ");
  print_result(-0xFFFFFFF, -100000, mulh);

  // Testing MULHU
  print_string("Testing MULHU\n");
  print_string("0xFFFFFFF * 100000 = ");
  print_result(0xFFFFFFF, 100000, mul);
  print_string("0xFFFFFFF * 100000 = ");
  print_result(0xFFFFFFF, 100000, mulhu);

  // Testing MULHSU
  print_string("Testing MULHSU\n");
  print_string("0xFFFFFFF * 100000 = ");
  print_result(0xFFFFFFF, 100000, mul);
  print_string("0xFFFFFFF * 100000 = ");
  print_result(0xFFFFFFF, 100000, mulhsu);

  print_string("-0xFFFFFFF * 100000 = ");
  print_result(-0xFFFFFFF, 100000, mul);
  print_string("-0xFFFFFFF * 100000 = ");
  print_result(-0xFFFFFFF, 100000, mulhsu);

  // Testing DIV
  print_string("Testing DIV:\n");
  print_string("341341 / 17 = ");
  print_result(341341, 17, div);

  print_string("-341341 / 17 = ");
  print_result(-341341, 17, div);

  print_string("341341 / -17 = ");
  print_result(341341, -17, div);
  print_signed_as_unsigned(-20078);

  // Testing DIVU
  print_string("Testing DIVU:\n");
  print_string("341341 / 17 = ");
  print_result(341341, 17, divu);

  // Testing REM
  print_string("Testing REM: ");
  print_string("42 % 9 = ");
  print_result(42, 9, rem);

  print_string("-42 % 9 = ");
  print_result(-42, 9, rem);
  print_signed_as_unsigned(-6);

  print_string("42 % -9 = ");
  print_result(42, -9, rem);

  print_string("Testing REM: ");
  print_string("42 % 9 = ");
  print_result(42, 9, remu);
  return 0;
}

/*
 * *
 *
 *0b100111000011111111111111111101100011110000
 */
