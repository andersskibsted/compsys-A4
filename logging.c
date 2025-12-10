#include <string.h>
#include "logging.h"
#include "simulate.h"


// Function for logging simulation to file
int log_instruction(int instruction_count, int addr, int instruction,
                    DecodedInstruction decoded_instruction, int jump_flag,
                    FILE *log_file, void *data, struct symbols *symbols) {
  if (!logging) {
    // Logging turned off
    return -1;
  }

  char disassembly[100];
  disassemble(addr, instruction, disassembly, 100, symbols);
  char buffer[200];
  memset(buffer, 0, sizeof(buffer));

  InstructionFormat format = get_format(decoded_instruction.opcode);
  int bytes_written = 0;

  switch (format) {

  case FRMT_R:
  case FRMT_I:
  case FRMT_U: {
    int result = *(int *)data;
    char description[30];
    snprintf(description, 30, "          R[%2d] <- %x", decoded_instruction.rd,
             result);
    bytes_written = LOG_INSTR(buffer, 200, instruction_count, addr, instruction,
                              disassembly, description, jump_flag);
    break;
  }

  case FRMT_S: {
    int store_data = register_file[decoded_instruction.rs1];
    int store_addr =
        register_file[decoded_instruction.rs2] + decoded_instruction.imm;
    char memory[55];
    snprintf(memory, 55, "                   %x -> Mem[%x]", store_data,
             store_addr);
    bytes_written = LOG_INSTR(buffer, 200, instruction_count, addr, instruction,
                              disassembly, memory, jump_flag);
    break;
  }

  case FRMT_B: {
    char jump[10];
    B_return result = *(B_return *)data;
    if (result.branch) {
      snprintf(jump, 10, "     {T}");
    } else {
      snprintf(jump, 10, "     {_}");
    }
    bytes_written = LOG_INSTR(buffer, 200, instruction_count, addr, instruction,
                              disassembly, jump, jump_flag);
    break;
  }

  case FRMT_J: {
    char description[30];
    snprintf(description, 30, "          R[%2d] <- %x", decoded_instruction.rd,
             addr + 4);
    bytes_written = LOG_INSTR(buffer, 200, instruction_count, addr, instruction,
                              disassembly, description, jump_flag);
    break;
  }

  case ECALL: {
    bytes_written = LOG_INSTR(buffer, 200, instruction_count, addr, instruction,
                              disassembly, "", jump_flag);
    break;
  }

  default: {
    bytes_written = snprintf(buffer, 100, "\n");
    break;
  }
  }

  fwrite(buffer, 1, bytes_written, log_file);
  return bytes_written;
}


void log_jump_predictions(FILE* log_file, int total_predictions) {
  fprintf(log_file, "\n\nJump predictions:\n");

fprintf(log_file, "NT predictions:\n");
fprintf(log_file, "Right: %d  Wrong: %d\n",
        NT_predictions.correct, NT_predictions.wrong);
fprintf(log_file, "Sum of predictions: %d. Total predictions: %d\n\n",
        NT_predictions.correct + NT_predictions.wrong, total_predictions);

fprintf(log_file, "BTFNT predictions:\n");
fprintf(log_file, "Right: %d  Wrong: %d\n",
        BTFNT_predictions.correct, BTFNT_predictions.wrong);
fprintf(log_file, "Sum of predictions: %d. Total predictions: %d\n\n",
        BTFNT_predictions.correct + BTFNT_predictions.wrong, total_predictions);

fprintf(log_file, "Bimodal 256 predictions:\n");
fprintf(log_file, "Right: %d  Wrong: %d\n",
        Bimodal_256_predictions.correct, Bimodal_256_predictions.wrong);
fprintf(log_file, "Sum of predictions: %d. Total predictions: %d\n\n",
        Bimodal_256_predictions.correct + Bimodal_256_predictions.wrong, total_predictions);

fprintf(log_file, "Bimodal 1024 predictions:\n");
fprintf(log_file, "Right: %d  Wrong: %d\n",
        Bimodal_1024_predictions.correct, Bimodal_1024_predictions.wrong);
fprintf(log_file, "Sum of predictions: %d. Total predictions: %d\n\n",
        Bimodal_1024_predictions.correct + Bimodal_1024_predictions.wrong, total_predictions);

fprintf(log_file, "Bimodal 4096 predictions:\n");
fprintf(log_file, "Right: %d  Wrong: %d\n",
        Bimodal_4096_predictions.correct, Bimodal_4096_predictions.wrong);
fprintf(log_file, "Sum of predictions: %d. Total predictions: %d\n\n",
        Bimodal_4096_predictions.correct + Bimodal_4096_predictions.wrong, total_predictions);

fprintf(log_file, "Bimodal 16384 predictions:\n");
fprintf(log_file, "Right: %d  Wrong: %d\n",
        Bimodal_16384_predictions.correct, Bimodal_16384_predictions.wrong);
fprintf(log_file, "Sum of predictions: %d. Total predictions: %d\n\n",
        Bimodal_16384_predictions.correct + Bimodal_16384_predictions.wrong, total_predictions);

fprintf(log_file, "gShare 256 predictions:\n");
fprintf(log_file, "Right: %d  Wrong: %d\n",
        gShare_256_predictions.correct, gShare_256_predictions.wrong);
fprintf(log_file, "Sum of predictions: %d. Total predictions: %d\n\n",
        gShare_256_predictions.correct + gShare_256_predictions.wrong, total_predictions);

fprintf(log_file, "gShare 1024 predictions:\n");
fprintf(log_file, "Right: %d  Wrong: %d\n",
        gShare_1024_predictions.correct, gShare_1024_predictions.wrong);
fprintf(log_file, "Sum of predictions: %d. Total predictions: %d\n\n",
        gShare_1024_predictions.correct + gShare_1024_predictions.wrong, total_predictions);

fprintf(log_file, "gShare 4096 predictions:\n");
fprintf(log_file, "Right: %d  Wrong: %d\n",
        gShare_4096_predictions.correct, gShare_4096_predictions.wrong);
fprintf(log_file, "Sum of predictions: %d. Total predictions: %d\n\n",
        gShare_4096_predictions.correct + gShare_4096_predictions.wrong, total_predictions);

fprintf(log_file, "gShare 16384 predictions:\n");
fprintf(log_file, "Right: %d  Wrong: %d\n",
        gShare_16384_predictions.correct, gShare_16384_predictions.wrong);
fprintf(log_file, "Sum of predictions: %d. Total predictions: %d\n\n",
        gShare_16384_predictions.correct + gShare_16384_predictions.wrong, total_predictions);

}
