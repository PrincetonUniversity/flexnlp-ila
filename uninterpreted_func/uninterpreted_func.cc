#include "systemc.h"
#include "flex_sim.h"

sc_biguint<8> signed_gt(sc_biguint<8> arg_0, sc_biguint<8> arg_1) {
  int arg0_int = arg0.to_int();
  int arg1_int = arg1.to_int();

  int sb_0 = arg0_int >> 7;
  int sb_1 = arg1_int >> 7;

  unsigned int s_arg0 = (sb_0) ? (~arg0_int + 1) : arg0_int;
  unsigned int s_arg1 = (sb_1) ? (~arg1_int + 1) : arg1_int;
  
  sc_biguint<8> result = (s_arg0 > s_arg1)? arg0 : arg1;
  return result;
}