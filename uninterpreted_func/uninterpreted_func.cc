#include "systemc.h"
#include "flex_sim.h"

sc_biguint<8> flex_sim::signed_gt(sc_biguint<8> arg_0, sc_biguint<8> arg_1) {
  int arg_0_int = arg_0.to_int();
  int arg_1_int = arg_1.to_int();

  bool sb_0 = arg_0_int >> 7;
  bool sb_1 = arg_1_int >> 7;

  int abs_0 = arg_0_int & 127;
  int abs_1 = arg_1_int & 127;

  sc_biguint<8> result;

  if (sb_0 && sb_1) {
    result = (abs_0 < abs_1) ? arg_0 : arg_1;
  } else if (sb_0 || sb_1) {
    result = (sb_1) ? arg_0 : arg_1; 
  } else {
    result = (abs_0 > abs_1) ? arg_0 : arg_1;
  }
  
  return result;
}