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

// File: gb_control.h

#ifndef FLEX_GB_CONTROL_H__
#define FLEX_GB_CONTROL_H__

#include <flex/gb_core.h>

namespace ilang {

namespace flex {

// config register: is_valid
#define GB_CONTROL_CONFIG_REG_IS_VALID "gb_control_is_valid"
#define GB_CONTROL_CONFIG_REG_IS_VALID_WIDTH 1

// config register: mode
#define GB_CONTROL_CONFIG_REG_MODE "gb_control_mode"
#define GB_CONTROL_CONFIG_REG_MODE_WIDTH 3

// config register: is_rnn
#define GB_CONTROL_CONFIG_REG_IS_RNN "gb_control_is_rnn"
#define GB_CONTROL_CONFIG_REG_IS_RNN_WIDTH 1

// config register: memory_index_1
#define GB_CONTROL_CONFIG_REG_MEMORY_INDEX_1 "gb_control_memory_index_1"
#define GB_CONTROL_CONFIG_REG_MEMORY_INDEX_1_WIDTH 3

// config register: memory_index_2
#define GB_CONTROL_CONFIG_REG_MEMORY_INDEX_2 "gb_control_memory_index_2"
#define GB_CONTROL_CONFIG_REG_MEMORY_INDEX_2_WIDTH 3

// config register: num_vector_1
#define GB_CONTROL_CONFIG_REG_NUM_VECTOR_1 "gb_control_num_vector_1"
#define GB_CONTROL_CONFIG_REG_NUM_VECTOR_1_WIDTH 8

// config register: num_vector_2
#define GB_CONTROL_CONFIG_REG_NUM_VECTOR_2 "gb_control_num_vector_2"
#define GB_CONTROL_CONFIG_REG_NUM_VECTOR_2_WIDTH 8

// config register: num_timestep_1
#define GB_CONTROL_CONFIG_REG_NUM_TIMESTEP_1 "gb_control_num_timestep_1"
#define GB_CONTROL_CONFIG_REG_NUM_TIMESTEP_1_WIDTH 16

/************* SHARED STATES WITH PE *****************/
// #define PE_START_SIGNAL_SHARED "pe_start_signal_shared"
#define PE_CORE_START_SIGNAL "pe_core_start_signal"
#define PE_ACT_START_SIGNAL "pe_act_start_signal"
#define ACT_START_SIGNAL "act_start_signal"
#define PE_START_SIGNAL_SHARED_BITWIDTH 1

#define PE_DONE_SIGNAL_SHARED "pe_done_signal_shared"
#define PE_DONE_SIGNAL_SHARED_BITWIDTH 1

// data_in has data (128 bit) and data_addr (8 bit: vector index in the
// timestep) divided into 16 states because ILA doesn't support 128 bit long
// state.
#define GB_CONTROL_DATA_IN "gb_control_data_in"
#define GB_CONTROL_DATA_IN_0 "gb_control_data_in_0"
#define GB_CONTROL_DATA_IN_1 "gb_control_data_in_1"
#define GB_CONTROL_DATA_IN_2 "gb_control_data_in_2"
#define GB_CONTROL_DATA_IN_3 "gb_control_data_in_3"
#define GB_CONTROL_DATA_IN_4 "gb_control_data_in_4"
#define GB_CONTROL_DATA_IN_5 "gb_control_data_in_5"
#define GB_CONTROL_DATA_IN_6 "gb_control_data_in_6"
#define GB_CONTROL_DATA_IN_7 "gb_control_data_in_7"
#define GB_CONTROL_DATA_IN_8 "gb_control_data_in_8"
#define GB_CONTROL_DATA_IN_9 "gb_control_data_in_9"
#define GB_CONTROL_DATA_IN_10 "gb_control_data_in_10"
#define GB_CONTROL_DATA_IN_11 "gb_control_data_in_11"
#define GB_CONTROL_DATA_IN_12 "gb_control_data_in_12"
#define GB_CONTROL_DATA_IN_13 "gb_control_data_in_13"
#define GB_CONTROL_DATA_IN_14 "gb_control_data_in_14"
#define GB_CONTROL_DATA_IN_15 "gb_control_data_in_15"

#define GB_CONTROL_DATA_IN_BITWIDTH 8

#define GB_CONTROL_DATA_IN_ADDR "gb_control_data_in_addr"
#define GB_CONTROL_DATA_IN_ADDR_BITWIDTH                                       \
  GB_CONTROL_CONFIG_REG_NUM_VECTOR_1_WIDTH

#define GB_CONTROL_DATA_OUT "gb_control_data_out"
#define GB_CONTROL_DATA_OUT_0 "gb_control_data_out_0"
#define GB_CONTROL_DATA_OUT_1 "gb_control_data_out_1"
#define GB_CONTROL_DATA_OUT_2 "gb_control_data_out_2"
#define GB_CONTROL_DATA_OUT_3 "gb_control_data_out_3"
#define GB_CONTROL_DATA_OUT_4 "gb_control_data_out_4"
#define GB_CONTROL_DATA_OUT_5 "gb_control_data_out_5"
#define GB_CONTROL_DATA_OUT_6 "gb_control_data_out_6"
#define GB_CONTROL_DATA_OUT_7 "gb_control_data_out_7"
#define GB_CONTROL_DATA_OUT_8 "gb_control_data_out_8"
#define GB_CONTROL_DATA_OUT_9 "gb_control_data_out_9"
#define GB_CONTROL_DATA_OUT_10 "gb_control_data_out_10"
#define GB_CONTROL_DATA_OUT_11 "gb_control_data_out_11"
#define GB_CONTROL_DATA_OUT_12 "gb_control_data_out_12"
#define GB_CONTROL_DATA_OUT_13 "gb_control_data_out_13"
#define GB_CONTROL_DATA_OUT_14 "gb_control_data_out_14"
#define GB_CONTROL_DATA_OUT_15 "gb_control_data_out_15"

#define GB_CONTROL_DATA_OUT_BITWIDTH 8

#define GB_CONTROL_DATA_OUT_ADDR "gb_control_data_out_addr"
#define GB_CONTROL_DATA_OUT_ADDR_BITWIDTH                                      \
  GB_CONTROL_CONFIG_REG_NUM_VECTOR_1_WIDTH

#define GB_CONTROL_DATA_OUT_INDEX "gb_control_data_out_index"
#define GB_CONTROL_DATA_OUT_INDEX_BITWIDTH 2

// this index is used to specify the PE manager
#define GB_CONTROL_DATA_OUT_INDEX_X 0
#define GB_CONTROL_DATA_OUT_INDEX_H 1

// valid flag for share states
#define GB_CONTROL_DATA_IN_VALID "gb_control_data_in_valid"
#define GB_CONTROL_DATA_IN_VALID_BITWIDTH 1

#define GB_CONTROL_DATA_OUT_VALID "gb_control_data_out_valid"
#define GB_CONTROL_DATA_OUT_VALID_BITWIDTH 1

#define GB_CONTROL_VALID 1
#define GB_CONTROL_INVALID 0
/************* states for child models **************/
#define GB_CONTROL_CHILD_VALID_FLAG "gb_control_child_valid_flag"
#define GB_CONTROL_CHILD_VALID_FLAG_BITWIDTH 1

#define GB_CONTROL_CHILD_ON 1
#define GB_CONTROL_CHILD_OFF 0

#define GB_CONTROL_MODE_UNI 0
#define GB_CONTROL_MODE_BI_FWD 1
#define GB_CONTROL_MODE_BI_BWD 2
#define GB_CONTROL_MODE_DECODE 3

#define GB_CONTROL_CHILD_STATE "gb_control_child_state"
#define GB_CONTROL_CHILD_STATE_BITWIDTH 4

#define GB_CONTROL_CHILD_STATE_PREP 0
#define GB_CONTROL_CHILD_STATE_SEND_PREP 1
#define GB_CONTROL_CHILD_STATE_SEND 2
#define GB_CONTROL_CHILD_STATE_PE_START 3
#define GB_CONTROL_CHILD_STATE_RECV_PREP 4
#define GB_CONTROL_CHILD_STATE_RECV 5
#define GB_CONTROL_CHILD_STATE_SEND_BACK_PREP 6
#define GB_CONTROL_CHILD_STATE_SEND_BACK 7
#define GB_CONTROL_CHILD_STATE_NEXT 8

#define GB_CONTROL_CHILD_TIMESTEP_CNTR "gb_control_child_timestep_cntr"
#define GB_CONTROL_CHILD_TIMESTEP_CNTR_BITWIDTH                                \
  GB_CONTROL_CONFIG_REG_NUM_TIMESTEP_1_WIDTH

#define GB_CONTROL_CHILD_VECTOR_CNTR "gb_control_child_vector_cntr"
#define GB_CONTROL_CHILD_VECTOR_CNTR_BITWIDTH                                  \
  GB_CONTROL_CONFIG_REG_NUM_VECTOR_1_WIDTH

#define GB_CONTROL_CHILD_TIMESTEP_INDEX "gb_control_child_timestep_index"
#define GB_CONTROL_CHILD_TIMESTEP_INDEX_BITWIDTH                               \
  GB_CONTROL_CONFIG_REG_NUM_TIMESTEP_1_WIDTH

#define GB_CONTROL_CHILD_TIMESTEP_BASE_ADDR                                    \
  "gb_control_child_timestep_base_addr"
#define GB_CONTROL_CHILD_TIMESTEP_BASE_ADDR_BITWIDTH                           \
  GB_CORE_STORE_LARGE_BITWIDTH

} // namespace flex

} // namespace ilang

#endif // FLEX_GB_CONTROL_H__
