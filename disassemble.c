#include "disassemble.h"
#include "read_elf.h"
#include "memory.h"
#include <stdint.h>
#include <string.h>

typedef enum {
  FRMT_R, FRMT_I, FRMT_S, FRMT_B, FRMT_U, FRMT_J
} InstructionFormat;

typedef enum {
  ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND
} ArithmeticType;

typedef struct {
  uint8_t opcode;
  uint8_t rd;
  uint8_t funct3;
  uint8_t rs1;
  uint8_t rs2;
  uint8_t funct7;
  int32_t imm;
} DecodedInstruction;

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
  return (instruction >> 15) & 0x17;
}

uint8_t get_rs2(uint32_t instruction) {
  return (instruction >> 20) & 0x17;
}


uint8_t get_funct7(uint32_t instruction) {
  return (instruction >> 25) & 0x7F;
}

DecodedInstruction disasm_r_type(uint32_t instruction) {
  DecodedInstruction decoded;

  decoded.opcode = get_opcode(instruction);
  decoded.rd = get_rd(instruction);
  decoded.funct3 = get_funct3(instruction);
  decoded.rs1 = get_rs1(instruction);
  decoded.rs2 = get_rs2(instruction);
  decoded.funct7 = get_funct7(instruction);

  return decoded;
}

void handle_r_type(DecodedInstruction r_type_instruction, char* result) {
  switch (r_type_instruction.funct7) {
    case 0x00:
      // Standard arithmetic
      if (r_type_instruction.funct3 == 0x00) strcpy(result, "ADD\0");
      if (r_type_instruction.funct3 == 0x01) strcpy(result, "SLL\0");
      if (r_type_instruction.funct3 == 0x02) strcpy(result, "SLT\0");
      if (r_type_instruction.funct3 == 0x03) strcpy(result, "SLTU\0");
      if (r_type_instruction.funct3 == 0x04) strcpy(result, "XOR\0");
      if (r_type_instruction.funct3 == 0x05) strcpy(result, "SRL\0");
      if (r_type_instruction.funct3 == 0x06) strcpy(result, "OR\0");
      if (r_type_instruction.funct3 == 0x07) strcpy(result, "AND\0");
      break;
    case 0x01:
      // Multiplication extension
      if (r_type_instruction.funct3 == 0x00)
        strcpy(result, "MUL");
      if (r_type_instruction.funct3 == 0x01)
        strcpy(result, "MULH");
      if (r_type_instruction.funct3 == 0x02)
        strcpy(result, "MULHSU");
      if (r_type_instruction.funct3 == 0x03)
        strcpy(result, "MULHU");
      if (r_type_instruction.funct3 == 0x04)
        strcpy(result, "DIV");
      if (r_type_instruction.funct3 == 0x05)
        strcpy(result, "DIVU");
      if (r_type_instruction.funct3 == 0x06)
        strcpy(result, "REM");
      if (r_type_instruction.funct3 == 0x07)
        strcpy(result, "REMU");
      break;

    case 0x20:
      if (r_type_instruction.funct3 == 0x00)
        strcpy(result, "SUB\0");
      if (r_type_instruction.funct3 == 0x05)
        strcpy(result, "SRA\0");
      break;
    default:
      strcpy(result, "ERR");
      break;
    }
}

DecodedInstruction disasm_i_type(uint32_t instruction) {
  DecodedInstruction decoded;

  decoded.opcode = get_opcode(instruction);
  decoded.rd = get_rd(instruction);
  decoded.funct3 = get_funct3(instruction);
  decoded.rs1 = get_rs1(instruction);
  decoded.imm = (int32_t)instruction >> 20;
  return decoded;
}

void handle_i_type(DecodedInstruction i_type_instruction, char *result) {
  char instruction[5];
  if (i_type_instruction.opcode == 0x67) {
    strcpy(result, "JALR");
  } else {

    switch (i_type_instruction.funct3) {
    case 0x00:
      strcpy(result, "LB");
      break;
    case 0x01:
      strcpy(result, "LH");
      break;
    case 0x02:
      strcpy(result, "LW");
      break;
    case 0x03:
      strcpy(result, "LBU");
      break;
    case 0x04:
      strcpy(result, "LHU");
      break;
    default:
      strcpy(result, "IERR");
      break;
    }
  }
}

DecodedInstruction disasm_s_type(uint32_t instruction) {
  DecodedInstruction decoded;

  decoded.opcode = get_opcode(instruction);

  decoded.funct3 = get_funct3(instruction);
  decoded.rs1 = get_rs1(instruction);
  decoded.rs2 = get_rs2(instruction);
  uint8_t lower_bits = (instruction >> 7) & 0x1F;
  uint8_t upper_bits = (instruction >> 25);
  int32_t imm = (upper_bits << 5) | lower_bits;
  // Sign extension if imm is negative.
  if (imm & 0x800) {
    imm |= 0xFFFFF000;
  }
  decoded.imm = imm;
  return decoded;
}

void handle_s_type(DecodedInstruction s_type_instruction, char* result) {

  switch (s_type_instruction.funct3) {
    case 0x00:
        strcpy(result, "SB");
        break;
    case 0x01:
        strcpy(result, "SH");
        break;
    case 0x02:
        strcpy(result, "SW");
        break;
    default:
      break;
  }
}

DecodedInstruction disasm_b_type(uint32_t instruction) {
  DecodedInstruction decoded;

  decoded.opcode = get_opcode(instruction);

  decoded.funct3 = get_funct3(instruction);
  decoded.rs1 = get_rs1(instruction);
  decoded.rs2 = get_rs2(instruction);
  uint8_t imm_12 = (instruction >> 31) & 0x01;
  uint8_t imm_10_5 = (instruction >> 25) & 0x17;
  uint8_t imm_11 = (instruction >> 7) & 0x01;
  uint8_t imm_4_1 = (instruction >> 8) & 0x0F;
  int32_t imm = (imm_12 << 12) | (imm_11 << 11) | (imm_10_5 << 5) | (imm_4_1 << 1);
  // Sign extension if imm is negative.
  if (imm & 0x1000) {
    imm |= 0xFFFFE000;
  }
  decoded.imm = imm;
  return decoded;
}

void handle_b_type(DecodedInstruction b_type_instruction, char* result) {

  switch (b_type_instruction.funct3) {
    case 0x00:
      strcpy(result, "BEQ");
      break;
    case 0x01:
      strcpy(result, "BNE");
      break;
    case 0x04:
      strcpy(result, "BLT");
      break;
    case 0x05:
      strcpy(result, "BGE");
      break;
    case 0x06:
      strcpy(result, "BLTU");
      break;
    case 0x07:
      strcpy(result, "BGEU");
      break;
    default:
      break;
    }
}


DecodedInstruction disasm_u_type(uint32_t instruction) {
  DecodedInstruction decoded;

  decoded.opcode = get_opcode(instruction);
  decoded.rd = get_rd(instruction);

  int32_t imm = (instruction >> 12);
  // Sign extension if imm is negative.
  imm = (int32_t)(imm << 11) >> 11;  // Sign-extend 21-bit til 32-bit
  /* if (imm & 0x1000) { */
  /*   imm |= 0xFFFFE000; */
  /* } */
  decoded.imm = imm;
  return decoded;
}

void handle_u_type(DecodedInstruction u_type_instruction, char* result) {
  if (u_type_instruction.opcode == 0x37) {
    strcpy(result, "LUI");
  } else if (u_type_instruction.opcode == 0x17 ) {
    strcpy(result, "AUIPC");
  }
}

DecodedInstruction disasm_j_type(uint32_t instruction) {
  DecodedInstruction decoded;

  decoded.opcode = get_opcode(instruction);
  decoded.rd = get_rd(instruction);

  uint8_t imm_20 = (instruction >> 31) & 0x01;
  uint8_t imm_10_1 = (instruction >> 21) & 0x3FF;
  uint8_t imm_11 = (instruction >> 20) & 0x01;
  uint8_t imm_19_12 = (instruction >> 12) & 0xFF;
  int32_t imm = (imm_20 << 20) | (imm_11 << 11) | (imm_10_1 << 1) | (imm_19_12 << 12);
  // Sign extension if imm is negative.
  imm = (int32_t)(imm << 11) >> 11;  // Sign-extend 21-bit til 32-bit
  /* if (imm & 0x1000) { */
  /*   imm |= 0xFFFFE000; */
  /* } */
  decoded.imm = imm;
  return decoded;
}

void handle_j_type(DecodedInstruction j_type_instruction, char* result) {
  strcpy(result, "JAL");
}



void disassemble(uint32_t addr, uint32_t instruction, char *result, size_t buf_size, struct symbols *symbols) {

  // System call handling - for now
  if (instruction == 0x73) {
    strcpy(result, "ECALL");
    return;
  }

  uint8_t opcode = instruction & 0x7F;
  InstructionFormat format = get_format(opcode);
  DecodedInstruction disassembled_instruction;
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
      handle_b_type(disassembled_instruction, result);
      break;
    case FRMT_U:
      disassembled_instruction = disasm_u_type(instruction);
      handle_u_type(disassembled_instruction, result);
      break;
    case FRMT_J:
      disassembled_instruction = disasm_j_type(instruction);
      handle_j_type(disassembled_instruction, result);
      break;
    default:
      strcpy(result, "Error in dissambly");
      break;
  }



  // Test for R-type instructions
  /* uint8_t opcode = instruction & 0x7F; */
  /* if (opcode == 0x33) { */
  /*   DecodedInstruction r_type_instruction = decode_r_type(instruction); */
  /*   if (r_type_instruction.funct3 == 0x00) { */
  /*     // ADD or SUB */
  /*     if (r_type_instruction.funct7 == 0x00 ) { */
  /*       strcpy(result, "ADD\0"); */
  /*     } else if (r_type_instruction.funct7 == 0x20) { */
  /*       strcpy(result, "SUB\0"); */
  /*     } */
  /*   } else if (r_type_instruction.funct3 == 0x01) { */
  /*     strcpy(result, "SLL\0"); */
  /*   } else if (r_type_instruction.funct3 == 0x02) { */
  /*     strcpy(result, "SLT\0"); */
  /*   } */
  /* } else { */
  /*   strcpy(result, "ELSE\0"); */
  /* } */
}
