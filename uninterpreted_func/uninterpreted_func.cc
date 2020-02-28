#include "systemc.h"
#include "flex_sim.h"

sc_biguint<8> signed_gt(sc_biguint<8> arg1, sc_biguint<8> arg2) {
  int arg1_int = arg1.to_int();
  int arg2_int = arg2.to_int();

  int sb_1 = arg1_int >> 7;
  int sb_2 = arg1_int >> 7;

  unsigned int s_arg1 = (sb_1) ? (~arg1_int + 1) : arg1_int;
  unsigned int s_arg2 = (sb_2) ? (~arg2_int + 1) : arg2_int;
  
  sc_biguint<8> result = (s_arg1 > s_arg2)? arg1 : arg2;
  return result;
}