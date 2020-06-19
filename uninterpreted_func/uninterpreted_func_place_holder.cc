// This is a place holder for the uninterpreted function.
// The actual uninterpreted function file needs to include catapult libraries.
//
#include "systemc.h"
#include "flex_sim.h"

sc_biguint<32> flex_sim::GBAttentionLSH(sc_biguint<32> arg_0, sc_biguint<32> arg_1) {
    return arg_0;
}

sc_biguint<32> flex_sim::GBAttentionRSH(sc_biguint<32> arg_0, sc_biguint<32> arg_1) {
    return arg_0;
}

sc_biguint<32> flex_sim::GBAttentionMax(sc_biguint<32> arg_0, sc_biguint<32> arg_1) {
    return arg_0;
}

sc_biguint<32> flex_sim::GBAttentionSub(sc_biguint<32> arg_0, sc_biguint<32> arg_1) {
    return arg_0;
}

sc_biguint<32> flex_sim::GBAttentionExp(sc_biguint<32> arg_0) {
    return arg_0;
}

sc_biguint<32> flex_sim::GBAttentionDiv(sc_biguint<32> arg_0, sc_biguint<32> arg_1) {
    return arg_0;
}

sc_biguint<8>  flex_sim::GBAttentionCompress(sc_biguint<32> arg_0, sc_biguint<3> arg_1) {
    return 0;
}


// GBNormAdd used in LayerNorm
sc_biguint<24> flex_sim::GBNormAdd_24_20(sc_biguint<24> arg_0, sc_biguint<20> arg_1) {
    return arg_0;
}

// GBNormDivide used in LayerNorm
sc_biguint<20> flex_sim::GBNormDivide_24(sc_biguint<24> arg_0, sc_biguint<24> arg_1) {
    sc_biguint<20> result = 0;
    return result;
}

// ActSignedMinus used in LayerNorm
sc_biguint<20> flex_sim::ActSignedMinus(sc_biguint<20> arg_0, sc_biguint<20> arg_1) {
    return arg_0;
}

// SInvSqrt used in LayerNorm
sc_biguint<20> flex_sim::SInvSqrt(sc_biguint<20> arg_0) {
    return arg_0;
}

// adpfloat_max used in Maxpooling of LayerReduce
sc_biguint<8> flex_sim::GBAdpfloat_max(sc_biguint<8> arg_0, sc_biguint<8> arg_1) {
  
    sc_biguint<8> result = 0;
  return result;
}

// adpfloat_mean in LayerReduce
sc_biguint<8> flex_sim::GBAdpfloat_mean(sc_biguint<8> arg_0, sc_biguint<8> arg_1) {
  
    sc_biguint<8> result = 0;
  return result;
}

// adpfloat_add in LayerReduce
sc_biguint<8> flex_sim::GBAdpfloat_add(sc_biguint<8> arg_0, sc_biguint<8> arg_1) {
  
    sc_biguint<8> result = 0;
  return result;
}

// Function: PECoreAccumRightShift
// used in right shifting the accumulation results in PE_Core: RunBias()
sc_biguint<32> flex_sim::PECoreAccumRightShift(sc_biguint<32> arg_0,
                sc_biguint<3> arg_1, sc_biguint<3> arg_2) {
  
  
  sc_biguint<32> result = 0;
  return result;
}

// Function: PECoreAccumGetBiasOut
// used in adding the input_bias and adaptive-float bias in PE_Core: RunBias()
sc_biguint<32> flex_sim::PECoreAccumGetBiasOut(sc_biguint<32> arg_0, sc_biguint<8> arg_1, sc_biguint<3> arg_2) {
  
  sc_biguint<32> result = 0;

  return result;
}

// Function: PECoreAccumOverflowCheck
// used in checking whether the accumulation result is overflow in PECore: RunBias()
sc_biguint<32> flex_sim::PECoreAccumOverflowCheck(sc_biguint<32> arg_0) {
  
  sc_biguint<32> result = 0;

  return result;
}

// Function: PECoreAccum2ActReg
// used to truncate the AccumScalar to ActScalar in PECore: RunBias()
sc_biguint<20> flex_sim::PECoreAccum2ActReg(sc_biguint<32> arg_0) {
  sc_biguint<20> result = 0;

  return result;
}

// Function: ProductSum
// used to calculate the product of two adpfloat scalar in PECore: RunMac()
sc_biguint<32> flex_sim::ProductSum(sc_biguint<8> arg_0, sc_biguint<8> arg_1) {
  
  sc_biguint<32> result = 0;

  return result;
}

// Function: AccumAdd
// used to add accum_tmp to accumulate register
sc_biguint<32> flex_sim::AccumAdd(sc_biguint<32> arg_0, sc_biguint<32> arg_1) {

  sc_biguint<32> result = 0;

  return result;
}

sc_biguint<32> flex_sim::AccumAdd2(sc_biguint<32> arg_0, sc_biguint<32> arg_1) {

    sc_biguint<32> result = 0;
    return result;
}

// Function: PEActEadd
// element-wise add, used in PE_Act
sc_biguint<20> flex_sim::PEActEadd(sc_biguint<20> arg_0, sc_biguint<20> arg_1) {

  sc_biguint<20> result = 0;
  
  return result;
}

// Function : PEActEmul
// element-wise multiplication, used in PE_Act
sc_biguint<20> flex_sim::PEActEmul(sc_biguint<20> arg_0, sc_biguint<20> arg_1) {

  sc_biguint<20> result = 0;
  return result;
}

// Function: PEActSigmoid
// element-wise Sigmoid funtion, used in PE_Act
sc_biguint<20> flex_sim::PEActSigmoid(sc_biguint<20> arg_0) {

  sc_biguint<20> result = 0;
  return result;
}

// Function: PEActTanh
// element-wise Tanh funtion, used in PE_Act
sc_biguint<20> flex_sim::PEActTanh(sc_biguint<20> arg_0) {

  sc_biguint<20> result = 0;
  return result;
}

// Function: PEActRelu
// element-wise Relu, used in PE_Act
sc_biguint<20> flex_sim::PEActRelu(sc_biguint<20> arg_0) {

  sc_biguint<20> result = 0;
  return result;
}

// Function: PEActOneX
// element-wise OneX, used in PE_Act
sc_biguint<20> flex_sim::PEActOnex(sc_biguint<20> arg_0) {

  sc_biguint<20> result = 0;
  return result;
}

// Function: Adptfloat2Fixed
// transfer Adptfloat point type to fixed point type, used in PE_Act
// Will implement another version for GB_LayerNorm
sc_biguint<20> flex_sim::Adptfloat2Fixed(sc_biguint<8> arg_0, sc_biguint<3> arg_1) {

  sc_bigint<20> result = 0;

  return result;
}

// Function: Fixed2Adaptfloat
// transfer fixed point type to adaptive-float point type, used in PE_Act
// will implement another version for GB_LayerNorm
sc_biguint<8> flex_sim::Fixed2Adaptfloat(sc_biguint<20> arg_0, sc_biguint<3> arg_1) {

  sc_biguint<8> result = 0;
  return result;
}

