#include "systemc.h"
#include "flex_sim.h"

unsigned int signed_gt(unsigned int arg1, unsigned int arg2) {
  unsigned int sb_1 = arg1 >> 7;
  unsigned int sb_2 = arg2 >> 7;
  unsigned int s_arg1 = (sb_1) ? (~arg1 + 1) : arg1;
  unsigned int s_arg2 = (sb_2) ? (~arg2 + 1) : arg2;
  unsigned int result = (s_arg1 > s_arg2)? arg1 : arg2;
  return result;
}