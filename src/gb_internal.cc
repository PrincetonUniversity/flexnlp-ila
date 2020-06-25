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

// File: gb_internal.cc

#include <ilang/ilang++.h>

#include <flex/gb_config.h>
#include <flex/top_config.h>

namespace ilang {

namespace flex {

void DefineGBInternalStates(Ila& m) {

  /******** GBLayerReduce internal architectural states ********/
  // state for total grouping number
  m.NewBvState(GB_LAYER_REDUCE_GROUPING_NUM,
               GB_LAYER_REDUCE_GROUPING_NUM_WIDTH);
  // state for current grouping index
  m.NewBvState(GB_LAYER_REDUCE_GROUPING_INDEX,
               GB_LAYER_REDUCE_GROUPING_INDEX_WIDTH);
  // state for recording the remaining timestep counts at the last grouping
  m.NewBvState(GB_LAYER_REDUCE_GROUPING_REM,
               GB_LAYER_REDUCE_GROUPING_REM_WIDTH);

  // states for iterations, iterations = num_timestep/2
  m.NewBvState(GB_LAYER_REDUCE_ITERATIONS, GB_LAYER_REDUCE_ITERATIONS_WIDTH);
  // states for holding the minimun address of the in the large buffer
  m.NewBvState(GB_LAYER_REDUCE_MEMORY_MIN_ADDR_OFFSET,
               GB_LAYER_REDUCE_MEMORY_MIN_ADDR_OFFSET_WIDTH);
  // states for holding the block size in the layer reduction
  m.NewBvState(GB_LAYER_REDUCE_MEMORY_BLOCK_SIZE,
               GB_LAYER_REDUCE_MEMORY_BLOCK_SIZE_WIDTH);

  // states for holding timestep size
  m.NewBvState(GB_LAYER_REDUCE_TIMESTEP_SIZE,
               GB_LAYER_REDUCE_TIMESTEP_SIZE_WIDTH);

  // state for time_step level pooling counter
  m.NewBvState(GB_LAYER_REDUCE_TIME_STEP_OP_CNTR,
               GB_LAYER_REDUCE_TIME_STEP_OP_CNTR_WIDTH);

  // states for flags controlling child sequence
  m.NewBvState(GB_LAYER_REDUCE_START_FLAG, GB_LAYER_REDUCE_START_FLAG_WIDTH);
  m.NewBvState(GB_LAYER_REDUCE_GROUP_LEVEL_FLAG,
               GB_LAYER_REDUCE_GROUP_LEVEL_FLAG_WIDTH);
  m.NewBvState(GB_LAYER_REDUCE_TIMESTEP_LEVEL_FLAG,
               GB_LAYER_REDUCE_TIMESTEP_LEVEL_FLAG_WIDTH);
  m.NewBvState(GB_LAYER_REDUCE_VECTOR_LEVEL_FLAG,
               GB_LAYER_REDUCE_VECTOR_LEVEL_FLAG_WIDTH);
  m.NewBvState(GB_LAYER_REDUCE_BYTE_LEVEL_FLAG,
               GB_LAYER_REDUCE_BYTE_LEVEL_FLAG_WIDTH);

  // state for setting the flag of child valid state
  m.NewBvState(GB_LAYER_REDUCE_CHILD_VALID_FLAG,
               GB_LAYER_REDUCE_CHILD_VALID_FLAG_BITWIDTH);
  // state for child model FSM
  m.NewBvState(GB_LAYER_REDUCE_CHILD_STATE,
               GB_LAYER_REDUCE_CHILD_STATE_BITWIDTH);

  /******** GBLayerNorm internal states ********/
  // state for setting flag of child valid state
  m.NewBvState(GB_LAYER_NORM_CHILD_VALID_FLAG,
               GB_LAYER_NORM_CHILD_VALID_FLAG_BITWIDTH);
  // state for child model state machine
  m.NewBvState(GB_LAYER_NORM_CHILD_STATE, GB_LAYER_NORM_CHILD_STATE_BITWIDTH);

  /******* GBLayerZeroPadding internal states ******/

  // state for setting flag of child valid states
  m.NewBvState(GB_ZERO_PADDING_CHILD_VALID_FLAG,
               GB_ZERO_PADDING_CHILD_VALID_FLAG_BITWIDTH);
  // state for child model state machine
  m.NewBvState(GB_ZERO_PADDING_CHILD_STATE,
               GB_ZERO_PADDING_CHILD_STATE_BITWIDTH);

  /******* GBControl internal states ********/
  // state for setting flag of child valid
  m.NewBvState(GB_CONTROL_CHILD_VALID_FLAG,
               GB_CONTROL_CHILD_VALID_FLAG_BITWIDTH);
  // state for child model state machine
  m.NewBvState(GB_CONTROL_CHILD_STATE, GB_CONTROL_CHILD_STATE_BITWIDTH);

  /******* GBAttention internal states ******/
  // state for setting the flag of child valid
  m.NewBvState(GB_ATTENTION_CHILD_VALID_FLAG,
               GB_ATTENTION_CHILD_VALID_FLAG_BITWIDTH);
  // state for child model FSM
  m.NewBvState(GB_ATTENTION_CHILD_STATE, GB_ATTENTION_CHILD_STATE_BITWIDTH);
}

} // namespace flex

} // namespace ilang
