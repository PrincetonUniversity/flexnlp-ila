// This file contains the plug-in numeric functions for int8 calculations
#include "flex.h"

#include <systemc>
#include <nvhls_int.h>
#include <nvhls_types.h>
#include <nvhls_vector.h>
#include <nvhls_module.h>


// uninterpreted functions used in LayerReduce
sc_biguint<8> flex::GBAdpfloat_max(sc_biguint<8> arg_0, sc_biguint<8> arg_1) {
  sc_bigint<8> arg_0_s = arg_0;
  sc_bigint<8> arg_1_s = arg_1;
  sc_bigint<8> result = (arg_0_s > arg_1_s) ? arg_0 : arg_1;
  return result;
}

sc_biguint<8> flex::GBAdpfloat_mean(sc_biguint<8> arg_0, sc_biguint<8> arg_1) {
  sc_bigint<8> arg_0_s = arg_0;
  sc_bigint<8> arg_1_s = arg_1;
  sc_bigint<8> result = (arg_0_s + arg_1_s) >> 1;
  return result;
}

sc_biguint<8> flex::GBAdpfloat_add(sc_biguint<8> arg_0, sc_biguint<8> arg_1) {
  sc_bigint<8> arg_0_s = arg_0;
  sc_bigint<8> arg_1_s = arg_1;
  sc_bigint<8> result = arg_0_s + arg_1_s;
  return result;
}


// uninterpreted functions used in pe_core
sc_biguint<32> flex::PECoreAccumRightShift(sc_biguint<32> arg_0,
  sc_biguint<3> arg_1, sc_biguint<3> arg_2) {
  // TODO: May not need to shift at all for int8 computation
  return arg_0;
}

sc_biguint<32> flex::PECoreAccumGetBiasOut(sc_biguint<32> arg_0, 
  sc_biguint<8> arg_1, sc_biguint<3> arg_2) {
  // adding biases to the accumulator
  sc_bigint<32> accum = arg_0;
  sc_bigint<8> bias = arg_1;
  sc_bigint<32> result = accum + bias;
  return result;
}

sc_biguint<32> flex::PECoreAccumOverflowCheck(sc_biguint<32> arg_0) {
  // don't need to check overflow situation. The overflow may be handled
  // already with the sc_int datatype
  return arg_0;
}

sc_biguint<32> flex::ProductSum(sc_biguint<8> arg_0, sc_biguint<8> arg_1) {
  // used to calculate the product of two numbers
  sc_bigint<8> arg_0_s = arg_0;
  sc_bigint<8> arg_1_s = arg_1;
  sc_bigint<32> result = arg_0_s * arg_1_s;
  return result;
}

sc_biguint<32> flex::AccumAdd(sc_biguint<32> arg_0, sc_biguint<32> arg_1) {
  sc_bigint<32> arg_0_s = arg_0;
  sc_bigint<32> arg_1_s = arg_1;
  sc_bigint<32> result = arg_0_s + arg_1_s;
  return result;
}

sc_biguint<32> flex::AccumAdd2(sc_biguint<32> arg_0, sc_biguint<32> arg_1) {
  sc_bigint<32> arg_0_s = arg_0;
  sc_bigint<32> arg_1_s = arg_1;
  sc_bigint<32> result = arg_0_s + arg_1_s;
  return result;
}

// converting adptfloat to fix-point.
// For int8 computation, just pass the 8bit integer value to 20bit integer
sc_biguint<20> flex::Adptfloat2Fixed(sc_biguint<8> arg_0, sc_biguint<3> arg_1) {
  sc_bigint<20> result = arg_0;
  return result;
}

// converting fix-point back to adpfloat
// using int8 computation, we should consider overflow
sc_biguint<8> flex::Fixed2Adaptfloat(sc_biguint<20> arg_0, sc_biguint<3> arg_1) {
  // TODO: let systemc take cares of the overflow
  sc_bigint<8> result = arg_0.to_int();
  return result;
}

// uninterpreted functions for PE_ACT functions
sc_biguint<20> flex::PEActEadd(sc_biguint<20> arg_0, sc_biguint<20> arg_1) {
  sc_bigint<20> arg_0_s = arg_0;
  sc_bigint<20> arg_1_s = arg_1;
  sc_bigint<20> result = arg_0_s + arg_1_s;
  return result;
}

sc_biguint<20> flex::PEActEmul(sc_biguint<20> arg_0, sc_biguint<20> arg_1) {
  sc_bigint<20> arg_0_s = arg_0;
  sc_bigint<20> arg_1_s = arg_1;
  sc_bigint<20> result = arg_0_s * arg_1_s;
  return result;
}

sc_biguint<20> flex::PEActSigmoid(sc_biguint<20> arg_0) {
  // TODO: implement int8 sigmoid
  return arg_0;
}

sc_biguint<20> flex::PEActTanh(sc_biguint<20> arg_0) {
  // TODO:: implement int8 tanh
  return arg_0;
}

sc_biguint<20> flex::PEActRelu(sc_biguint<20> arg_0) {
  sc_bigint<20> arg_0_s = arg_0;
  sc_bigint<20> result;
  if (arg_0_s < 0) {
    result = 0;
  } else {
    result = arg_0_s;
  }
  return result;
}

sc_biguint<20> flex::PEActOnex(sc_biguint<20> arg_0) {
  // TODO: implement int8 version element-wise one-x function
  return arg_0;
}


// uninterpreted function used in gb_layernorm
sc_biguint<24> flex::GBNormAdd_24_20(sc_biguint<24> arg_0, sc_biguint<20> arg_1) {
  sc_bigint<24> arg_0_s = arg_0;
  sc_bigint<20> arg_1_s = arg_1;
  sc_bigint<24> result = arg_0_s.to_int() + arg_1_s.to_int();
  return result;
}

sc_biguint<20> flex::GBNormDivide_24(sc_biguint<24> arg_0, sc_biguint<24> arg_1) {
  sc_bigint<24> arg_0_s = arg_0;
  sc_bigint<24> arg_1_s = arg_1;
  sc_bigint<20> result = arg_0_s / arg_1_s;
  return result;
}

sc_biguint<20> flex::ActSignedMinus(sc_biguint<20> arg_0, sc_biguint<20> arg_1) {
  sc_bigint<20> arg_0_s = arg_0;
  sc_bigint<20> arg_1_s = arg_1;
  sc_bigint<20> result = arg_0_s - arg_1_s;
  return result;
}

sc_biguint<20> flex::SInvSqrt(sc_biguint<20> arg_0) {
  // TODO: implement int8 version SInvSqrt
  return arg_0;
}

// uninterpreted functions used in GBAttention
sc_biguint<32> flex::GBAttentionLSH(sc_biguint<32> arg_0, sc_biguint<32> arg_1) {
  sc_bigint<32> arg_0_s = arg_0;
  sc_bigint<32> arg_1_s = arg_1;
  sc_bigint<32> result = arg_0_s << arg_1_s;
  return result;
}

sc_biguint<32> flex::GBAttentionRSH(sc_biguint<32> arg_0, sc_biguint<32> arg_1) {
  sc_bigint<32> arg_0_s = arg_0;
  sc_bigint<32> arg_1_s = arg_1;
  sc_bigint<32> result = arg_0_s >> arg_1_s;
  return result;
}

sc_biguint<32> flex::GBAttentionMax(sc_biguint<32> arg_0, sc_biguint<32> arg_1) {
  sc_bigint<32> arg_0_s = arg_0;
  sc_bigint<32> arg_1_s = arg_1;
  return (arg_0_s.to_int() > arg_1_s.to_int()) ? arg_0 : arg_1;
}

sc_biguint<32> flex::GBAttentionSub(sc_biguint<32> arg_0, sc_biguint<32> arg_1) {
  sc_bigint<32> arg_0_s = arg_0;
  sc_bigint<32> arg_1_s = arg_1;
  sc_bigint<32> result = arg_0_s - arg_1_s;
  return result;
}

sc_biguint<32> flex::GBAttentionExp(sc_biguint<32> arg_0) {
  // TODO: implement int8 version of Exp
  return arg_0;
}

// GBAttentionDiv: attention scalar div used in gb_attention
sc_biguint<32> flex::GBAttentionDiv(sc_biguint<32> arg_0, sc_biguint<32> arg_1) {
  sc_bigint<32> arg_0_s = arg_0;
  sc_bigint<32> arg_1_s = arg_1;
  sc_bigint<32> result = arg_0_s / arg_1_s;
  return result;
}

sc_biguint<8> flex::GBAttentionCompress(sc_biguint<32> arg_0, sc_biguint<3> arg_1) {
  // TODO: check the gb attention compress
  sc_bigint<32> arg_0_s = arg_0;
  sc_bigint<8> result = arg_0_s.to_int();
  return result;
}


  