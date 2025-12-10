#ifndef __SIMULATE_H__
#define __SIMULATE_H__

#include "memory.h"
#include "read_elf.h"
#include <stdio.h>

#define PHT_SIZE 256
#define PHT_BITS 8

typedef struct {
  int rd;
  int jump_addr;
} JAL_return;

typedef struct {
  int branch;
  int branch_addr;
} B_return;

typedef struct {
  int correct;
  int wrong;
} jump_predictions_t;

// Simuler RISC-V program i givet lager og fra given start adresse
struct Stat { long int insns; };

// NOTE: Use of symbols provide for nicer disassembly, but is not required for A4.
// Feel free to remove this parameter or pass in a NULL pointer and ignore it.

struct Stat simulate(struct memory *mem, int start_addr, FILE *log_file, struct symbols* symbols);

#endif
