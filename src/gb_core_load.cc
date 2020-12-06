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

// File: gb_core_load.cc

#include <flex/flex.h>

namespace ilang{
namespace flex{

void DefineGBCoreLoad(Ila& m) {

  { // instruction for reading data from GB large buffer
    auto instr = m.NewInstr("GB_CORE_LOAD_LARGE");
    // decode condition
    auto is_read = (~m.input(TOP_IF_WR) & m.input(TOP_IF_RD));
    auto valid_addr = (m.input(TOP_ADDR_IN) >=
                       (TOP_ADDR_BASE + GB_CORE_STORE_LARGE_ADDR_MIN)) &
                      (m.input(TOP_ADDR_IN) <=
                       (TOP_ADDR_BASE + GB_CORE_STORE_LARGE_ADDR_MAX));
    instr.SetDecode(is_read & valid_addr);

    auto base_addr =
        m.input(TOP_ADDR_IN) - TOP_ADDR_BASE - GB_CORE_STORE_LARGE_ADDR_MIN;
    auto mem = m.state(GB_CORE_LARGE_BUFFER);

    instr.SetUpdate(m.state(TOP_DATA_OUT_0), Load(mem, base_addr + 0));
    instr.SetUpdate(m.state(TOP_DATA_OUT_1), Load(mem, base_addr + 1));
    instr.SetUpdate(m.state(TOP_DATA_OUT_2), Load(mem, base_addr + 2));
    instr.SetUpdate(m.state(TOP_DATA_OUT_3), Load(mem, base_addr + 3));
    instr.SetUpdate(m.state(TOP_DATA_OUT_4), Load(mem, base_addr + 4));
    instr.SetUpdate(m.state(TOP_DATA_OUT_5), Load(mem, base_addr + 5));
    instr.SetUpdate(m.state(TOP_DATA_OUT_6), Load(mem, base_addr + 6));
    instr.SetUpdate(m.state(TOP_DATA_OUT_7), Load(mem, base_addr + 7));
    instr.SetUpdate(m.state(TOP_DATA_OUT_8), Load(mem, base_addr + 8));
    instr.SetUpdate(m.state(TOP_DATA_OUT_9), Load(mem, base_addr + 9));
    instr.SetUpdate(m.state(TOP_DATA_OUT_10), Load(mem, base_addr + 10));
    instr.SetUpdate(m.state(TOP_DATA_OUT_11), Load(mem, base_addr + 11));
    instr.SetUpdate(m.state(TOP_DATA_OUT_12), Load(mem, base_addr + 12));
    instr.SetUpdate(m.state(TOP_DATA_OUT_13), Load(mem, base_addr + 13));
    instr.SetUpdate(m.state(TOP_DATA_OUT_14), Load(mem, base_addr + 14));
    instr.SetUpdate(m.state(TOP_DATA_OUT_15), Load(mem, base_addr + 15));


    auto vir_mem = m.state("VIR_OUTPUT_MEM");
    auto vir_mem_next = vir_mem;
    
    vir_mem_next = Store(vir_mem_next, base_addr+0, Load(mem, base_addr+0));
    vir_mem_next = Store(vir_mem_next, base_addr+1, Load(mem, base_addr+1));
    vir_mem_next = Store(vir_mem_next, base_addr+2, Load(mem, base_addr+2));
    vir_mem_next = Store(vir_mem_next, base_addr+3, Load(mem, base_addr+3));
    vir_mem_next = Store(vir_mem_next, base_addr+4, Load(mem, base_addr+4));
    vir_mem_next = Store(vir_mem_next, base_addr+5, Load(mem, base_addr+5));
    vir_mem_next = Store(vir_mem_next, base_addr+6, Load(mem, base_addr+6));
    vir_mem_next = Store(vir_mem_next, base_addr+7, Load(mem, base_addr+7));
    vir_mem_next = Store(vir_mem_next, base_addr+8, Load(mem, base_addr+8));
    vir_mem_next = Store(vir_mem_next, base_addr+9, Load(mem, base_addr+9));
    vir_mem_next = Store(vir_mem_next, base_addr+10, Load(mem, base_addr+10));
    vir_mem_next = Store(vir_mem_next, base_addr+11, Load(mem, base_addr+11));
    vir_mem_next = Store(vir_mem_next, base_addr+12, Load(mem, base_addr+12));
    vir_mem_next = Store(vir_mem_next, base_addr+13, Load(mem, base_addr+13));
    vir_mem_next = Store(vir_mem_next, base_addr+14, Load(mem, base_addr+14));
    vir_mem_next = Store(vir_mem_next, base_addr+15, Load(mem, base_addr+15));

    instr.SetUpdate(vir_mem, vir_mem_next);
  }

  { // instruction for reading data from GB small buffer
    auto instr = m.NewInstr("GB_CORE_LOAD_SMALL");
    // decode condition
    auto is_read = (~m.input(TOP_IF_WR) & m.input(TOP_IF_RD));
    auto valid_addr = (m.input(TOP_ADDR_IN) >=
                       (TOP_ADDR_BASE + GB_CORE_STORE_SMALL_ADDR_MIN)) &
                      (m.input(TOP_ADDR_IN) <=
                       (TOP_ADDR_BASE + GB_CORE_STORE_SMALL_ADDR_MAX));
    instr.SetDecode(is_read & valid_addr);

    auto base_addr =
        m.input(TOP_ADDR_IN) - TOP_ADDR_BASE - GB_CORE_STORE_SMALL_ADDR_MIN;
    auto mem = m.state(GB_CORE_SMALL_BUFFER);

    instr.SetUpdate(m.state(TOP_DATA_OUT_0), Load(mem, base_addr + 0));
    instr.SetUpdate(m.state(TOP_DATA_OUT_1), Load(mem, base_addr + 1));
    instr.SetUpdate(m.state(TOP_DATA_OUT_2), Load(mem, base_addr + 2));
    instr.SetUpdate(m.state(TOP_DATA_OUT_3), Load(mem, base_addr + 3));
    instr.SetUpdate(m.state(TOP_DATA_OUT_4), Load(mem, base_addr + 4));
    instr.SetUpdate(m.state(TOP_DATA_OUT_5), Load(mem, base_addr + 5));
    instr.SetUpdate(m.state(TOP_DATA_OUT_6), Load(mem, base_addr + 6));
    instr.SetUpdate(m.state(TOP_DATA_OUT_7), Load(mem, base_addr + 7));
    instr.SetUpdate(m.state(TOP_DATA_OUT_8), Load(mem, base_addr + 8));
    instr.SetUpdate(m.state(TOP_DATA_OUT_9), Load(mem, base_addr + 9));
    instr.SetUpdate(m.state(TOP_DATA_OUT_10), Load(mem, base_addr + 10));
    instr.SetUpdate(m.state(TOP_DATA_OUT_11), Load(mem, base_addr + 11));
    instr.SetUpdate(m.state(TOP_DATA_OUT_12), Load(mem, base_addr + 12));
    instr.SetUpdate(m.state(TOP_DATA_OUT_13), Load(mem, base_addr + 13));
    instr.SetUpdate(m.state(TOP_DATA_OUT_14), Load(mem, base_addr + 14));
    instr.SetUpdate(m.state(TOP_DATA_OUT_15), Load(mem, base_addr + 15));

    auto vir_mem = m.state("VIR_OUTPUT_MEM");
    auto vir_mem_next = vir_mem;
    
    vir_mem_next = Store(vir_mem_next, base_addr+0, Load(mem, base_addr+0));
    vir_mem_next = Store(vir_mem_next, base_addr+1, Load(mem, base_addr+1));
    vir_mem_next = Store(vir_mem_next, base_addr+2, Load(mem, base_addr+2));
    vir_mem_next = Store(vir_mem_next, base_addr+3, Load(mem, base_addr+3));
    vir_mem_next = Store(vir_mem_next, base_addr+4, Load(mem, base_addr+4));
    vir_mem_next = Store(vir_mem_next, base_addr+5, Load(mem, base_addr+5));
    vir_mem_next = Store(vir_mem_next, base_addr+6, Load(mem, base_addr+6));
    vir_mem_next = Store(vir_mem_next, base_addr+7, Load(mem, base_addr+7));
    vir_mem_next = Store(vir_mem_next, base_addr+8, Load(mem, base_addr+8));
    vir_mem_next = Store(vir_mem_next, base_addr+9, Load(mem, base_addr+9));
    vir_mem_next = Store(vir_mem_next, base_addr+10, Load(mem, base_addr+10));
    vir_mem_next = Store(vir_mem_next, base_addr+11, Load(mem, base_addr+11));
    vir_mem_next = Store(vir_mem_next, base_addr+12, Load(mem, base_addr+12));
    vir_mem_next = Store(vir_mem_next, base_addr+13, Load(mem, base_addr+13));
    vir_mem_next = Store(vir_mem_next, base_addr+14, Load(mem, base_addr+14));
    vir_mem_next = Store(vir_mem_next, base_addr+15, Load(mem, base_addr+15));

    instr.SetUpdate(vir_mem, vir_mem_next);
  }
}

}
}