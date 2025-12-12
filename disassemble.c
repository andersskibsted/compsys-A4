#include "disassemble.h"
#include "read_elf.h"
#include "memory.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* void print_bin_uint32(uint32_t x) { */
/*     for (int i = 31; i >= 0; i--) { */
/*         putchar( (x & (1u << i)) ? '1' : '0' ); */
/*     } */
/* } */

/* void print_bin_uint8(uint8_t x) { */
/*     for (int i = 7; i >= 0; i--) { */
/*         putchar( (x & (1u << i)) ? '1' : '0' ); */
/*     } */
/* } */

/* typedef enum { */
/*   ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND */
/* } ArithmeticType; */


const char *registers[32] = {
    [0] = "zero", [1] = "ra",   [2] = "sp",   [3] = "gp",  [4] = "tp",
    [5] = "t0",   [6] = "t1",   [7] = "t2",   [8] = "fp",  [9] = "s1",
    [10] = "a0",  [11] = "a1",  [12] = "a2",  [13] = "a3", [14] = "a4",
    [15] = "a5",  [16] = "a6",  [17] = "a7",  [18] = "s2", [19] = "s3",
    [20] = "s4",  [21] = "s5",  [22] = "s6",  [23] = "s7", [24] = "s8",
    [25] = "s9",  [26] = "s10", [27] = "s11", [28] = "t3", [29] = "t4",
    [30] = "t5",  [31] = "t6"};

InstructionFormat get_format(uint8_t opcode) {

  switch (opcode) {
    case 0x33: return FRMT_R;
    case 0x03: return FRMT_I;
    case 0x13: return FRMT_I;
    case 0x67: return FRMT_I;
    case 0x23: return FRMT_S;
    case 0x63: return FRMT_B;
    case 0x37: return FRMT_U;
    case 0x17: return FRMT_U;
    case 0x6F: return FRMT_J;
    case 0x73: return ECALL;
    default:
        return -1;
  }
}


uint8_t get_opcode(uint32_t instruction) {
  return instruction & 0x7F;
}

uint8_t get_rd(uint32_t instruction) {
  return (instruction >> 7) & 0x1F;
}

uint8_t get_funct3(uint32_t instruction) {
  return (instruction >> 12) & 0x07;
}

uint8_t get_rs1(uint32_t instruction) {
  return (instruction >> 15) & 0x1F;
}

uint8_t get_rs2(uint32_t instruction) {
  return (instruction >> 20) & 0x1F;
}


uint8_t get_funct7(uint32_t instruction) {
  return (instruction >> 25) & 0x7F;
}

DecodedInstruction disasm_r_type(uint32_t instruction) {
  DecodedInstruction decoded;

  decoded.opcode = get_opcode(instruction);
  decoded.rd = get_rd(instruction);
  strcpy(decoded.rd_string, registers[decoded.rd]);
  decoded.funct3 = get_funct3(instruction);
  decoded.rs1 = get_rs1(instruction);
  strcpy(decoded.rs1_string, registers[decoded.rs1]);
  decoded.rs2 = get_rs2(instruction);
  strcpy(decoded.rs2_string, registers[decoded.rs2]);
  decoded.funct7 = get_funct7(instruction);

  return decoded;
}

void handle_r_type(DecodedInstruction r_type_instruction, char* output) {

    char result[5];
  switch (r_type_instruction.funct7) {
    case 0x00:
      // Standard arithmetic
      if (r_type_instruction.funct3 == 0x00) memcpy(result, "ADD", 4);
      if (r_type_instruction.funct3 == 0x01) memcpy(result, "SLL", 4);
      if (r_type_instruction.funct3 == 0x02) memcpy(result, "SLT", 4);
      if (r_type_instruction.funct3 == 0x03) memcpy(result, "SLTU", 5);
      if (r_type_instruction.funct3 == 0x04) memcpy(result, "XOR", 4);
      if (r_type_instruction.funct3 == 0x05) memcpy(result, "SRL", 4);
      if (r_type_instruction.funct3 == 0x06) memcpy(result, "OR", 3);
      if (r_type_instruction.funct3 == 0x07) memcpy(result, "AND", 4);
      break;
    case 0x01:
      // Multiplication extension
      if (r_type_instruction.funct3 == 0x00)
        memcpy(result, "MUL", 4);
      if (r_type_instruction.funct3 == 0x01)
        memcpy(result, "MULH", 5);
      if (r_type_instruction.funct3 == 0x02)
        memcpy(result, "MULHSU", 7);
      if (r_type_instruction.funct3 == 0x03)
        memcpy(result, "MULHU", 6);
      if (r_type_instruction.funct3 == 0x04)
        memcpy(result, "DIV", 4);
      if (r_type_instruction.funct3 == 0x05)
        memcpy(result, "DIVU", 5);
      if (r_type_instruction.funct3 == 0x06)
        memcpy(result, "REM", 4);
      if (r_type_instruction.funct3 == 0x07)
        memcpy(result, "REMU", 5);
      break;

    case 0x20:
      if (r_type_instruction.funct3 == 0x00)
        memcpy(result, "SUB", 4);
      if (r_type_instruction.funct3 == 0x05)
        memcpy(result, "SRA", 4);
      break;
    default:
      memcpy(result, "ERR", 4);
      break;
    }
  sprintf(output, "%-20s %-10s %s, %s, %s",
          "",
          result,
          r_type_instruction.rd_string,
          r_type_instruction.rs1_string,
          r_type_instruction.rs2_string);
}

DecodedInstruction disasm_i_type(uint32_t instruction) {
  DecodedInstruction decoded;

  decoded.opcode = get_opcode(instruction);
  decoded.rd = get_rd(instruction);
  strcpy(decoded.rd_string, registers[decoded.rd]);
  decoded.funct3 = get_funct3(instruction);
  decoded.rs1 = get_rs1(instruction);
  strcpy(decoded.rs1_string, registers[decoded.rs1]);
  // Sign extend 12-bit to 32-bit
  decoded.imm = ((int32_t)instruction) >> 20;
  return decoded;
}

void handle_i_type(DecodedInstruction i_type_instruction, char *output) {
  char result[6];
  // if SLLI, SRLI or SRAI, imm should be turned into 5 bit shamt
  int imm_5bit = 0;
  int l_instruction = 0;
  if (i_type_instruction.opcode == 0x67) {
    memcpy(result, "JALR", 5);
  } else if (i_type_instruction.opcode == 0x03) {
    switch (i_type_instruction.funct3) {
        l_instruction = 1;
    case 0x00:
      memcpy(result, "LB", 3);
      break;
    case 0x01:
      memcpy(result, "LH", 3);
      break;
    case 0x02:
      memcpy(result, "LW", 3);
      break;
    case 0x04:
      memcpy(result, "LBU", 4);
      break;
    case 0x05:
      memcpy(result, "LHU", 4);
      break;
    default:
      memcpy(result, "IERR", 5);
      break;
    }
  } else if (i_type_instruction.opcode == 0x13) {
    switch (i_type_instruction.funct3) {
    case 0x00:
      memcpy(result, "ADDI", 5);
      break;
    case 0x02:
      memcpy(result, "SLTI", 4);
      break;
    case 0x03:
      memcpy(result, "SLTIU", 6);
      break;
    case 0x04:
      memcpy(result, "XORI", 5);
      break;
    case 0x06:
      memcpy(result, "ORI", 4);
      break;
    case 0x07:
      memcpy(result, "ANDI", 5);
      break;
    case 0x01:
      memcpy(result, "SLLI", 5);
      imm_5bit = 1;
      break;
    case 0x05:
      imm_5bit = 1;
      // It is an I-type, but with 5 bit shift amount
      // If immediate contains a 0x20 higher up than the shift amount
      // then it means its "funct7" is for SRAI. Else it is a SRLI.
      if ((i_type_instruction.imm >> 5) & 0x20) {
          memcpy(result, "SRAI", 5);
      } else  {
          memcpy(result, "SRLI", 5);
      }
      break;
    default:
      memcpy(result, "IERR", 5);
      break;
    }
    }
  // If SLLI, SRAI or SRLI change imm to shamt
  int32_t imm = i_type_instruction.imm;
  if (imm_5bit) {
      imm = i_type_instruction.imm & 31;
  }
  if (l_instruction) {
    sprintf(output, "%-20s %-10s %s, %d(%s)",
            "",
            result,
            i_type_instruction.rd_string,
            imm,
            i_type_instruction.rs1_string);
  } else {
    sprintf(output, "%-20s %-10s %s, %s, %d",
            "",
            result,
            i_type_instruction.rd_string,
            i_type_instruction.rs1_string,
            imm);
  }
}

DecodedInstruction disasm_s_type(uint32_t instruction) {
  DecodedInstruction decoded;

  decoded.opcode = get_opcode(instruction);

  decoded.funct3 = get_funct3(instruction);
  decoded.rs1 = get_rs1(instruction);
  strcpy(decoded.rs1_string, registers[decoded.rs1]);
  decoded.rs2 = get_rs2(instruction);
  strcpy(decoded.rs2_string, registers[decoded.rs2]);
  uint32_t lower_bits = (instruction >> 7) & 0x1F;
  uint32_t upper_bits = (instruction >> 25);
  uint32_t imm = (upper_bits << 5) | lower_bits;
  // Sign extension if imm is negative.
  int32_t signed_imm = (int32_t) (imm << 20) >> 20;
  decoded.imm = signed_imm;
  return decoded;
}

void handle_s_type(DecodedInstruction s_type_instruction, char* output) {
    char result[5];
  switch (s_type_instruction.funct3) {
    case 0x00:
        memcpy(result, "SB", 3);
        break;
    case 0x01:
        memcpy(result, "SH", 3);
        break;
    case 0x02:
        memcpy(result, "SW", 3);
        break;
    default:
      break;
  }

  sprintf(output, "%-20s %-10s %s, %d(%s)",
          "",
          result,
          s_type_instruction.rs2_string,
          s_type_instruction.imm,
          s_type_instruction.rs1_string);
}

DecodedInstruction disasm_b_type(uint32_t instruction) {
  DecodedInstruction decoded;

  decoded.opcode = get_opcode(instruction);

  decoded.funct3 = get_funct3(instruction);
  decoded.rs1 = get_rs1(instruction);
  strcpy(decoded.rs1_string, registers[decoded.rs1]);
  decoded.rs2 = get_rs2(instruction);
  strcpy(decoded.rs2_string, registers[decoded.rs2]);
  uint32_t imm_12 = (instruction >> 31) & 0x01;
  uint32_t imm_10_5 = (instruction >> 25) & 0x3F;
  uint32_t imm_11 = (instruction >> 7) & 0x01;
  uint32_t imm_4_1 = (instruction >> 8) & 0x0F;
  uint32_t imm = (imm_12 << 12) | (imm_11 << 11) | (imm_10_5 << 5) | (imm_4_1 << 1);
  // Sign extension if imm is negative.
  int32_t signed_imm = (int32_t) (imm << 19) >> 19;
  decoded.imm = signed_imm;
  return decoded;
}

void handle_b_type(DecodedInstruction b_type_instruction, uint32_t addr, char* output) {
    char result[5];
  switch (b_type_instruction.funct3) {
    case 0x00:
      memcpy(result, "BEQ", 4);
      break;
    case 0x01:
      memcpy(result, "BNE", 4);
      break;
    case 0x04:
      memcpy(result, "BLT", 4);
      break;
    case 0x05:
      memcpy(result, "BGE", 4);
      break;
    case 0x06:
      memcpy(result, "BLTU", 5);
      break;
    case 0x07:
      memcpy(result, "BGEU", 5);
      break;
    default:
      break;
    }
  int32_t branch_address = b_type_instruction.imm + (int32_t) addr;

    sprintf(output, "%-20s %-10s %s, %s, %x",
            "",
            result,
            b_type_instruction.rs1_string,
            b_type_instruction.rs2_string,
            branch_address);
}

DecodedInstruction disasm_u_type(uint32_t instruction) {
  DecodedInstruction decoded;

  decoded.opcode = get_opcode(instruction);
  decoded.rd = get_rd(instruction);
  strcpy(decoded.rd_string, registers[decoded.rd]);

  uint32_t imm = (instruction >> 12);
  // Sign extension if imm is negative.
  int32_t signed_imm = (int32_t)(imm << 11) >> 11;
  decoded.imm = signed_imm;
  return decoded;
}

void handle_u_type(DecodedInstruction u_type_instruction, char* output) {
    char result[6];
  if (u_type_instruction.opcode == 0x37) {
    memcpy(result, "LUI", 4);
  } else if (u_type_instruction.opcode == 0x17 ) {
    memcpy(result, "AUIPC", 6);
  }

  sprintf(output, "%-20s %-10s %s, 0x%x", "", result, u_type_instruction.rd_string, u_type_instruction.imm);
}

DecodedInstruction disasm_j_type(uint32_t instruction) {
  DecodedInstruction decoded;

  decoded.opcode = get_opcode(instruction);
  decoded.rd = get_rd(instruction);
  strcpy(decoded.rd_string, registers[decoded.rd]);

  uint32_t imm_20 = (instruction >> 31) & 0x01;
  uint32_t imm_10_1 = (instruction >> 21) & 0x3FF;
  uint32_t imm_11 = (instruction >> 20) & 0x01;
  uint32_t imm_19_12 = (instruction >> 12) & 0xFF;
  uint32_t imm = (imm_20 << 20) | (imm_11 << 11) | (imm_10_1 << 1) | (imm_19_12 << 12);
  // Sign extension if imm is negative.
  int32_t signed_imm = (int32_t)(imm << 11) >> 11;
  decoded.imm = signed_imm;
  return decoded;
}

void handle_j_type(DecodedInstruction j_type_instruction, uint32_t addr, char* output) {
    char result[4];
  memcpy(result, "JAL", 4);
  int32_t jump_address = j_type_instruction.imm + (int32_t) addr;
  sprintf(output, "%-20s %-10s %s, %x", "", result, j_type_instruction.rd_string, jump_address);
}

DecodedInstruction disasm_ecall(uint32_t instruction) {
    DecodedInstruction decoded;

    decoded.opcode = get_opcode(instruction);

    return decoded;
}


void disassemble(uint32_t addr, uint32_t instruction, char *result, size_t buf_size, struct symbols *symbols) {

  uint8_t opcode = instruction & 0x7F;
  InstructionFormat format = get_format(opcode);
  DecodedInstruction disassembled_instruction;
  const char* symbol = symbols_value_to_sym(symbols, addr);

  switch (format) {

  case FRMT_R:
    disassembled_instruction = disasm_r_type(instruction);
    handle_r_type(disassembled_instruction, result);
    break;
  case FRMT_I:
    disassembled_instruction = disasm_i_type(instruction);
    handle_i_type(disassembled_instruction, result);
    break;
  case FRMT_S:
    disassembled_instruction = disasm_s_type(instruction);
    handle_s_type(disassembled_instruction, result);
    break;
  case FRMT_B:
    disassembled_instruction = disasm_b_type(instruction);
    handle_b_type(disassembled_instruction, addr, result);
    break;
  case FRMT_U:
    disassembled_instruction = disasm_u_type(instruction);
    handle_u_type(disassembled_instruction, result);
    break;
  case FRMT_J:
    disassembled_instruction = disasm_j_type(instruction);
    handle_j_type(disassembled_instruction, addr, result);
    break;
  case ECALL:
    sprintf(result, "%-20s %-10s", "", "ECALL");
    break;
  default:
    break;
  }

  if (symbol != NULL) {
      memcpy(result, symbol, strlen(symbol));
  }
}
