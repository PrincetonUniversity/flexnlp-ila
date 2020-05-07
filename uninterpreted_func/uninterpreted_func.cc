#include "systemc.h"
#include "flex_sim.h"
#include <nvhls_int.h>
#include <nvhls_types.h>
#include <nvhls_vector.h>
#include <nvhls_module.h>

#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_math.h>
#include <ac_math/ac_sigmoid_pwl.h>
#include <ac_math/ac_tanh_pwl.h>
#include <ac_math/ac_div.h>
#include <ac_math/ac_pow_pwl.h>
#include <ac_math/ac_inverse_sqrt_pwl.h>
#include "SM6Spec.h"
#include "AdpfloatUtils.h"
#include "AdpfloatSpec.h"

#include <fstream>

static int counter = 0;

// signed_gt used in Maxpooling of LayerReduce
sc_biguint<8> flex_sim::signed_gt(sc_biguint<8> arg_0, sc_biguint<8> arg_1) {

  ac_int<8,false> arg_0_ac = arg_0.to_uint();
  ac_int<8,false> arg_1_ac = arg_1.to_uint();
  AdpfloatType<spec::kAdpfloatWordWidth,spec::kAdpfloatExpWidth> in_a = arg_0_ac;
  AdpfloatType<spec::kAdpfloatWordWidth,spec::kAdpfloatExpWidth> in_b = arg_1_ac;
  AdpfloatType<spec::kAdpfloatWordWidth,spec::kAdpfloatExpWidth> out_tmp;
  adpfloat_max(in_a, in_b, out_tmp);

  ac_int<8,false> result_ac = out_tmp.to_rawbits();
  sc_biguint<8> result = result_ac.to_uint();

  return result;
}

// Function: PECoreAccumRightShift
// used in right shifting the accumulation results in PE_Core: RunBias()
sc_biguint<32> flex_sim::PECoreAccumRightShift(sc_biguint<32> arg_0,
                sc_biguint<3> arg_1, sc_biguint<3> arg_2) {
  
  ac_int<3,false> arg_1_ac = arg_1.to_uint();
  ac_int<3,false> arg_2_ac = arg_2.to_uint();
  spec::AdpfloatBiasType bias_weight = arg_1_ac;
  spec::AdpfloatBiasType bias_input = arg_2_ac;

  sc_bigint<32> arg_0_s = arg_0;
  ac_int<32,true> accum_vector_ac = arg_0_s.to_int();
  spec::AccumScalarType accum_vector = accum_vector_ac;

  NVUINT5 right_shift = -2*spec::kAdpfloatOffset + 2*spec::kAdpfloatManWidth - spec::kActNumFrac
                        - bias_weight - bias_input;
  //std::cout << "right_shift: " << right_shift << '\t';
  spec::AccumScalarType result_tmp = accum_vector >> right_shift;
  //std::cout << "shifted value: " << result_tmp << std::endl;

  sc_bigint<32> result_s = result_tmp.to_int();
  sc_biguint<32> result = result_s;
  
  return result;
}

// Function: PECoreAccumGetBiasOut
// used in adding the input_bias and adaptive-float bias in PE_Core: RunBias()
sc_biguint<32> flex_sim::PECoreAccumGetBiasOut(sc_biguint<32> arg_0, sc_biguint<8> arg_1, sc_biguint<3> arg_2) {
  
  sc_bigint<32> arg_0_s = arg_0;
  ac_int<32,true> arg_0_ac = arg_0_s.to_int();
  spec::AccumScalarType accum_vector_out = arg_0_ac;

  ac_int<8,false> arg_1_ac = arg_1.to_uint();
  spec::ScalarType bias_input = arg_1_ac;

  ac_int<3,false> arg_2_ac = arg_2.to_uint();
  spec::AdpfloatBiasType bias_bias = arg_2_ac;

  AdpfloatType<spec::kAdpfloatWordWidth, spec::kAdpfloatExpWidth> adpfloat_tmp(bias_input);
  spec::ActScalarType bias_tmp2 = 
                        adpfloat_tmp.to_fixed<spec::kActWordWidth, spec::kActNumFrac>(bias_bias);

  accum_vector_out += bias_tmp2;

  sc_bigint<32> result_s = accum_vector_out.to_int();
  sc_biguint<32> result = result_s;

  return result;
}

// Function: PECoreAccumOverflowCheck
// used in checking whether the accumulation result is overflow in PECore: RunBias()
sc_biguint<32> flex_sim::PECoreAccumOverflowCheck(sc_biguint<32> arg_0) {
  
  sc_bigint<32> arg_0_s = arg_0;
  ac_int<32,true> arg_0_ac = arg_0_s.to_int();
  spec::AccumScalarType accum_vector_out = arg_0_ac;

  if (accum_vector_out > spec::kActWordMax)
    accum_vector_out = spec::kActWordMax;
  else if (accum_vector_out < spec::kActWordMin) 
    accum_vector_out = spec::kActWordMin;

  sc_bigint<32> result_s = accum_vector_out.to_int();
  sc_biguint<32> result = result_s;

  return result;
}

// Function: PECoreAccum2ActReg
// used to truncate the AccumScalar to ActScalar in PECore: RunBias()
sc_biguint<20> flex_sim::PECoreAccum2ActReg(sc_biguint<32> arg_0) {
  
  sc_bigint<32> arg_0_s = arg_0;
  ac_int<32, true> arg_0_ac = arg_0_s.to_int();
  spec::AccumScalarType accum_vector = arg_0_ac;
  spec::ActScalarType act_port_reg = accum_vector;
  
  sc_bigint<20> result_s = act_port_reg.to_int();
  sc_biguint<20> result = result_s;

  return result;
}

// Function: ProductSum
// used to calculate the product of two adpfloat scalar in PECore: RunMac()
sc_biguint<32> flex_sim::ProductSum(sc_biguint<8> arg_0, sc_biguint<8> arg_1) {
  
  ac_int<8,false> arg_0_ac = arg_0.to_uint();
  ac_int<8,false> arg_1_ac = arg_1.to_uint();

  spec::ScalarType in_1 = arg_0_ac;
  spec::ScalarType in_2 = arg_1_ac;

  AdpfloatType<8,3> in_1_adpfloat(in_1);
  AdpfloatType<8,3> in_2_adpfloat(in_2);
  spec::AccumScalarType acc_tmp;
  adpfloat_mul(in_1_adpfloat, in_2_adpfloat, acc_tmp);

  sc_bigint<32> result_s = acc_tmp.to_int();
  sc_biguint<32> result = result_s;

  return result;
}

// Function: AccumAdd
// used to add accum_tmp to accumulate register
sc_biguint<32> flex_sim::AccumAdd(sc_biguint<32> arg_0, sc_biguint<32> arg_1) {

  sc_bigint<32> arg_0_s = arg_0;
  sc_bigint<32> arg_1_s = arg_1;
  
  ac_int<32,true> arg_0_ac = arg_0_s.to_int();
  ac_int<32,true> arg_1_ac = arg_1_s.to_int();

  spec::AccumScalarType out_tmp = arg_0_ac;
  spec::AccumScalarType acc_tmp = arg_1_ac;

  out_tmp += acc_tmp;

  sc_bigint<32> result_s = out_tmp.to_int();
  sc_biguint<32> result = result_s;

  return result;
}

// Function: AccumAdd2
// used to update values in accumulator vector registers
sc_biguint<32> flex_sim::AccumAdd2(sc_biguint<32> arg_0, sc_biguint<32> arg_1) {

  sc_bigint<32> arg_0_s = arg_0;
  sc_bigint<32> arg_1_s = arg_1;
  
  ac_int<32,true> arg_0_ac = arg_0_s.to_int();
  ac_int<32,true> arg_1_ac = arg_1_s.to_int();

  spec::AccumScalarType out_tmp = arg_0_ac;
  spec::AccumScalarType acc_tmp = arg_1_ac;

  out_tmp += acc_tmp;

  sc_bigint<32> result_s = out_tmp.to_int();
  sc_biguint<32> result = result_s;

  return result;
}

// Function: PEActEadd
// element-wise add, used in PE_Act
sc_biguint<20> flex_sim::PEActEadd(sc_biguint<20> arg_0, sc_biguint<20> arg_1) {

  sc_bigint<20> arg_0_s = arg_0;
  sc_bigint<20> arg_1_s = arg_1;

  spec::ActScalarType arg_0_ac = arg_0_s.to_int();
  spec::ActScalarType arg_1_ac = arg_1_s.to_int();

  spec::ActScalarType out_tmp = arg_0_ac + arg_1_ac;

  sc_bigint<20> result_s = out_tmp.to_int();
  sc_biguint<20> result = result_s;
  
  //std::cout << dec << "No." << counter << " " << "PEActEadd: ";
 // std::cout << hex << "arg_0: " << arg_0 << '\t' << "arg_1: " << arg_1 << '\t';
  //std::cout << "result: " << result << std::endl;
  counter++;
  return result;
}

// Function : PEActEmul
// element-wise multiplication, used in PE_Act
sc_biguint<20> flex_sim::PEActEmul(sc_biguint<20> arg_0, sc_biguint<20> arg_1) {

  sc_bigint<20> arg_0_s = arg_0;
  sc_bigint<20> arg_1_s = arg_1;
  
  spec::ActScalarType arg_0_ac = arg_0_s.to_int();
  spec::ActScalarType arg_1_ac = arg_1_s.to_int();
  
  NVINTW(2*spec::kActWordWidth) tmp;
  tmp = arg_0_ac * arg_1_ac;
  tmp = nvhls::right_shift<NVINTW(2*spec::kActWordWidth)>(tmp, spec::kActNumFrac);
  spec::ActScalarType out_tmp = nvhls::get_slc<spec::kActWordWidth>(tmp, 0);

  sc_bigint<20> result_s = out_tmp.to_int();
  sc_biguint<20> result = result_s;

  //std::cout << dec << "No." << counter << " " << "PEActEmul: ";
  //std::cout << hex << "arg_0: " << arg_0 << '\t' << "arg_1: " << arg_1 << '\t';
  //std::cout << "result: " << result << std::endl;
  counter++;
  return result;
}

// Function: PEActSigmoid
// element-wise Sigmoid funtion, used in PE_Act
sc_biguint<20> flex_sim::PEActSigmoid(sc_biguint<20> arg_0) {

  sc_bigint<20> arg_0_s = arg_0;
  //std::cout << "arg_0_s" << arg_0_s << '\t';
  
  spec::ActScalarType arg_0_ac = arg_0_s.to_int();
  spec::ActScalarType out_tmp;

  //std::cout << "arg_0_ac: " << arg_0_ac << '\t';

  ac_fixed<spec::kActWordWidth, spec::kActNumInt, true, AC_TRN, AC_WRAP> in_ac; 
  ac_fixed<spec::kActWordWidth, spec::kActNumInt, false, AC_TRN, AC_WRAP> out_ac;
  
  in_ac.set_slc(0, arg_0_ac);

  //std::cout << "in_ac: " << in_ac << '\t';

  out_ac = ac_math::ac_sigmoid_pwl
            <ac_fixed<spec::kActWordWidth, spec::kActNumInt, false, AC_TRN, AC_WRAP> >(in_ac);

  //std::cout << "out ac: " << out_ac << '\t';

  out_tmp.set_slc(0, nvhls::get_slc<spec::kActWordWidth>(out_ac, 0));

  //std::cout << "out_tmp: " << out_tmp << '\t';

  sc_bigint<20> result_s = out_tmp.to_int();
  sc_biguint<20> result = result_s;

  //std::cout << dec << "No." << counter << " " << "PEActSigmoid: ";
  //std::cout << hex << "in " << in_ac << '\t';
  //std::cout << "out_tmp: " << out_tmp << '\t';
  //std::cout << "result: " << result << std::endl;
  counter++;
  return result;
}

// Function: PEActTanh
// element-wise Tanh funtion, used in PE_Act
sc_biguint<20> flex_sim::PEActTanh(sc_biguint<20> arg_0) {

  sc_bigint<20> arg_0_s = arg_0;
//  std::cout << "arg_0_s" << arg_0_s << '\t';

  spec::ActScalarType arg_0_ac = arg_0_s.to_int();
  spec::ActScalarType out_tmp;

//  std::cout << "arg_0_ac: " << arg_0_ac << '\t';

  ac_fixed<spec::kActWordWidth, spec::kActNumInt, true, AC_TRN, AC_WRAP> in_ac;
  ac_fixed<spec::kActWordWidth, spec::kActNumInt, false, AC_TRN, AC_WRAP> out_ac;

  in_ac.set_slc(0, arg_0_ac);

//  std::cout << "in_ac: " << in_ac << '\t';

  out_ac = ac_math::ac_tanh_pwl
             <ac_fixed<spec::kActWordWidth, spec::kActNumInt, false, AC_TRN, AC_WRAP> >(in_ac);

//  std::cout << "out ac: " << out_ac << '\t';

  out_tmp.set_slc(0, nvhls::get_slc<spec::kActWordWidth>(out_ac, 0));

//  std::cout << "out_tmp: " << out_tmp << std::endl;
  
  sc_bigint<20> result_s = out_tmp.to_int();
  sc_biguint<20> result = result_s;

  //std::cout << dec << "No." << counter << " " << "PEActTanh: ";
  //std::cout << hex << "in: " << in_ac << '\t';
  //std::cout << "out_tmp: " << out_tmp << '\t';
  //std::cout << "result: " << result << std::endl;
  counter++;
  return result;
}

// Function: PEActRelu
// element-wise Relu, used in PE_Act
sc_biguint<20> flex_sim::PEActRelu(sc_biguint<20> arg_0) {

  sc_bigint<20> arg_0_s = arg_0;

  spec::ActScalarType arg_0_ac = arg_0_s.to_int();
  spec::ActScalarType out_tmp;

  if (arg_0_ac < 0)
      out_tmp = 0;
  else 
      out_tmp = arg_0_ac;

  sc_bigint<20> result_s = out_tmp.to_int();
  sc_biguint<20> result = result_s;

  //std::cout << dec << "No." << counter << " " << "PEActRelu: ";
  //std::cout << hex << "arg_0: " << arg_0 << '\t';
  //std::cout << "result: " << result << std::endl;
  counter++;
  return result;
}

// Function: PEActOneX
// element-wise OneX, used in PE_Act
sc_biguint<20> flex_sim::PEActOnex(sc_biguint<20> arg_0) {

  sc_bigint<20> arg_0_s = arg_0;

  spec::ActScalarType arg_0_ac = arg_0_s.to_int();
  spec::ActScalarType out_tmp;

  out_tmp = (1<<spec::kActNumFrac) - arg_0_ac;

  sc_bigint<20> result_s = out_tmp.to_int();
  sc_biguint<20> result = result_s;

  //std::cout << dec << "No." << counter << " " << "PEActOnex: ";
  //std::cout << hex << "arg_0: " << arg_0 << '\t';
  //std::cout << "result: " << result << std::endl;
  counter++;
  return result;
}

// Function: Adptfloat2Fixed
// transfer Adptfloat point type to fixed point type, used in PE_Act
// Will implement another version for GB_LayerNorm
sc_biguint<20> flex_sim::Adptfloat2Fixed(sc_biguint<8> arg_0, sc_biguint<3> arg_1) {

  spec::ScalarType in = arg_0.to_uint();
  spec::AdpfloatBiasType bias = arg_1.to_uint(); 
  spec::ActScalarType out_tmp;

  AdpfloatType<spec::kAdpfloatWordWidth, spec::kAdpfloatExpWidth> input_adpfloat(in);
  out_tmp = input_adpfloat.to_fixed<spec::kActWordWidth, spec::kActNumFrac>(bias);

  sc_bigint<20> result_s = out_tmp.to_int();
  sc_bigint<20> result = result_s;

  //std::cout << dec << "No." << counter << " " << "PEAdptfloat2Fixed: ";
  //std::cout << hex << "arg_0: " << arg_0 << '\t' << "arg_1: " << arg_1 << '\t';
  //std::cout << "result: " << result << std::endl;
  counter++;

  return result;
}

// Function: Fixed2Adaptfloat
// transfer fixed point type to adaptive-float point type, used in PE_Act
// will implement another version for GB_LayerNorm
sc_biguint<8> flex_sim::Fixed2Adaptfloat(sc_biguint<20> arg_0, sc_biguint<3> arg_1) {

  sc_bigint<20> arg_0_s = arg_0;
  spec::ActScalarType in = arg_0_s.to_int();
  spec::AdpfloatBiasType bias = arg_1.to_uint();
  spec::ScalarType out_tmp;

  AdpfloatType<spec::kAdpfloatWordWidth, spec::kAdpfloatExpWidth> out_adpfloat;
  out_adpfloat.set_value_fixed<spec::kActWordWidth, spec::kActNumFrac>(in, bias);
  out_tmp = out_adpfloat.to_rawbits();

  sc_biguint<8> result = out_tmp.to_uint();

  //std::cout << dec << "No." << counter << " " << "PEFixed2Adpfloat: ";
  //std::cout << hex << "arg_0: " << arg_0 << '\t' << "arg_1: " << arg_1 << '\t';
  //std::cout << "result: " << result << std::endl;
  counter++;
  return result;
}

