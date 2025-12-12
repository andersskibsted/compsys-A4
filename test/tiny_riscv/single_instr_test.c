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

// I-types
int addi(int a) {
  int result = 0;

  asm volatile(
    "addi %0, %1, 1000"
    :   "=r" (result)
    :   "r" (a)
    );
  return result;
}

int neg_addi(int a) {
  int result = 0;

  asm volatile(
    "addi %0, %1, -1000"
    :   "=r" (result)
    :   "r" (a)
    );
  return result;
}

int andi(int a) {
  int result = 0;

  asm volatile(
    "andi %0, %1, 254"
    :   "=r" (result)
    :   "r" (a)
    );
  return result;
}


int ori(int a) {
  int result = 0;

  asm volatile(
    "ori %0, %1, 257"
    :   "=r" (result)
    :   "r" (a)
    );
  return result;
}

int xori(int a) {
  int result = 0;

  asm volatile(
    "xori %0, %1, 261"
    :   "=r" (result)
    :   "r" (a)
    );
  return result;
}

int slli(int a) {
  int result = 0;

  asm volatile(
    "slli %0, %1, 4"
    :   "=r" (result)
    :   "r" (a)
    );
  return result;
}

int srli(int a) {
  int result = 0;

  asm volatile(
    "srli %0, %1, 4"
    :   "=r" (result)
    :   "r" (a)
    );
  return result;
}


int srai(int a) {
  int result = 0;

  asm volatile(
    "srai %0, %1, 4"
    :   "=r" (result)
    :   "r" (a)
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

// Store and load tests
int sb_lb(int a) {
  signed char result = 0;
  signed char array[10];
  signed char char_a = (signed char) a;
  asm volatile(
    "sb %0, 0(%1)"
    :
    :   "r" (char_a), "r" (array)
    :   "memory"
    );
  asm volatile(
    "lb %0, 0(%1)"
    :   "=r" (result)
    :   "r" (array)
    :
    );

  return (int)result;
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

int sw_lw(int a) {
  int result = 0;
  char array[10];

  asm volatile(
    "sw %0, 0(%1)"
    :
    :   "r" (a), "r" (array)
    :   "memory"
    );
  asm volatile(
    "lw %0, 0(%1)"
    :   "=r" (result)
    :   "r" (array)
    :
    );

  return result;
}

int sb_lbu(int a) {
  unsigned char result = 0;
  char array[10];
  unsigned char unsigned_char_a = (unsigned char) a;
  asm volatile(
    "sb %0, 0(%1)"
    :
    :   "r" (unsigned_char_a), "r" (array)
    :   "memory"
    );
  asm volatile(
    "lbu %0, 0(%1)"
    :   "=r" (result)
    :   "r" (array)
    :
    );

  return (int)result;
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

int beq(int a, int b) {
  int result = -1; \
    int pc = 0; \
    asm volatile( \
      "beq %2, %3, taken\n" \
      "addi %0, zero, 0\n" \
      "jal zero, end\n" \
      "taken:\n" \
      "addi %0, zero, 1\n" \
      "end:\n" \
      "auipc %1, 0\n" \
      : "+r" (result), "=r" (pc) \
      : "r" (a), "r" (b) \
    ); \
    return result;
}

int bne(int a, int b) {
  int result = -1; \
    int pc = 0; \
    asm volatile( \
      "bne %2, %3, takenne\n" \
      "addi %0, zero, 0\n" \
      "jal zero, endne\n" \
      "takenne:\n" \
      "addi %0, zero, 1\n" \
      "endne:\n" \
      "auipc %1, 0\n" \
      : "+r" (result), "=r" (pc) \
      : "r" (a), "r" (b) \
    ); \
    return result;
}

int blt(int a, int b) {
  int result = -1; \
    int pc = 0; \
    asm volatile( \
      "blt %2, %3, takenlt\n" \
      "addi %0, zero, 0\n" \
      "jal zero, endlt\n" \
      "takenlt:\n" \
      "addi %0, zero, 1\n" \
      "endlt:\n" \
      "auipc %1, 0\n" \
      : "+r" (result), "=r" (pc) \
      : "r" (a), "r" (b) \
    ); \
    return result;
}

int bge(int a, int b) {
  int result = -1; \
    int pc = 0; \
    asm volatile( \
      "bge %2, %3, takenge\n" \
      "addi %0, zero, 0\n" \
      "jal zero, endge\n" \
      "takenge:\n" \
      "addi %0, zero, 1\n" \
      "endge:\n" \
      "auipc %1, 0\n" \
      : "+r" (result), "=r" (pc) \
      : "r" (a), "r" (b) \
    ); \
    return result;
}

int bltu(int a, int b) {
  int result = -1; \
    int pc = 0; \
    asm volatile( \
      "bltu %2, %3, takenltu\n" \
      "addi %0, zero, 0\n" \
      "jal zero, endltu\n" \
      "takenltu:\n" \
      "addi %0, zero, 1\n" \
      "endltu:\n" \
      "auipc %1, 0\n" \
      : "+r" (result), "=r" (pc) \
      : "r" (a), "r" (b) \
    ); \
    return result;
}

int bgeu(int a, int b) {
  int result = -1;
    int pc = 0;
    asm volatile(
      "bgeu %2, %3, takengeu\n"
      "addi %0, zero, 0\n"
      "jal zero, endgeu\n"
      "takengeu:\n"
      "addi %0, zero, 1\n"
      "endgeu:\n"
      "auipc %1, 0\n"
      : "+r" (result), "=r" (pc)
      : "r" (a), "r" (b)
    );
    return result;
}

// Jump intsructions
int jal(int a) {
  char buffer[50];
  int pc_before = 0;
  int rd = 0;
  int pc_after = 0;

  asm volatile(
    "auipc %0, 0\n"
    "jal %1, jump\n"
    "addi %0, zero, 5\n"
    "addi %1, zero, 6\n"
    "jump:\n"
    "auipc %2, 0"
    : "=r" (pc_before), "=r" (rd), "=r" (pc_after)
    :
    );
  print_string("PC before jump: ");
  uns_to_str(buffer, pc_before);
  print_string(buffer);
  print_string("\n");

  print_string("RD from jump: ");
  uns_to_str(buffer, rd);
  print_string(buffer);
  print_string("\n");

  return pc_after;
}

int jalr(int a) {
  char buffer[50];
  int pc_before = 0;
  int rd = 0;
  int pc_after = 0;

  asm volatile(
    "auipc %0, 0\n"
    "jalr %1, 16(%3)\n"
    "addi %0, zero, 5\n"
    "addi %1, zero, 6\n"
    "auipc %2, 0"
    : "+r" (pc_before), "=r" (rd), "=r" (pc_after)
    : "r" (pc_before)
    );
  print_string("PC before jump: ");
  uns_to_str(buffer, pc_before);
  print_string(buffer);
  print_string("\n");

  print_string("RD from jump: ");
  uns_to_str(buffer, rd);
  print_string(buffer);
  print_string("\n");

  return pc_after;
}

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
  print_string("\nTesting R-types:\n\n");
  print_string("Testing ADD:\n");
  print_string("10 + 20 = ");
  print_result(10, 20, add);

  print_string("Testing SUB:\n");
  print_string("10 - 20 = ");
  print_result(10, 20, sub);

  print_string("Testing SUB:\n");
  print_string("-10 - 20 = ");
  print_result(-10, 20, sub);

  print_string("Testing AND:\n");
  print_string("0b1101 & 0b0101 = ");
  print_binary_result_bits(13, 5, and);

  print_string("Testing OR:\n");
  print_string("0b11010 | 0b01010 = ");
  print_binary_result_bits(26, 10, or);

  print_string("Testing XOR:\n");
  print_string("0b11010 | 0b01010 = ");
  print_binary_result_bits(26, 10, xor);

  print_string("Testing SLL:\n");
  print_string("0b1101 << 4 = ");
  print_binary_result_bits(13, 4, sll);

  print_string("Testing SRL:\n");
  print_string("0b11010 >> 4 = ");
  print_binary_result_bits(26, 4, srl);

  print_string("Testing SRA:\n");
  print_string("SRA: -0b11010 >> 4 = ");
  print_binary_result_bits(-26, 4, sra);

  print_string("Testing SLT:\n");
  print_string("8 < 100 is ");
  print_result(8, 100, slt);

  print_string("Testing SLT:\n");
  print_string("101 < 100 is ");
  print_result(101, 100, slt);

  print_string("Testing SLT:\n");
  print_string("-101 < 100 is ");
  print_result(-101, 100, slt);

  print_string("Testing SLT:\n");
  print_string("8 < -100 is ");
  print_result(8, -100, slt);

  print_string("Testing SLT:\n");
  print_string("-8 < -100 is ");
  print_result(-8, -100, slt);

  print_string("Testing SLTU:\n");
  print_string("8 < 100 is ");
  print_result(8, 100, sltu);

  print_string("Testing SLTU:\n");
  print_string("101 < 100 is ");
  print_result(101, 100, sltu);

  print_string("\n\n");
  // Test SB LB
  print_string("Testing SB and LB:\n\n");
  print_string("Storing byte 0x78 / 120 and loading it to be: ");
  print_unary_result(120, sb_lb);
  // Testing with halfword to be truncated
  print_string("Storing halfword 0x5678 / 22136 and loading it to be 120: ");
  print_unary_result(0x5678, sb_lb);
  // Testing with negative halfword
  print_string("Storing halfword -0x5678 / -22136 and loading it to be -120: ");
  print_unary_result(-0x5678, sb_lb);

  // Testing with negative byte
  print_string("Storing word -0x78 / -120 and loading it to be: ");
  print_unary_result(-120, sb_lb);

  print_string("\n");
  // Test SH LH
  print_string("Testing SH and LH:\n");
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

  print_string("\n");
  // Test SW LW
  print_string("Testing SW and LW:\n");
  print_string("Storing byte 0x78 / 120 and loading it to be: ");
  print_unary_result(120, sw_lw);

  print_string("Storing byte -0x78 / -120 and loading it to be: ");
  print_unary_result(-120, sw_lw);

  print_string("Storing halfword 0x5678 / 22136 and loading it to be: ");
  print_unary_result(22136, sw_lw);
  // Testing with word
  print_string("Storing word 0x12345678 / 305419896 and loading it to be: ");
  print_unary_result(305419896, sw_lw);
  // Testing with negative word
  print_string("Storing word -0x12345678 / -305419896 and loading it to be: ");
  print_unary_result(-305419896, sw_lw);

  // Testing with negative halfword
  print_string("Storing halfword -22136 and loading it to be: ");
  print_unary_result(-22136, sw_lw);

  print_string("\n");
  // Test SH LHU
  print_string("Testing SH and LHU:\n");
  print_string("Storing halfword 0x5678 / 22136 and loading it to be: ");
  print_unary_result(22136, sh_lhu);
  // Testing with word to be truncated
  print_string("Storing word 0x12345678 / 305419896 and loading it to be 22136: ");
  print_unary_result(305419896, sh_lhu);

  // Test SB LBU
  print_string("Testing SB and LBU:\n");
  print_string("Storing byte 0x78 / 120 and loading it to be: ");
  print_unary_result(120, sb_lbu);
  // Testing with word to be truncated
  print_string("Storing halfword 0x5678 / 22136 and loading it to be 120: ");
  print_unary_result(22136, sb_lbu);

  print_string("\n\n");
  // Testing RV32M
  print_string("Testing RV32M:\n\n");
  // Testing MUL
  print_string("Testing MUL:\n");
  print_string("12 * 56 = ");
  print_result(12, 56, mul);

  print_string("Testing MUL:\n");
  print_string("-12 * 56 = ");
  print_result(-12, 56, mul);

  print_signed_as_unsigned(-672);
  print_string("12 * -56 = ");
  print_result(12, -56, mul);

  print_string("-12 * -56 = ");
  print_result(-12, -56, mul);

  // Testing MULH
  print_string("Testing MULH\n");
  print_string("MUL of 0xFFFFFFF * 100000 = ");
  print_result(0xFFFFFFF, 100000, mul);

  print_string("MULH of 0xFFFFFFF * 100000 = ");
  print_result(0xFFFFFFF, 100000, mulh);
  print_string("In bits (MULH - MUL):\n");

  print_binary_result_bits(0xFFFFFFF, 100000, mulh);
  print_binary_result_bits(0xFFFFFFF, 100000, mul);

  print_string("Testing MULH\n");
  print_string("MUL of 0xFFFFFFF * -100000 = ");
  print_result(0xFFFFFFF, -100000, mul);
  print_string("MULH of 0xFFFFFFF * -100000 = ");
  print_result(0xFFFFFFF, -100000, mulh);
  print_string("In bits (MULH - MUL):\n");
  print_binary_result_bits(0xFFFFFFF, -100000, mulh);
  print_binary_result_bits(0xFFFFFFF, -100000, mul);

  print_string("Testing MULH\n");
  print_string("MUL of -0xFFFFFFF * -100000 = ");
  print_result(-0xFFFFFFF, -100000, mul);
  print_string("MULH of -0xFFFFFFF * -100000 = ");
  print_result(-0xFFFFFFF, -100000, mulh);
  print_string("In bits (MULH - MUL):\n");
  print_binary_result_bits(-0xFFFFFFF, -100000, mulh);
  print_binary_result_bits(-0xFFFFFFF, -100000, mul);
  print_string("\n");

  // Testing MULHU
  print_string("Testing MULHU\n");
  print_string("MUL of 0xFFFFFFF * 100000 = ");
  print_result(0xFFFFFFF, 100000, mul);
  print_string("MULHU of 0xFFFFFFF * 100000 = ");
  print_result(0xFFFFFFF, 100000, mulhu);
  print_string("In bits (MULHU - MUL):\n");
  print_binary_result_bits(0xFFFFFFF, 100000, mulhu);
  print_binary_result_bits(0xFFFFFFF, 100000, mul);

  // Testing MULHSU
  print_string("Testing MULHSU\n");
  print_string("MUL of 0xFFFFFFF * 100000 = ");
  print_result(0xFFFFFFF, 100000, mul);
  print_string("MULHSU of 0xFFFFFFF * 100000 = ");
  print_result(0xFFFFFFF, 100000, mulhsu);
  print_string("In bits (MULHSU - MUL):\n");
  print_binary_result_bits(0xFFFFFFF, 100000, mulhsu);
  print_binary_result_bits(0xFFFFFFF, 100000, mul);

  print_string("Testing MULHSU\n");
  print_string("MUL of -0xFFFFFFF * 100000 = ");
  print_result(-0xFFFFFFF, 100000, mul);
  print_string("MULHSU of -0xFFFFFFF * 100000 = ");
  print_result(-0xFFFFFFF, 100000, mulhsu);
  print_string("In bits (MULHSU - MUL):\n");
  print_binary_result_bits(-0xFFFFFFF, 100000, mulhsu);
  print_binary_result_bits(-0xFFFFFFF, 100000, mul);

  // Testing DIV
  print_string("Testing DIV:\n");
  print_string("341341 / 17 = ");
  print_result(341341, 17, div);

  print_string("Testing DIV:\n");
  print_string("-341341 / 17 = ");
  print_result(-341341, 17, div);

  print_string("Testing DIV:\n");
  print_string("341341 / -17 = ");
  print_result(341341, -17, div);

  // Testing DIVU
  print_string("Testing DIVU:\n");
  print_string("341341 / 17 = ");
  print_result(341341, 17, divu);

  // Testing REM
  print_string("Testing REM: ");
  print_string("42 % 9 = ");
  print_result(42, 9, rem);

  print_string("Testing REM: ");
  print_string("-42 % 9 = ");
  print_result(-42, 9, rem);

  print_string("Testing REM: ");
  print_string("42 % -9 = ");
  print_result(42, -9, rem);

  print_string("Testing REMU: \n");
  print_string("42 % 9 = ");
  print_result(42, 9, remu);

  print_string("\n\nTesting U-type:\n\n");
  // Testing LUI
  print_string("Testing LUI: \n");
  print_string("1 << 12 = ");
  print_unary_result_bits(1, lui);

  // Testing AUIPC
  print_string("Testing AUIPC: \n");
  print_string("PC + 4 + (16 << 12) is ");
  print_unary_result(1, auipc);

  // Testing BEQ
  print_string("\n\nTesting braching:\n\n");
  print_string("Testing BEQ: \n");
  print_string("Jump if 1 = 1: ");
  print_result(1, 1, beq);

  print_string("Testing BEQ: \n");
  print_string("Jump if 2 = 1: ");
  print_result(2, 1, beq);

  print_string("Testing BEQ: \n");
  print_string("Jump if -1 = -1: ");
  print_result(-1, -1, beq);

  print_string("Testing BEQ: \n");
  print_string("Jump if -2 = 1: ");
  print_result(-2, 1, beq);

  // Testing BNE
  print_string("Testing BNE: \n");
  print_string("Jump if 1 != 1: ");
  print_result(1, 1, bne);

  print_string("Testing BNE: \n");
  print_string("Jump if 2 != 1: ");
  print_result(2, 1, bne);

  print_string("Testing BNE: \n");
  print_string("Jump if -1 != -1: ");
  print_result(-1, -1, bne);

  print_string("Testing BNE: \n");
  print_string("Jump if -2 != 1: ");
  print_result(-2, 1, bne);

  // Testing BLT
  print_string("Testing BLT: \n");
  print_string("Jump if 1 < 1: ");
  print_result(1, 1, blt);

  print_string("Testing BLT: \n");
  print_string("Jump if 1 < 2: ");
  print_result(1, 2, blt);

  print_string("Testing BLT: \n");
  print_string("Jump if -1 < -1: ");
  print_result(-1, -1, blt);

  print_string("Testing BLT: \n");
  print_string("Jump if -2 < 1: ");
  print_result(-2, 1, blt);

  // Testing BGE
  print_string("Testing BGE: \n");
  print_string("Jump if 1 >= 1: ");
  print_result(1, 1, bge);

  print_string("Testing BGE: \n");
  print_string("Jump if 1 >= 2: ");
  print_result(1, 2, bge);

  print_string("Testing BGE: \n");
  print_string("Jump if 2 >= 1: ");
  print_result(2, 1, bge);

  print_string("Testing BGE: \n");
  print_string("Jump if -1 >= -1: ");
  print_result(-1, -1, bge);

  print_string("Testing BGE: \n");
  print_string("Jump if -2 >= 1: ");
  print_result(-2, 1, bge);

  print_string("Testing BGE: \n");
  print_string("Jump if -2 >= -3: ");
  print_result(-2, -3, bge);

  // Testing BLTU
  print_string("Testing BLTU: \n");
  print_string("Jump if 1 < 1: ");
  print_result(1, 1, bltu);

  print_string("Testing BLTU: \n");
  print_string("Jump if 1 < 2: ");
  print_result(1, 2, bltu);

  print_string("Testing BLTU: \n");
  print_string("Jump if 2 < 1: ");
  print_result(2, 1, bltu);

  // Testing BGEU
  print_string("Testing BGEU: \n");
  print_string("Jump if 1 >= 1: ");
  print_result(1, 1, bgeu);

  print_string("Testing BGEU: \n");
  print_string("Jump if 1 >= 2: ");
  print_result(1, 2, bgeu);

  print_string("Testing BGEU: \n");
  print_string("Jump if 2 >= 1: ");
  print_result(2, 1, bgeu);

  // Test jumps
  print_string("\n\nTesting jumps:\n\n");
  print_string("Testing JAL:\n");
  print_unary_result(1, jal);

  print_string("Testing JALR:\n");
  print_unary_result(1, jalr);
  print_string("\n");
  // Test I-types
  print_string("\nTesting I arithmetic and logic\n\n");
  print_string("Testing ADDI:\n");
  print_string("101 + 1000 = ");
  print_unary_result(101, addi);

  print_string("Testing ADDI:\n");
  print_string("-1010 + 1000 = ");
  print_unary_result(-1010, addi);

  print_string("Testing ADDI:\n");
  print_string("101 - 1000 = ");
  print_unary_result(101, neg_addi);

  //
  print_string("Testing ANDI:\n");
  print_string("0b10101 + 0b1111110 = ");
  print_unary_result_bits(21, andi);

  print_string("Testing ANDI:\n");
  print_string("0b00 + 0b1111110 = ");
  print_unary_result_bits(0, andi);

  print_string("Testing ORI:\n");
  print_string("0b10101 + 0b100000001 = ");
  print_unary_result_bits(21, ori);

  print_string("Testing ORI:\n");
  print_string("0b00 + 0b100000001 = ");
  print_unary_result_bits(0, ori);

  print_string("Testing XORI:\n");
  print_string("0b10101 + 0b100000101 = ");
  print_unary_result_bits(21, xori);

  print_string("Testing XORI:\n");
  print_string("0b1000000000 + 0b100000101 = ");
  print_unary_result_bits(512, xori);

  print_string("Testing SLLI:\n");
  print_string("1 << 4 = ");
  print_unary_result_bits(1, slli);

  print_string("Testing SLLI:\n");
  print_string("256 << 4 = ");
  print_unary_result_bits(256, slli);

  print_string("Testing SRLI:\n");
  print_string("32 >> 4 = ");
  print_unary_result_bits(32, srli);

  print_string("Testing SRLI:\n");
  print_string("256 >> 4 = ");
  print_unary_result_bits(256, srli);

  print_string("Testing SRAI:\n");
  print_string("32 >> 4 = ");
  print_unary_result_bits(32, srai);

  print_string("Testing SRAI:\n");
  print_string("256 >> 4 = ");
  print_unary_result_bits(256, srai);

  print_string("Testing SRAI:\n");
  print_string("-512 >> 4 = ");
  print_unary_result_bits(-640, srai);

  // Test SLTIU
  print_string("Testing SLTIU:\n");
  print_string("8 < 100 is ");
  print_unary_result(8, sltiu);

  print_string("Testing SLTIU:\n");
  print_string("101 < 100 is ");
  print_unary_result(101, sltiu);

  // Test SLTI
  print_string("Testing SLTI:\n");
  print_string("-200 < -100 is ");
  print_unary_result(-200, slti);

  print_string("Testing SLTI:\n");
  print_string("-100 < -100 is ");
  print_unary_result(-100, slti);

  print_string("Testing SLTI:\n");
  print_string("-50 < -100 is ");
  print_unary_result(-50, slti);

  print_string("Testing SLTI:\n");
  print_string("50 < -100 is ");
  print_unary_result(50, slti);

  return 0;
}

