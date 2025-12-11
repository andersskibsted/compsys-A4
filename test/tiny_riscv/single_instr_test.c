#include "lib.h"

typedef int (*BinaryOp)(int, int);
typedef int (*UnaryOp)(int);

int signed_to_str(char* buffer, int val) {
  if (val == 0) {
    buffer[0] = '0';
    buffer[1] = 0;
    return 1;
  }
  if (val > 0) {
    // produce digits in reverse order
    char *ptr = buffer;
    while (val) {
      *ptr++ = (val % 10) + '0';
      val = val / 10;
    }
    *ptr-- = 0;
    int retval = ptr - buffer;
    // reverse
    while (ptr > buffer) {
      char tmp = *ptr;
      *ptr = *buffer;
      *buffer = tmp;
      --ptr;
      ++buffer;
    }
    return retval;
  } else {
    // produce digits in reverse order
    val = val*(-1);
    char *ptr = buffer;
    // make space for sign
    while (val) {
      *ptr++ = (val % 10) + '0';
      val = val / 10;
    }
    *ptr++ = '-';
    *ptr-- = 0;
    int retval = ptr - buffer;
    // reverse
    while (ptr > buffer) {
      char tmp = *ptr;
      *ptr = *buffer;
      *buffer = tmp;
      --ptr;
      ++buffer;
    }
    return retval;
  }
}

int int_to_bitstr(char* buffer, int val) {
    char* ptr = buffer;

    for (int i = 31; i >= 0; i--) {
        if (val & (1 << i)) {
            *ptr++ = '1';
        } else {
            *ptr++ = '0';
        }
    }

    *ptr = '\0';
    return 32;
}

void print_result(int a, int b, BinaryOp func) {
  char buffer[50];
  int result = func(a, b);

  signed_to_str(buffer, result);
  print_string(buffer);
  print_string("\n");

}

void print_result_unsigned(int a, int b, BinaryOp func) {
  char buffer[50];
  int result = func(a, b);

  uns_to_str(buffer, result);
  print_string(buffer);
  print_string("\n");

}


void print_unary_result(int a, UnaryOp func) {
  char buffer[50];
  int result = func(a);

  signed_to_str(buffer, result);
  print_string(buffer);
  print_string("\n");
}

void print_unary_result_bits(int a, UnaryOp func) {
  char buffer[50];
  int result = func(a);

  int_to_bitstr(buffer, result);
  print_string(buffer);
  print_string("\n");
}

void print_unary_result_unsigned(int a, UnaryOp func) {
  char buffer[50];
  int result = func(a);

  uns_to_str(buffer, result);
  print_string(buffer);
  print_string("\n");
}

void print_binary_result_bits(int a, int b, BinaryOp func) {
  char buffer[50];
  int result = func(a, b);
  int_to_bitstr(buffer, result);
  print_string(buffer);
  print_string("\n");

}


void print_as_bits(int a) {
  char buffer[50];

  int_to_bitstr(buffer, a);
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
// R-types
int add(int a, int b) {
  int result = 0;

  asm volatile(
    "add %0, %1, %2"
    :   "=r" (result)
    :   "r" (a), "r" (b)
    );
  return result;
}

int sub(int a, int b) {
  int result = 0;

  asm volatile(
    "sub %0, %1, %2"
    :   "=r" (result)
    :   "r" (a), "r" (b)
    );
  return result;
}

int and(int a, int b) {
  int result = 0;

  asm volatile(
    "and %0, %1, %2"
    :   "=r" (result)
    :   "r" (a), "r" (b)
    );
  return result;
}


int or(int a, int b) {
  int result = 0;

  asm volatile(
    "or %0, %1, %2"
    :   "=r" (result)
    :   "r" (a), "r" (b)
    );
  return result;
}

int xor(int a, int b) {
  int result = 0;

  asm volatile(
    "xor %0, %1, %2"
    :   "=r" (result)
    :   "r" (a), "r" (b)
    );
  return result;
}

int sll(int a, int b) {
  int result = 0;

  asm volatile(
    "sll %0, %1, %2"
    :   "=r" (result)
    :   "r" (a), "r" (b)
    );
  return result;
}

int srl(int a, int b) {
  int result = 0;

  asm volatile(
    "srl %0, %1, %2"
    :   "=r" (result)
    :   "r" (a), "r" (b)
    );
  return result;
}

int sra(int a, int b) {
  int result = 0;

  asm volatile(
    "sra %0, %1, %2"
    :   "=r" (result)
    :   "r" (a), "r" (b)
    );
  return result;
}



int slt(int a, int b) {
  int result = 0;
  asm volatile(
    "slt %0, %1, %2"
    :   "=r" (result)
    :   "r" (a), "r" (b)
    );

  return result;
}



int sltu(int a, int b) {
  unsigned int result = 0;
  unsigned int unsigned_a = (unsigned int)a;
  unsigned int unsigned_b = (unsigned int)b;

  asm volatile(
    "slt %0, %1, %2"
    :   "=r" (result)
    :   "r" (unsigned_a), "r" (unsigned_b)
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

// B-instruktions



int lui(int imm) {
  int result = 0;
  asm volatile(
    "lui %0, 1"
    :   "=r" (result)
    :
    );
  return result;
}

int auipc(int a) {
  int result = 0;
  int pc = 0;
  char buffer[30];
  asm volatile(
    "auipc %0, 0\n"
    "auipc %1, 16"
    :   "=r" (pc), "=r" (result)
    :
    );
  print_string("PC 1 instruction before is ");
  uns_to_str(buffer, pc);
  print_string(buffer);
  print_string("\n");
  return result;
}

int main(int argc, char** argv) {

  // R-types
  print_string("Testing R-types: \n");
  print_string("10 + 20 = ");
  print_result(10, 20, add);

  print_string("10 - 20 = ");
  print_result(10, 20, sub);

  print_string("0b1101 & 0b0101 = ");
  print_binary_result_bits(13, 5, and);

  print_string("0b11010 | 0b01010 = ");
  print_binary_result_bits(26, 10, or);

  print_string("0b11010 | 0b01010 = ");
  print_binary_result_bits(26, 10, xor);

  print_string("0b1101 << 4 = ");
  print_binary_result_bits(13, 4, sll);

  print_string("0b11010 >> 4 = ");
  print_binary_result_bits(26, 4, srl);

  print_string("SRA: -0b11010 >> 4 = ");
  print_binary_result_bits(-26, 4, sra);

  print_string("8 < 100 is ");
  print_result(8, 100, slt);

  print_string("101 < 100 is ");
  print_result(101, 100, slt);

  print_string("-101 < 100 is ");
  print_result(-101, 100, slt);

  print_string("8 < -100 is ");
  print_result(8, -100, slt);

  print_string("-8 < -100 is ");
  print_result(-8, -100, slt);

  print_string("8 < 100 is ");
  print_result(8, 100, sltu);

  print_string("101 < 100 is ");
  print_result(101, 100, sltu);
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
  print_string("In bits:\n");
  print_binary_result_bits(0xFFFFFFF, 100000, mul);
  print_binary_result_bits(0xFFFFFFF, 100000, mulh);

  print_string("0xFFFFFFF * -100000 = ");
  print_result(0xFFFFFFF, -100000, mul);
  print_string("0xFFFFFFF * -100000 = ");
  print_result(0xFFFFFFF, -100000, mulh);
  print_string("In bits:\n");
  print_binary_result_bits(0xFFFFFFF, -100000, mul);
  print_binary_result_bits(0xFFFFFFF, -100000, mulh);

  print_string("-0xFFFFFFF * -100000 = ");
  print_result(-0xFFFFFFF, -100000, mul);
  print_string("-0xFFFFFFF * -100000 = ");
  print_result(-0xFFFFFFF, -100000, mulh);
  print_string("In bits:\n");
  print_binary_result_bits(-0xFFFFFFF, -100000, mul);
  print_binary_result_bits(-0xFFFFFFF, -100000, mulh);

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
  /* print_signed_as_unsigned(-20078); */

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
  /* print_signed_as_unsigned(-6); */

  print_string("42 % -9 = ");
  print_result(42, -9, rem);

  print_string("Testing REM: \n");
  print_string("42 % 9 = ");
  print_result(42, 9, remu);

  // Testing LUI
  print_string("Testing LUI: \n");
  print_string("1 << 12 = ");
  print_unary_result_bits(1, lui);

  // Testing AUIPC
  print_string("Testing AUIPC: \n");
  print_string("PC + 4 + (16 << 12) is ");
  print_unary_result(1, auipc);


  /* char buffer[10]; */
  /* signed_to_str(buffer, 102); */
  /* print_string(buffer); */

  return 0;
}

/*
 * *
 *
 *0b100111000011111111111111111101100011110000
 */
