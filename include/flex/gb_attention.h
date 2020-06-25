// =============================================================================
// MIT License
//
// Copyright (c) 2019 Princeton University
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// =============================================================================

// File: gb_attention.h

#ifndef FLEX_GB_ATTENTION_H__
#define FLEX_GB_ATTENTION_H__

#include <flex/adpfloat_spec.h>

namespace ilang {

namespace flex {

// config register: is_valid
#define GB_ATTENTION_CONFIG_REG_IS_VALID "gb_attention_is_valid"
#define GB_ATTENTION_CONFIG_REG_IS_VALID_WIDTH 1

// config register: memory_index_1
#define GB_ATTENTION_CONFIG_REG_MEMORY_INDEX_1 "gb_attention_memory_index_1"
#define GB_ATTENTION_CONFIG_REG_MEMORY_INDEX_1_WIDTH 3

// config register: memory_index_2
#define GB_ATTENTION_CONFIG_REG_MEMORY_INDEX_2 "gb_attention_memory_index_2"
#define GB_ATTENTION_CONFIG_REG_MEMORY_INDEX_2_WIDTH 3

// config register: num_vector_1
#define GB_ATTENTION_CONFIG_REG_NUM_VECTOR_1 "gb_attention_num_vector_1"
#define GB_ATTENTION_CONFIG_REG_NUM_VECTOR_1_WIDTH 8

// config register: num_timestep_1
#define GB_ATTENTION_CONFIG_REG_NUM_TIMESTEP_1 "gb_attention_num_timestep_1"
#define GB_ATTENTION_CONFIG_REG_NUM_TIMESTEP_1_WIDTH 16

// config register: adpbias_1
#define GB_ATTENTION_CONFIG_REG_ADPBIAS_1 "gb_attention_adpbias_1"
#define GB_ATTENTION_CONFIG_REG_ADPBIAS_1_WIDTH 3

// config register: adpbias_2
#define GB_ATTENTION_CONFIG_REG_ADPBIAS_2 "gb_attention_adpbias_2"
#define GB_ATTENTION_CONFIG_REG_ADPBIAS_2_WIDTH 3

// config register: adpbias_3
#define GB_ATTENTION_CONFIG_REG_ADPBIAS_3 "gb_attention_adpbias_3"
#define GB_ATTENTION_CONFIG_REG_ADPBIAS_3_WIDTH 3

// config register: adpbias_4
#define GB_ATTENTION_CONFIG_REG_ADPBIAS_4 "gb_attention_adpbias_4"
#define GB_ATTENTION_CONFIG_REG_ADPBIAS_4_WIDTH 3

/************ parameters other than configurations *********/
#define GB_ATTENTION_VALID 1
#define GB_ATTENTION_INVALID 0

#define GB_ATTENTION_SOFTMAX_INDEX 7

#define GB_ATTENTION_CHILD_VALID_FLAG "gb_attention_child_valid_flag"
#define GB_ATTENTION_CHILD_VALID_FLAG_BITWIDTH 1

#define GB_ATTENTION_CHILD_STATE "gb_attention_child_state"
#define GB_ATTENTION_CHILD_STATE_BITWIDTH 5

#define GB_ATTENTION_CHILD_STATE_IDLE 0
#define GB_ATTENTION_CHILD_STATE_PREP 1
#define GB_ATTENTION_CHILD_STATE_BMM_RD 2
#define GB_ATTENTION_CHILD_STATE_BMM_TP 3
#define GB_ATTENTION_CHILD_STATE_BMM_MV 4
#define GB_ATTENTION_CHILD_STATE_NEXT_SH 5
#define GB_ATTENTION_CHILD_STATE_NEXT_MAX 6
#define GB_ATTENTION_CHILD_STATE_NEXT_WR 7
#define GB_ATTENTION_CHILD_STATE_SFM1_RD 8
#define GB_ATTENTION_CHILD_STATE_SFM1_COMP 9
#define GB_ATTENTION_CHILD_STATE_SFM2_RD 10
#define GB_ATTENTION_CHILD_STATE_SFM2_COMP0 11
#define GB_ATTENTION_CHILD_STATE_SFM2_COMP1 12
#define GB_ATTENTION_CHILD_STATE_SFM2_COMP2 13
#define GB_ATTENTION_CHILD_STATE_SFM2_COMP3 14
#define GB_ATTENTION_CHILD_STATE_SFM3 15
#define GB_ATTENTION_CHILD_STATE_OUT1 16
#define GB_ATTENTION_CHILD_STATE_OUT2 17
#define GB_ATTENTION_CHILD_STATE_FIN 18
#define GB_ATTENTION_CHILD_STATE_BMM_NEXT 19

#define GB_ATTENTION_SUM_EXP "gb_attention_sum_exp"
#define GB_ATTENTION_SUM_EXP_BITWIDTH ATTENTION_WORD_WIDTH

#define GB_ATTENTION_MAX_VALUE "gb_attention_max_value"
#define GB_ATTENTION_MAX_VALUE_BITWIDTH ATTENTION_WORD_WIDTH

#define GB_ATTENTION_ACCUM_VECTOR "gb_attention_accum_vector"
#define GB_ATTENTION_ACCUM_VECTOR_BITWIDTH ATTENTION_WORD_WIDTH

#define GB_ATTENTION_SFM_RESULT_VECTOR "gb_attention_softmax_result_vector"
#define GB_ATTENTION_SFM_RESULT_VECTOR_BITWIDTH 8

#define GB_ATTENTION_OUT_VECTOR "gb_attention_out_vector"
#define GB_ATTENTION_OUT_VECTOR_BITWIDTH 8

#define GB_ATTENTION_VECTOR "gb_attention_vector"
#define GB_ATTENTION_VECTOR_BITWIDTH ATTENTION_WORD_WIDTH
#define GB_ATTENTION_VECTOR_NUM 4

#define GB_ATTENTION_EXP_VECTOR "gb_attention_exp_vector"
#define GB_ATTENTION_EXP_VECTOR_BITWIDTH ATTENTION_WORD_WIDTH
#define GB_ATTENTION_EXP_VECTOR_NUM 4

#define GB_ATTENTION_DP0 "gb_attention_dp0"
#define GB_ATTENTION_DP0_BITWIDTH 8

#define GB_ATTENTION_DP1 "gb_attention_dp1"
#define GB_ATTENTION_DP1_BITWIDTH 8

#define GB_ATTENTION_BMM_CNTR "gb_attention_bmm_cntr"
#define GB_ATTENTION_BMM_CNTR_BITWIDTH 1

#define GB_ATTENTION_SOFTMAX_CNTR "gb_attention_softmax_cntr"
#define GB_ATTENTION_SOFTMAX_CNTR_BITWIDTH 2

#define GB_ATTENTION_VECTOR_CNTR "gb_attention_vector_cntr"
#define GB_ATTENTION_VECTOR_CNTR_BITWIDTH 8

#define GB_ATTENTION_TIMESTEP_CNTR "gb_attention_timestep_cntr"
#define GB_ATTENTION_TIMESTEP_CNTR_BITWIDTH 16

} // namespace flex

} // namespace ilang

#endif // FLEX_GB_ATTENTION_H__
