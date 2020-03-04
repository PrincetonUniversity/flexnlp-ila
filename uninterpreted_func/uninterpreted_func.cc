#include "systemc.h"
#include "flex_sim.h"

sc_biguint<8> flex_sim::signed_gt(sc_biguint<8> arg_0, sc_biguint<8> arg_1) {
  int arg_0_int = arg_0.to_int();
  int arg_1_int = arg_1.to_int();

  int sb_0 = arg_0_int >> 7;
  int sb_1 = arg_1_int >> 7;

  int s_arg_0 = (sb_0 > 0) ? (arg_0_int - 128) : arg_0_int;
  int s_arg_1 = (sb_1 > 0) ? (arg_1_int - 128) : arg_1_int;
  
  sc_biguint<8> result = (s_arg_0 > s_arg_1)? arg_0 : arg_1;
  return result;
}