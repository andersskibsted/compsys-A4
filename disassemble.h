#ifndef __DISASSEMBLE_H__
#define __DISASSEMBLE_H__

#include <stddef.h>
#include <stdint.h>

// NOTE: Use of symbols provide for nicer disassembly, but is not required for A4.
// Feel free to remove this parameter or pass in a NULL pointer and ignore it.

struct symbols;

typedef enum {
  FRMT_R, FRMT_I, FRMT_S, FRMT_B, FRMT_U, FRMT_J, ECALL
} InstructionFormat;

typedef struct {
  uint8_t opcode;
  uint8_t rd;
    char rd_string[5];
  uint8_t funct3;
  uint8_t rs1;
    char rs1_string[5];
  uint8_t rs2;
    char rs2_string[5];
  uint8_t funct7;
  int32_t imm;
} DecodedInstruction;

InstructionFormat get_format(uint8_t opcode);

uint8_t get_opcode(uint32_t instruction);

uint8_t get_rd(uint32_t instruction);

uint8_t get_funct3(uint32_t instruction);

uint8_t get_rs1(uint32_t instruction);

uint8_t get_rs2(uint32_t instruction);

uint8_t get_rs2(uint32_t instruction);

uint8_t get_funct7(uint32_t instruction);

DecodedInstruction disasm_r_type(uint32_t instruction);

DecodedInstruction disasm_i_type(uint32_t instruction);

DecodedInstruction disasm_s_type(uint32_t instruction);

DecodedInstruction disasm_b_type(uint32_t instruction);

DecodedInstruction disasm_u_type(uint32_t instruction);

DecodedInstruction disasm_j_type(uint32_t instruction);



void disassemble(uint32_t addr, uint32_t instruction, char* result, size_t buf_size, struct symbols* symbols);

#endif
