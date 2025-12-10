#include "branch_predictor.h"


uint32_t get_index(uint32_t addr, uint8_t number_of_bits) {
  return (addr >> 2) & ((1 << (number_of_bits)) - 1);
}

BimodalPredictor* create_bimodal_predictor(int bits) {

  BimodalPredictor *pht = malloc(sizeof(BimodalPredictor));
  // Convert number of bits to table size
  int size = 1 << bits;
  // Allocate memory
  pht->entry_table = malloc(size * sizeof(PHTEntry));


  for (int i = 0; i < size; i++) {
    pht->entry_table[i].counter = PREDICTION_INITIAL_VALUE;
  }
  return pht;
}

int bimodal_branch_prediction(BimodalPredictor *prediction_table,
                              uint32_t b_instruction_addr,
                              uint8_t number_of_bits) {

  uint32_t index = get_index(b_instruction_addr, number_of_bits);

  return prediction_table->entry_table[index].counter >= 2;
}

void update_pht(BimodalPredictor *prediction_table, uint32_t index, int prediction_result) {

  // Only use 4 LSB for addressing the prediction counter

  if (prediction_result) {
    if (prediction_table->entry_table[index].counter < 3) {
      prediction_table->entry_table[index].counter++;
    }
  } else {
    if (prediction_table->entry_table[index].counter > 0) {
      prediction_table->entry_table[index].counter--;
    }
  }

}

void update_bimodal_predictor(BimodalPredictor *prediction_table,
                              uint32_t b_instruction_addr,
                              int prediction_result, uint8_t number_of_bits) {

  uint32_t index = get_index(b_instruction_addr, number_of_bits);

  update_pht(prediction_table, index, prediction_result);
}

uint32_t update_GHR(uint32_t GHR, int branch_result, int number_of_bits) {
  uint32_t bit_mask = (1 << number_of_bits) - 1;
  return ((GHR << 1) | (branch_result & 1)) & bit_mask;
}

int gShare_prediction(uint32_t GHR, BimodalPredictor *gShare_PHT,
                      uint32_t b_instruction_addr, uint8_t number_of_bits) {
  uint32_t index = get_index(b_instruction_addr, number_of_bits);
  index ^= GHR;

  return gShare_PHT->entry_table[index].counter >= 2;
}

void update_gShare_bimodal_predictor(uint32_t GHR, BimodalPredictor *gShare_PHT,
                                     uint32_t b_instruction_addr,
                                     int prediction_result, uint8_t number_of_bits) {


  uint32_t index = get_index(b_instruction_addr, number_of_bits);
  index ^= GHR;

  update_pht(gShare_PHT, index, prediction_result);
}

