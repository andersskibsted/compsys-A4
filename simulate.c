#include "simulate.h"
#include "disassemble.h"
#include "memory.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int printout = 0;

void printer(char* buffer) {
  if (printout) {
    printf("%s", buffer);
  }
}

int skipped = 0;
typedef struct {
  int rd;
  int jump_addr;
} JAL_return;

typedef struct {
  int branch;
  int branch_addr;
} B_return;

int32_t register_file[32] = {0};
int pc = 0;

const int instruction_size = 4;

// R-type instructions
int ADD(int rs1, int rs2) {
  return rs1 + rs2;
}

int SLL(int rs1, int rs2) {
  return rs1 << rs2;
}

int SLT(int rs1, int rs2) {
  return rs1 < rs2;
}

int SLTU(int rs1, int rs2) {
  return rs1 < rs2;
}

int XOR(int rs1, int rs2) {
  return rs1 ^ rs2;
}

int SRL(int a, int b) {
  // TODO - should b be casted to unsigned int?
  // Shouldn't be necessary, but for robustness
  return (unsigned int)a >> b;
}

int OR(int rs1, int rs2) {
  return rs1 | rs2;
}

int AND(int rs1, int rs2) {
  return rs1 & rs2;
}

int SUB(int rs1, int rs2) {
  return rs1 - rs2;
}

int SRA(int a, int b) {
  // TODO - implement sign extension to be sure
  // that this is arithmetic shift
  int sign_bit = 0;
  int output = a >> b;
  if (a & 0x400) {
    sign_bit = 1;
    int sign_extension = 1;
    for (int i = 1; i<=b; i++) {
      sign_extension *= 2;
    }
  }
  return a >> b;
}

int MUL(int rs1, int rs2) {
  return rs1 * rs2;
}

// TODO - Look into the signed bit of these high
// multiplications
int MULH(int a, int b) {
  // TODO - Check if care should be taken,
  // since this is the high 32 bits
  // and signed x signed...
  return a * b;
}

int MULHU(int a, int b) {
  // TODO - Check if care should be taken,
  // since this is the high 32 bits and it is *unsigned*
  return a * b;
}

int MULHSU(int a, int b) {
  // TODO - Check if care should be taken,
  // since this is the high 32 bits
  // and it is signed x unsigned
  return a * b;
}

int DIV(int a, int b) {
  return a/b;
}

int REM(int a, int b) {
  return a % b;
}

// TODO - Unsigned... Check how this is properly implemented
unsigned int DIVU(int a, int b) {
  return (unsigned int)a / (unsigned int)b;
}

unsigned int REMU(int a, int b) {
  return (unsigned int)a % (unsigned int)b;
}



// I-type instructions
void ADDI(int rd, int rs1, int32_t imm) {
  register_file[rd] = register_file[rs1] + imm;
}

int execute_r_type(DecodedInstruction r_type_instruction) {
  // TODO Get data from register
  int rs1 = register_file[r_type_instruction.rs1];
  int rs2 = register_file[r_type_instruction.rs2];
  int rd = 0;
  switch (r_type_instruction.funct7) {
  case 0x00:
    // Standard arithmetic
    if (r_type_instruction.funct3 == 0x00) {
      rd = ADD(rs1, rs2);
    }
    if (r_type_instruction.funct3 == 0x01) {
      rd = SLL(rs1, rs2);
    }
    if (r_type_instruction.funct3 == 0x02) {
      rd = SLT(rs1, rs2);
    }
    if (r_type_instruction.funct3 == 0x03) {
      rd = SLTU(rs1, rs2);
    }
    if (r_type_instruction.funct3 == 0x04) {
      rd = XOR(rs1, rs2);
    }
    if (r_type_instruction.funct3 == 0x05) {
      rd = SRL(rs1, rs2);
    }
    if (r_type_instruction.funct3 == 0x06) {
      rd = OR(rs1, rs2);
    }
    if (r_type_instruction.funct3 == 0x07) {
      rd = AND(rs1, rs2);
    }
    break;
  case 0x01:
    // Multiplication extension
    if (r_type_instruction.funct3 == 0x00) {
      rd = MUL(rs1, rs2);
    }
    if (r_type_instruction.funct3 == 0x01) {
      rd = MULH(rs1, rs2);
    }
    if (r_type_instruction.funct3 == 0x02) {
      rd = MULHSU(rs1, rs2);
    }
    if (r_type_instruction.funct3 == 0x03) {
      rd = MULHU(rs1, rs2);
    }
    if (r_type_instruction.funct3 == 0x04) {
      rd = DIV(rs1, rs2);
    }
    if (r_type_instruction.funct3 == 0x05) {
      rd = DIVU(rs1, rs2);
    }
    if (r_type_instruction.funct3 == 0x06) {
      rd = REM(rs1, rs2);
    }
    if (r_type_instruction.funct3 == 0x07) {
      rd = REMU(rs1, rs2);
    }
    break;

  case 0x20:
    if (r_type_instruction.funct3 == 0x00) {
      rd = SUB(rs1, rs2);
    }
    if (r_type_instruction.funct3 == 0x05) {
      rd = SRA(rs1, rs2);
    }
    break;
  default:
    printf("R-type not found\n");
    break;
  }

  return rd;
}

int execute_i_type(DecodedInstruction i_type_instruction, struct memory *mem) {
    // TODO Get data from register
  int rs1 = register_file[i_type_instruction.rs1];
  int imm = i_type_instruction.imm;
  int rd = 0;
  if (i_type_instruction.opcode == 0x67) {
    // JALR
  } else if (i_type_instruction.opcode == 0x03) {
   // Load instructions
   // TODO - abstract instructions away into functions
   int address = rs1 + imm;
    switch (i_type_instruction.funct3) {
    case 0x00:
      // LB
      rd = memory_rd_b(mem, address);
      rd = (rd << 24) >> 24;
      break;
    case 0x01:
      // LH
      rd = memory_rd_h(mem, address);
      rd = (rd << 16) >> 16;
      break;
    case 0x02:
      // LW
      rd = memory_rd_w(mem, address);
      break;
    case 0x04:
      // LBU
      rd = memory_rd_b(mem, address);
      break;
    case 0x05:
      // LHU
      rd = memory_rd_h(mem, address);
      break;
    default:
      printf("I-type not found\n");
      break;
    }
  } else if (i_type_instruction.opcode == 0x13) {
    switch (i_type_instruction.funct3) {
    // Standard Immediate logic & arithmetic
    // All is ...I instructions
    case 0x00:

      printer("I-adding\n");
      //printf("rs1 %d, imm %d\n", rs1, imm);
      rd = ADD(rs1, imm);
      //printf("ADDI is %d\n", ADD(rs1, imm));
      //printf("rd = %d\n", rd);
      break;
    case 0x01:
      // TODO - 5bit_imm, should be implemented. Shift amount.
      rd = SLL(rs1, imm);
      break;
    case 0x02:
      rd = SLT(rs1, imm);
      break;
    case 0x03:
      rd = SLTU(rs1, imm);
      break;
    case 0x04:
      rd = XOR(rs1, imm);
      break;
    case 0x05:
      // TODO - 5bit_imm, should be implemented. Shift amount.
      rd = SRA(rs1, imm);
      break;
    case 0x06:
      rd = OR(rs1, imm);
      break;
    case 0x07:
      rd = AND(rs1, imm);
      break;
    default:
      printf("I-type not found\n");
      break;
    }
  }
  return rd;
}

JAL_return execute_JALR(DecodedInstruction JALR_instruction) {

  JAL_return return_values;
  return_values.rd = JALR_instruction.rd;
  return_values.jump_addr = (register_file[JALR_instruction.rs1] + JALR_instruction.imm) & ~1;

  return return_values;

}


int execute_s_type(DecodedInstruction s_type_instruction, struct memory *mem) {
  int store_status = 0;
  int store_addr = register_file[s_type_instruction.rs1] + s_type_instruction.imm;
  int rs2 = register_file[s_type_instruction.rs2];
  //printf("In store execution. %d, %d, addr %d\n", s_type_instruction.rs1, s_type_instruction.imm, store_addr);
  switch (s_type_instruction.funct3) {
  case 0x00:
    memory_wr_b(mem, store_addr, rs2);
    break;
  case 0x01:
    memory_wr_h(mem, store_addr, rs2);
    break;
  case 0x02:
    memory_wr_w(mem, store_addr, rs2);
    break;
  default:
    break;
  }
  return store_status;
}

B_return execute_b_type(DecodedInstruction b_type_instruction, int addr) {

  /* int branch_addr = b_type_instruction.imm; */
  int32_t rs1 = register_file[b_type_instruction.rs1];
  int32_t rs2 = register_file[b_type_instruction.rs2];
  B_return return_value;
  return_value.branch = 0;
  return_value.branch_addr = b_type_instruction.imm + (int32_t) addr;
  //printf("Branch addr %x and imm: %x\n", return_value.branch_addr, (int32_t)b_type_instruction.imm);
  switch (b_type_instruction.funct3) {
  case 0x00:
    // BEQ
    if (rs1 == rs2) {
      return_value.branch = 1;
    }
    break;
  case 0x01:
    // BNE
    if (rs1 != rs2) {
     // printf("rs1 %d and rs2 %d\n", rs1, rs2);
      return_value.branch = 1;
    }
    break;
  case 0x04:
    // BLT
    if (rs1 < rs2) {
      return_value.branch = 1;
    }
    break;
  case 0x05:
    // BGE
    if (rs1 >= rs2) {
      return_value.branch = 1;
    }
    break;
  case 0x06:
    // BLTU
    if ((unsigned int) rs1 < (unsigned int) rs2) {
      return_value.branch = 1;
    }
    break;
  case 0x07:
    // BGEU
    if ((unsigned int) rs1 >= (unsigned int) rs2) {
      return_value.branch = 1;
    }
    break;
  default:
    printf("Unknown branch instruction\n");
    break;
  }
  return return_value;
}

int execute_u_type(DecodedInstruction u_type_instruction, int addr) {
  int upper_constant = 0;

  switch (u_type_instruction.opcode) {
  case 0x37:
    upper_constant = u_type_instruction.imm << 12;
    break;
  case 0x17:
    upper_constant = addr + (u_type_instruction.imm << 12);
    break;
  default:
    printf("Unknown U-type instruction.\n");
    break;
  }

  return upper_constant;
}

JAL_return execute_j_type(DecodedInstruction j_type_instruction, int address) {

  JAL_return return_values;
  return_values.rd = j_type_instruction.rd;
  return_values.jump_addr = address + j_type_instruction.imm;

  return return_values;
}

int execute_ecall() {
  int a7 = register_file[17];
  if (printout) {
    printf("a7 is %d\n", a7);
  }
  int exit_code = 0;
  switch (a7) {
  case 1:
    register_file[10] = getchar();
    exit_code = 1;
    break;
  case 2:
    putchar(register_file[10]);
    exit_code = 1;
    break;
  case 3:
  case 93:
    exit_code = -1;
    break;
  default:
    printf("Unknown ecall\n");
    break;
  }
  return exit_code;
}

int advance_addr(int addr, int step) {
  return addr + step;
}

struct Stat simulate(struct memory *mem, int start_addr, FILE *log_file, struct symbols *symbols) {

  // initialization
  DecodedInstruction decoded_instruction;
  struct Stat stat;
  stat.insns = 0;
  pc = start_addr;
  printf("Starting simulator at %x\n", pc);

  // main execution of instructions

  uint32_t instruction = memory_rd_w(mem, pc);
  int instruction_count = 0;
  int jumps = 0;
  int terminate_address = 0x100e8;
  int terminate_execution = 0;
  while (instruction > 0 && !terminate_execution) {
    register_file[0] = 0;
  /* while (instruction_count < 200) { */

    // Decode instruction
    uint32_t opcode = get_opcode(instruction);
    InstructionFormat format = get_format(opcode);
    /* printf("Now executing %x\n", instruction); */
    /* printf("Address: %x\n", pc); */
    /* printf("Opcode: %x\n", opcode); */

    char print_out[100];
    disassemble(pc, instruction, print_out, 100, NULL);
    if (printout) {


    printf("Instruction number: %d %8x : %08X       %s\n", instruction_count, pc, instruction, print_out);
    printf("Jumps: %d\n", jumps);
    printf("ra is %x\n", register_file[1]);
    /* printf("Skipped instructions %d,\n", skipped); */

    }

    switch (format) {

      // // R-type
    case FRMT_R:
      decoded_instruction = disasm_r_type(instruction);
      printer("R-type \n");
      if (printout) {
      printf("Register[%d] is %d\n", decoded_instruction.rd,
             register_file[decoded_instruction.rd]);
      }
      register_file[decoded_instruction.rd] =
          execute_r_type(decoded_instruction);

      if (printout) {
      printf("Register[%d] is %d\n", decoded_instruction.rd,
             register_file[decoded_instruction.rd]);

      }
      pc = advance_addr(pc, instruction_size);
      break;

      // I-type
    case FRMT_I:
      decoded_instruction = disasm_i_type(instruction);
      printer("I-type \n");

      if (printout) {
      printf("Register[%d] is %d\n", decoded_instruction.rd,
             register_file[decoded_instruction.rd]);
      }
      // Execution and storage in register
      if (decoded_instruction.opcode == 0x67) {
        // If JALR
        // Execute
        JAL_return return_value = execute_JALR(decoded_instruction);
        register_file[return_value.rd] = advance_addr(pc, instruction_size);
        // Update PC to jump
        pc = return_value.jump_addr;
       // printf("pc to jump to %x\n", pc);
        jumps++;

      } else {
        // If not JALR
        //printf("Trying to do I\n");
        int i_return = execute_i_type(decoded_instruction, mem);
        //printf("rs1 %d, rd %d, imm %d, I return %d\n", decoded_instruction.rs1,
        //       decoded_instruction.rd, decoded_instruction.imm, i_return);
        register_file[decoded_instruction.rd] = i_return;
        //printf("%d i_return %d\n", decoded_instruction.rd, i_return);
            //execute_i_type(decoded_instruction, mem);
        pc = advance_addr(pc, instruction_size);
      }

      if (printout) {
        printf("Register[%d] is %d\n", decoded_instruction.rd,
               register_file[decoded_instruction.rd]);
      }


      break;
    case FRMT_S:
      decoded_instruction = disasm_s_type(instruction);
      printer("S-type\n");
      // Execution
      int store_status = execute_s_type(decoded_instruction, mem);
      // Advance instruction address
      pc = advance_addr(pc, instruction_size);
      break;

    case FRMT_B:
      decoded_instruction = disasm_b_type(instruction);
      printer("B-type\n");
      // Execution
      B_return branch_result = execute_b_type(decoded_instruction, pc);
      // Advance or jump address
      if (branch_result.branch) {
        //printf("Branch address %x\n", branch_result.branch_addr);
        pc = branch_result.branch_addr;
        jumps++;
        /* if (pc == 0x100e4) { */
        /*   exit(0); */
        /* } */
      } else {
        pc = advance_addr(pc, instruction_size);
      }
      //printf("PC is %x\n", pc);
      break;

    case FRMT_U:
      decoded_instruction = disasm_u_type(instruction);
      printer("U-type\n");
      // Execution
      if (printout) {
        printf("Register[%d] is %d\n", decoded_instruction.rd,
             register_file[decoded_instruction.rd]);
      }
      register_file[decoded_instruction.rd] = execute_u_type(decoded_instruction, pc);
      if (printout) {
        printf("Register[%d] is %d\n", decoded_instruction.rd,
             register_file[decoded_instruction.rd]);
      }
      // Advance instruction address
      pc = advance_addr(pc, instruction_size);
      //printf("PC is %x\n", pc);
      break;
      // J-type
    case FRMT_J:
      decoded_instruction = disasm_j_type(instruction);
      printer("J-type \n");
      if (printout) {
        printf("Register[%d] is %d\n", decoded_instruction.rd,
             register_file[decoded_instruction.rd]);
      }
      // Execution and address update
      JAL_return return_values = execute_j_type(decoded_instruction, pc);
      // Save pc + 4 in rd-register
      register_file[return_values.rd] = advance_addr(pc, instruction_size);
      //printf("rd %d register saved %x\n", return_values.rd, register_file[return_values.rd]);
      //printf("PC is %x\n", pc);
      pc = return_values.jump_addr;
      if (printout) {
      printf("Register[%d] is %d\n", decoded_instruction.rd,
             register_file[decoded_instruction.rd]);
      printf("PC is %x\n", pc);
      }
      jumps++;
      break;

    case ECALL: {
      int ecall_status = execute_ecall();
      if (ecall_status == -1) {
        terminate_execution = 1;
      }
      pc = advance_addr(pc, instruction_size);
      break;
    }

    default:
      skipped++;
      pc = advance_addr(pc, instruction_size);
      break;
    }

    instruction = memory_rd_w(mem, pc);
    instruction_count++;


    stat.insns++;
  }

  //InstructionFormat format = get_format(opcode);
  /* if (opcode == FRMT_I) { */
  /*   decoded_instruction = disasm_r_type(instruction); */
  /*   ADDI(decoded_instruction.rd, decoded_instruction.rs1, decoded_instruction.imm); */
  /*   printf("ADDI %s, %s, %d is %d\n", decoded_instruction.rd_string, decoded_instruction.rs1_string, decoded_instruction.imm, */
  /*          register_file[decoded_instruction.rd]); */
  /* } */
  if (printout) {
    for (int i = 0; i < 32; i++) {
      printf("Register[%d] is %d\n", i, register_file[i]);
      printf("\n");
    }
  }
  return stat;
}
