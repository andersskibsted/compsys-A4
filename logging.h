#ifndef LOGGING_H_
#define LOGGING_H_

#include "simulate.h"
#include "disassemble.h"

#define LOG_INSTR(buf, size, line, addr, hex, instr, desc, jump) \
    snprintf(buf, size, "%5d %3s %8x : %08X %-50s %10s %-30s\n", \
             line, (jump) ? "==>" : "  ", addr, hex, instr, "", desc)

extern int logging;
extern int32_t register_file[32];

extern jump_predictions_t NT_predictions;
extern jump_predictions_t BTFNT_predictions;
extern jump_predictions_t Bimodal_256_predictions;
extern jump_predictions_t gShare_256_predictions;

extern jump_predictions_t Bimodal_1024_predictions;
extern jump_predictions_t gShare_1024_predictions;


extern jump_predictions_t Bimodal_4096_predictions;
extern jump_predictions_t gShare_4096_predictions;

extern jump_predictions_t Bimodal_16384_predictions;
extern jump_predictions_t gShare_16384_predictions;


// Function for logging simulation to file
int log_instruction(int instruction_count, int addr, int instruction,
                    DecodedInstruction decoded_instruction, int jump_flag,
                    FILE *log_file, void *data, struct symbols *symbols);


void log_jump_predictions(FILE* log_file, int total_predictions);
#endif // LOGGING_H_
