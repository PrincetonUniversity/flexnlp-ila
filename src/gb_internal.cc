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

#include <flex/gb_config.h>
#include <flex/top_config.h>

#include <ilang/ilang++.h>

namespace ilang {
  // GB Layer Reduce Internal States
  // states for iterations, iterations = num_timestep/2
  m.NewState(GB_LAYER_REDUCE_ITERATIONS, GB_LAYER_REDUCE_ITERATIONS_WIDTH);
  // states for holding the minimun address of the in the large buffer
  m.NewState(GB_LAYER_REDUCE_MEMORY_MIN_ADDR_OFFSET,
                GB_LAYER_REDUCE_MEMORY_MIN_ADDR_OFFSET_WIDTH);
  // states for holding the block size in the layer reduction
  m.NewState(GB_LAYER_REDUCE_MEMORY_BLOCK_SIZE,
                GB_LAYER_REDUCE_MEMORY_BLOCK_SIZE_WIDTH);

  // state for time_step level pooling counter
  m.NewState(GB_LAYER_REDUCE_TIME_STEP_OP_CNTR, 
                GB_LAYER_REDUCE_TIME_STEP_OP_CNTR_WIDTH);



};