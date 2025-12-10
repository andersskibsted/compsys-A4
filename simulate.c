#include "simulate.h"
#include "branch_predictor.h"
#include "disassemble.h"
#include "memory.h"
#include "logging.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int logging = 0;
int instruction_count = 0;

/* #define LOG_INSTR(buf, size, line, addr, hex, instr, desc, jump) \ */
/*     snprintf(buf, size, "%5d %3s %8x : %08X %-50s %10s %-30s\n", \ */
/*              line, (jump) ? "==>" : "  ", addr, hex, instr, "", desc) */

/* typedef struct { */
/*   int rd; */
/*   int jump_addr; */
/* } JAL_return; */

/* typedef struct { */
/*   int branch; */
/*   int branch_addr; */
/* } B_return; */

/* typedef struct { */
/*   int correct; */
/*   int wrong; */
/* } jump_predictions_t; */


jump_predictions_t NT_predictions = { .correct = 0, .wrong = 0};
jump_predictions_t BTFNT_predictions = { .correct = 0, .wrong = 0};
jump_predictions_t Bimodal_256_predictions = { .correct = 0, .wrong = 0};
jump_predictions_t gShare_256_predictions = { .correct = 0, .wrong = 0};

jump_predictions_t Bimodal_1024_predictions = { .correct = 0, .wrong = 0};
jump_predictions_t gShare_1024_predictions = { .correct = 0, .wrong = 0};


jump_predictions_t Bimodal_4096_predictions = { .correct = 0, .wrong = 0};
jump_predictions_t gShare_4096_predictions = { .correct = 0, .wrong = 0};

jump_predictions_t Bimodal_16384_predictions = { .correct = 0, .wrong = 0};
jump_predictions_t gShare_16384_predictions = { .correct = 0, .wrong = 0};

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

int execute_r_type(DecodedInstruction r_type_instruction) {

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

  int rs1 = register_file[i_type_instruction.rs1];
  int imm = i_type_instruction.imm;
  int rd = 0;

  if (i_type_instruction.opcode == 0x03) {
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
      rd = ADD(rs1, imm);
      break;
    case 0x01:
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

  switch (s_type_instruction.funct3) {
  case 0x00:
    memory_wr_b(mem, store_addr, rs2);
    store_status = 1;
    break;
  case 0x01:
    memory_wr_h(mem, store_addr, rs2);
    store_status = 1;
    break;
  case 0x02:
    memory_wr_w(mem, store_addr, rs2);
    store_status = 1;
    break;
  default:
    break;
  }
  return store_status;
}

B_return execute_b_type(DecodedInstruction b_type_instruction, int addr) {

  int32_t rs1 = register_file[b_type_instruction.rs1];
  int32_t rs2 = register_file[b_type_instruction.rs2];

  B_return return_value;
  return_value.branch = 0;
  return_value.branch_addr = b_type_instruction.imm + (int32_t) addr;

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
    printf("ecall code %d\n", a7);
    printf("Unknown ecall\n");
    break;
  }
  return exit_code;
}

int advance_addr(int addr, int step) {
  return addr + step;
}

/* // Function for logging simulation to file */
/* int log_instruction(int instruction_count, int addr, int instruction, */
/*                     DecodedInstruction decoded_instruction, int jump_flag, */
/*                     FILE *log_file, void *data, struct symbols *symbols) { */
/*   if (!logging) { */
/*     // Logging turned off */
/*     return -1; */
/*   } */

/*   char disassembly[100]; */
/*   disassemble(addr, instruction, disassembly, 100, symbols); */
/*   char buffer[200]; */
/*   memset(buffer, 0, sizeof(buffer)); */

/*   InstructionFormat format = get_format(decoded_instruction.opcode); */
/*   int bytes_written = 0; */

/*   switch (format) { */

/*   case FRMT_R: */
/*   case FRMT_I: */
/*   case FRMT_U: { */
/*     int result = *(int *)data; */
/*     char description[30]; */
/*     snprintf(description, 30, "          R[%2d] <- %x", decoded_instruction.rd, */
/*              result); */
/*     bytes_written = LOG_INSTR(buffer, 200, instruction_count, addr, instruction, */
/*                               disassembly, description, jump_flag); */
/*     break; */
/*   } */

/*   case FRMT_S: { */
/*     int store_data = register_file[decoded_instruction.rs1]; */
/*     int store_addr = */
/*         register_file[decoded_instruction.rs2] + decoded_instruction.imm; */
/*     char memory[55]; */
/*     snprintf(memory, 55, "                   %x -> Mem[%x]", store_data, */
/*              store_addr); */
/*     bytes_written = LOG_INSTR(buffer, 200, instruction_count, addr, instruction, */
/*                               disassembly, memory, jump_flag); */
/*     break; */
/*   } */

/*   case FRMT_B: { */
/*     char jump[10]; */
/*     B_return result = *(B_return *)data; */
/*     if (result.branch) { */
/*       snprintf(jump, 10, "     {T}"); */
/*     } else { */
/*       snprintf(jump, 10, "     {_}"); */
/*     } */
/*     bytes_written = LOG_INSTR(buffer, 200, instruction_count, addr, instruction, */
/*                               disassembly, jump, jump_flag); */
/*     break; */
/*   } */

/*   case FRMT_J: { */
/*     char description[30]; */
/*     snprintf(description, 30, "          R[%2d] <- %x", decoded_instruction.rd, */
/*              addr + 4); */
/*     bytes_written = LOG_INSTR(buffer, 200, instruction_count, addr, instruction, */
/*                               disassembly, description, jump_flag); */
/*     break; */
/*   } */

/*   case ECALL: { */
/*     bytes_written = LOG_INSTR(buffer, 200, instruction_count, addr, instruction, */
/*                               disassembly, "", jump_flag); */
/*     break; */
/*   } */

/*   default: { */
/*     bytes_written = snprintf(buffer, 100, "\n"); */
/*     break; */
/*   } */
/*   } */

/*   fwrite(buffer, 1, bytes_written, log_file); */
/*   return bytes_written; */
/* } */

struct Stat simulate(struct memory *mem, int start_addr, FILE *log_file, struct symbols *symbols) {

  if (log_file != NULL) {
    // Turn on logging only if log_file exists
    logging = 1;

  }
  int written = 0;

  // initialization
  DecodedInstruction decoded_instruction;
  struct Stat stat;
  stat.insns = 0;
  pc = start_addr;
  // Flags for logging and termination of simulation
  int jump_log_flag = 0;
  int terminate_execution = 0;
  // Data structures for jump prediction
  // TODO - This can be kept op stack,
  // maybe their entries should be free'd at end of function
  // Bimodal predictions
  BimodalPredictor* bim_pht_256 = create_bimodal_predictor(8); // 8-bits
  BimodalPredictor* bim_pht_1024 = create_bimodal_predictor(10); // 10-bits
  BimodalPredictor* bim_pht_4096 = create_bimodal_predictor(12); // 12-bits
  BimodalPredictor* bim_pht_16384 = create_bimodal_predictor(14); // 14-bits

  // gShare predictions
  BimodalPredictor* gs_pht_256 = create_bimodal_predictor(8);
  BimodalPredictor* gs_pht_1024 = create_bimodal_predictor(10);
  BimodalPredictor* gs_pht_4096 = create_bimodal_predictor(12);
  BimodalPredictor* gs_pht_16384 = create_bimodal_predictor(14);

  // GHR shift registers
  uint8_t GHR_256 = 0;
  uint32_t GHR_1024 = 0;
  uint32_t GHR_4096 = 0;
  uint32_t GHR_16384 = 0;
  uint32_t total_predictions = 0;
// Debug prints
//
  // Read first instruction
  uint32_t instruction = memory_rd_w(mem, pc);
  // Main execution loop for instructions.
  while (instruction > 0 && !terminate_execution) {
    register_file[0] = 0;

    // Decode opcode and format
    uint32_t opcode = get_opcode(instruction);
    InstructionFormat format = get_format(opcode);

    switch (format) {

    // R-type
    case FRMT_R: {
      // Decode
      decoded_instruction = disasm_r_type(instruction);

      // Execute
      int result = execute_r_type(decoded_instruction);
      register_file[decoded_instruction.rd] = result;

      // Log
      written += log_instruction(instruction_count, pc, instruction,
                                 decoded_instruction, jump_log_flag, log_file,
                                 &result, symbols);
      jump_log_flag = 0;
      // Advance pc
      pc = advance_addr(pc, instruction_size);
      break;
      }

      // I-type
    case FRMT_I: {

      // Decode
      decoded_instruction = disasm_i_type(instruction);

      if (decoded_instruction.opcode == 0x67) {
        // If JALR
        // Execute
        JAL_return return_value = execute_JALR(decoded_instruction);
        register_file[return_value.rd] = advance_addr(pc, instruction_size);

        // Log
        written += log_instruction(instruction_count, pc, instruction,
                                   decoded_instruction, jump_log_flag, log_file,
                                   &return_value.jump_addr, symbols);
        jump_log_flag = 1;

        // Update PC to jump
        pc = return_value.jump_addr;

      } else {
        // If not JALR
        int result = execute_i_type(decoded_instruction, mem);
        register_file[decoded_instruction.rd] = result;

        // Log
        written += log_instruction(instruction_count, pc, instruction,
                                   decoded_instruction, jump_log_flag, log_file,
                                   &result, symbols);
        jump_log_flag = 0;

        // Advance pc
        pc = advance_addr(pc, instruction_size);
      }

      break;
      }

    // S-type
    case FRMT_S: {
      decoded_instruction = disasm_s_type(instruction);

      // Execution
      // Storing data to memory as a sideeffect
      int store_status = execute_s_type(decoded_instruction, mem);

      // Log
      written += log_instruction(instruction_count, pc, instruction,
                                 decoded_instruction, jump_log_flag, log_file,
                                 &store_status, symbols);
      jump_log_flag = 0;

      // Advance instruction address
      pc = advance_addr(pc, instruction_size);
      break;
      }

      // B-type
    case FRMT_B: {

      // Decode instruction
      decoded_instruction = disasm_b_type(instruction);

      // Execution
      B_return branch_result = execute_b_type(decoded_instruction, pc);
      // Branch prediction
      // Bimodal
      int bimodal_prediction_256 = bimodal_branch_prediction(bim_pht_256, pc, 8);
      int bimodal_prediction_accuracy_256 = bimodal_prediction_256 == branch_result.branch;
      update_bimodal_predictor(bim_pht_256, pc, branch_result.branch, 8);
      // 1024
      int bimodal_prediction_1024 = bimodal_branch_prediction(bim_pht_1024, pc, 10);
      int bimodal_prediction_accuracy_1024 = bimodal_prediction_1024 == branch_result.branch;
      update_bimodal_predictor(bim_pht_1024, pc, branch_result.branch, 10);
      // 4096
      int bimodal_prediction_4096 = bimodal_branch_prediction(bim_pht_4096, pc, 12);
      int bimodal_prediction_accuracy_4096 = bimodal_prediction_4096 == branch_result.branch;
      update_bimodal_predictor(bim_pht_4096, pc, branch_result.branch, 12);
      // 16384
      int bimodal_prediction_16384 = bimodal_branch_prediction(bim_pht_16384, pc, 14);
      int bimodal_prediction_accuracy_16384 = bimodal_prediction_16384 == branch_result.branch;
      update_bimodal_predictor(bim_pht_16384, pc, branch_result.branch, 14);

      // gShare
      int gS_prediction_256 = gShare_prediction(GHR_256, gs_pht_256, pc, 8);
      int gS_prediction_accuracy_256 = gS_prediction_256 == branch_result.branch;
      update_gShare_bimodal_predictor(GHR_256, gs_pht_256, pc, branch_result.branch, 8);
      GHR_256 = update_GHR(GHR_256, branch_result.branch, 8);
      // 1024
      int gS_prediction_1024 = gShare_prediction(GHR_1024, gs_pht_1024, pc, 10);
      int gS_prediction_accuracy_1024 = gS_prediction_1024 == branch_result.branch;
      update_gShare_bimodal_predictor(GHR_1024, gs_pht_1024, pc, branch_result.branch, 10);
      GHR_1024 = update_GHR(GHR_1024, branch_result.branch, 10);
      // 4096
      int gS_prediction_4096 = gShare_prediction(GHR_4096, gs_pht_4096, pc, 12);
      int gS_prediction_accuracy_4096 = gS_prediction_4096 == branch_result.branch;
      update_gShare_bimodal_predictor(GHR_4096, gs_pht_4096, pc, branch_result.branch, 12);
      GHR_4096 = update_GHR(GHR_4096, branch_result.branch, 12);
      // 1024
      int gS_prediction_16384 = gShare_prediction(GHR_16384, gs_pht_16384, pc, 14);
      int gS_prediction_accuracy_16384 = gS_prediction_16384 == branch_result.branch;
      update_gShare_bimodal_predictor(GHR_16384, gs_pht_16384, pc, branch_result.branch, 14);
      GHR_16384 = update_GHR(GHR_16384, branch_result.branch, 14);

      total_predictions++;

      if (bimodal_prediction_accuracy_256) {
        Bimodal_256_predictions.correct++;
      } else {
        Bimodal_256_predictions.wrong++;
      }
      if (gS_prediction_accuracy_256) {
        gShare_256_predictions.correct++;
      } else {
        gShare_256_predictions.wrong++;
      }
      // 1024
      if (bimodal_prediction_accuracy_1024) {
        Bimodal_1024_predictions.correct++;
      } else {
        Bimodal_1024_predictions.wrong++;
      }
      if (gS_prediction_accuracy_1024) {
        gShare_1024_predictions.correct++;
      } else {
        gShare_1024_predictions.wrong++;
      }
      // 4096
      if (bimodal_prediction_accuracy_4096) {
        Bimodal_4096_predictions.correct++;
      } else {
        Bimodal_4096_predictions.wrong++;
      }
      if (gS_prediction_accuracy_4096) {
        gShare_4096_predictions.correct++;
      } else {
        gShare_4096_predictions.wrong++;
      }
      // 16384
      if (bimodal_prediction_accuracy_16384) {
        Bimodal_16384_predictions.correct++;
      } else {
        Bimodal_16384_predictions.wrong++;
      }
      if (gS_prediction_accuracy_16384) {
        gShare_16384_predictions.correct++;
      } else {
        gShare_16384_predictions.wrong++;
      }




      if (branch_result.branch) {
        // Branch condition true, jump
        // Log
        written += log_instruction(instruction_count, pc, instruction,
                                   decoded_instruction, jump_log_flag, log_file,
                                   &branch_result, symbols);
        jump_log_flag = 1;
        // Branch prediction
        // NT - Always predict not taken
        NT_predictions.wrong++;
        // BTFNT - Backwards jump predicted, forwards not predicted taken.
        if (pc < branch_result.branch_addr) {
          BTFNT_predictions.correct++;
        } else {
          BTFNT_predictions.wrong++;
        }

        // Update pc to jump address
        pc = branch_result.branch_addr;

      } else {
        // Branch condition not true, no jump
        // Log
        written += log_instruction(instruction_count, pc, instruction,
                                   decoded_instruction, jump_log_flag, log_file,
                                   &branch_result, symbols);
        jump_log_flag = 0;

        // Advance address - no jump
        pc = advance_addr(pc, instruction_size);

        // // Branch prediction
        // NT - Awlays predict not taken
        NT_predictions.correct++;
        // BTFNT - Backwards jump predicted, forwards not predicted taken.
        if (pc < branch_result.branch_addr) {
          BTFNT_predictions.wrong++;
        } else {
          BTFNT_predictions.correct++;
        }

      }
      break;
      }

      // U-type
    case FRMT_U: {
        // Decode instruction
      decoded_instruction = disasm_u_type(instruction);
      // Execution
      int result = execute_u_type(decoded_instruction, pc);
      register_file[decoded_instruction.rd] = result;

      // Log
      written += log_instruction(instruction_count, pc, instruction,
                                 decoded_instruction, jump_log_flag, log_file,
                                 &result, symbols);
      jump_log_flag = 0;

      // Advance instruction address
      pc = advance_addr(pc, instruction_size);
      break;
      }

      // J-type
    case FRMT_J: {

      // Decode instruction
      decoded_instruction = disasm_j_type(instruction);

      // Execution and address update
      JAL_return return_values = execute_j_type(decoded_instruction, pc);

      // Save pc + 4 in rd-register
      register_file[return_values.rd] = advance_addr(pc, instruction_size);

      // Log
      written += log_instruction(instruction_count, pc, instruction,
                                 decoded_instruction, jump_log_flag, log_file,
                                 &return_values.jump_addr, symbols);
      jump_log_flag = 1;

      // Update pc
      pc = return_values.jump_addr;
      break;
    }

      // ECALL
    case ECALL: {
      // Decode (for logging)
      decoded_instruction = disasm_ecall(instruction);
      // Execute
      int ecall_status = execute_ecall();

      // Log
      written += log_instruction(instruction_count, pc, instruction,
                                 decoded_instruction, jump_log_flag, log_file,
                                 &ecall_status, symbols);

      if (ecall_status == -1) {
        // Terminate if ecall returns status -1
        terminate_execution = 1;
      }

      // Advance pc
      pc = advance_addr(pc, instruction_size);
      break;
    }

    default:
      // Fallthrough
      pc = advance_addr(pc, instruction_size);
      break;
    }

    // Fetch next instruction
    instruction = memory_rd_w(mem, pc);
    instruction_count++;

    stat.insns++;
  }

  // Write jump predictions to log file
  if (logging) {
    log_jump_predictions(log_file, total_predictions);
  }

  return stat;

}
