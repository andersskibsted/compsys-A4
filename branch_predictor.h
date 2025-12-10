#ifndef BRANCH_PREDICTOR_H_
#define BRANCH_PREDICTOR_H_

#include <stdint.h>
#include <stdlib.h>
#include "simulate.h"
// Prediction set to weakly taken
#define PREDICTION_INITIAL_VALUE 2

typedef struct {
  uint8_t counter;
} PHTEntry;

typedef struct {
  size_t size;
  PHTEntry* entry_table;
} BimodalPredictor;

typedef struct {
  PHTEntry entry_table[PHT_SIZE];
} PHT;

BimodalPredictor* create_bimodal_predictor(int bits);

uint32_t get_index(uint32_t b_instruction_addr, uint8_t number_of_bits);

void update_pht(BimodalPredictor *prediction_table, uint32_t index,
                int prediction_result);

void update_bimodal_predictor(BimodalPredictor *prediction_table,
                              uint32_t b_instruction_addr,
                              int prediction_result, uint8_t number_of_bits);


uint32_t update_GHR(uint32_t GHR, int branch_result, int number_of_bits);


int bimodal_branch_prediction(BimodalPredictor *prediction_table,
                              uint32_t b_instruction_addr,
                              uint8_t number_of_bits);



int gShare_prediction(uint32_t GHR, BimodalPredictor *gShare_PHT,
                      uint32_t b_instruction_addr, uint8_t number_of_bits);

void update_gShare_bimodal_predictor(uint32_t GHR, BimodalPredictor *gShare_PHT,
                                     uint32_t b_instruction_addr,
                                     int prediction_result, uint8_t number_of_bits);

#endif // BRANCH_PREDICTOR_H_
