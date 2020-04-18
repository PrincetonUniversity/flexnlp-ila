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

sc_biguint<20> flex_sim::PEActEadd(sc_biguint<20> arg_0, sc_biguint<20> arg_1) {
  // TODO: need to implement signed Add
  return arg_0;
}

sc_biguint<20> PEActEmul(sc_biguint<20> arg_0, sc_biguint<20> arg_1) {
  // TODO: implement signed multiply
  return arg_0;
}

sc_biguint<20> PEActSigmoid(sc_biguint<20> arg_0) {
  // TODO: implement Sigmoid
  return arg_0;
}

sc_biguint<20> PEActTanh(sc_biguint<20> arg_0) {
  // TODO: implement Tanh
  return arg_0;
}

sc_biguint<20> PEActRelu(sc_biguint<20> arg_0) {
  // TODO: implement Relu
  return arg_0;
}

sc_biguint<20> PEActOnex(sc_biguint<20> arg_0) {
  // TODO: implement Onex
  return arg_0;
}

sc_biguint<20> Adptflow2Fixed(sc_biguint<8> arg_0, sc_biguint<3> arg_1) {
  // TODO: implement adptflow to fixed
  sc_biguint<20> result = 0xabcd;
  
  return result;
}

sc_biguint<8> Fixed2Adaptflow(sc_biguint<20> arg_0, sc_biguint<3> arg_1) {
  // TODO: implement fixed to adaptflow
  sc_biguint<8> result = 0xab;

  return result;
}








