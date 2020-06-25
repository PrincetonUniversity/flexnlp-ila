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

// File: top_config.h

#ifndef FLEX_TOP_CONFIG_H__
#define FLEX_TOP_CONFIG_H__

namespace ilang {

namespace flex {

// AXI interface
//
// - cmod/Top/Top.h
// Current AXI Format (detain is in AxiSpec.h)
// AXI: data width = 128, addr width = 32 (0x12345678)
// RVA: data width = 128, addr width = 24
// This means that each Partitation has 24 bit address (0x345678), and
// within 24 bits, 20 bits are used to locate major units, e.g. Weight Buffer,
// Config
//
// - FlexNLP_ISA.ppt
// Each 128-bit vector corresponds to 16 AXI addresses => last 4-bits of AXI
// address is always zero.
// Each configuration variable may occupy 1 or 2 bytes in
// a 128-bit vector

#define TOP_IF_WR "if_axi_wr"
#define TOP_IF_WR_WIDTH 1

#define TOP_IF_RD "if_axi_rd"
#define TOP_IF_RD_WIDTH 1

#define TOP_ADDR_IN "addr_in"
#define TOP_ADDR_IN_WIDTH 32

#define TOP_DATA_IN_0 "data_in_0"
#define TOP_DATA_IN_1 "data_in_1"
#define TOP_DATA_IN_2 "data_in_2"
#define TOP_DATA_IN_3 "data_in_3"
#define TOP_DATA_IN_4 "data_in_4"
#define TOP_DATA_IN_5 "data_in_5"
#define TOP_DATA_IN_6 "data_in_6"
#define TOP_DATA_IN_7 "data_in_7"
#define TOP_DATA_IN_8 "data_in_8"
#define TOP_DATA_IN_9 "data_in_9"
#define TOP_DATA_IN_10 "data_in_10"
#define TOP_DATA_IN_11 "data_in_11"
#define TOP_DATA_IN_12 "data_in_12"
#define TOP_DATA_IN_13 "data_in_13"
#define TOP_DATA_IN_14 "data_in_14"
#define TOP_DATA_IN_15 "data_in_15"

#define TOP_DATA_IN_WIDTH 8

// AXI address mapping
//
// - cmod/Top/Top.h
// For AxiSplitter
// GBPartition will use 0x10000000 ~ 0x10FFFFFF
// PEPartition will use 0x1i000000 ~ 0x1iFFFFFF, for 1<=i<=kNumPE
//
// update: change from 0x10000000 ~ 0x33000000

// FIXME: The address is inconsistent with the specs.
// The start address of GB buffer should be 0x33500000
#define TOP_ADDR_BASE 0x33000000

// #define TOP_GB_ADDR_MIN (TOP_ADDR_BASE + 0x10000000) // 0x43000000
// #define TOP_GB_ADDR_MAX (TOP_ADDR_BASE + 0x10FFFFFF)

#define TOP_GB_ADDR_MIN TOP_ADDR_BASE
#define TOP_GB_ADDR_MAX (TOP_ADDR_BASE + 0x00FFFFFF)

#define TOP_PE0_ADDR_OFFSET 0x01000000
#define TOP_PE0_ADDR_MIN (TOP_ADDR_BASE + 0x01000000)
#define TOP_PE0_ADDR_MAX (TOP_ADDR_BASE + 0x01FFFFFF)

#define TOP_PE1_ADDR_OFFSET 0x02000000
#define TOP_PE1_ADDR_MIN (TOP_ADDR_BASE + 0x02000000)
#define TOP_PE1_ADDR_MAX (TOP_ADDR_BASE + 0x02FFFFFF)

#define TOP_PE2_ADDR_OFFSET 0x03000000
#define TOP_PE2_ADDR_MIN (TOP_ADDR_BASE + 0x03000000)
#define TOP_PE2_ADDR_MAX (TOP_ADDR_BASE + 0x03FFFFFF)

#define TOP_PE3_ADDR_OFFSET 0x04000000
#define TOP_PE3_ADDR_MIN (TOP_ADDR_BASE + 0x04000000)
#define TOP_PE3_ADDR_MAX (TOP_ADDR_BASE + 0x04FFFFFF)

} // namespace flex

} // namespace ilang

#endif // FLEX_TOP_CONFIG_H__
