#include <systemc.h>
#include <iostream>
#include <string>

#include "../build/sim_model/flex_sim.h"

// source module of the testbench
// creating signals for flex_sim model
SC_MODULE(Source) {
  sc_in<bool> clk;
  sc_in<bool> rst;
  
  sc_out<sc_biguint<1>> flex_wr_in;
  sc_out<sc_biguint<1>> flex_rd_in;
  sc_out<sc_biguint<32>> flex_addr_in;
  sc_out<sc_biguint<8>> flex_data_in[16];

  void source_input();

  SC_CTOR(Source) {
    SC_THREAD(source_input);
    sensitive << clk.pos();
    reset_signal_is(rst, true);
  }

  void source_intput() {
    // reset
    flex_wr_in = 0;
    flex_rd_in = 0;
    flex_addr_in = "0x0";
    for (int i = 0; i < 16; i++) {
      flex_data_in[i] = "0x0";
    }

    wait(10);

    std::ifstream fin;
    std::string mode, addr, data;
    std::string data_format;
    std::string temp;
    std::string hex_hdr = "0x";

    while(std::getline(fin, temp, ',')) {
      std::getline(fin, mode, ',');
      std::getline(fin, addr, ',');
      std::getline(fin, data, '\n');
      // set the WR/RD mode
      if (mode.compare("W")) {
        flex_wr_in = 1;
        flex_rd_in = 0;
      } else {
        flex_wr_in = 0;
        flex_rd_in = 1;
      }
      // extract the address
      addr = addr.substr(addr.length() - 6, addr.length());
      const char *addr_c = ("0x" + addr).c_str();
      flex_addr_in = addr_c;

      // extract the data
      data_format.clear();
      data_format.append(34 - data.length(), '0');
      data_format.append(data.substr(2));
      std::string data_byte;
      const char *data_byte_c;
      for (int i = 0; i<16; i++) {
        data_byte = data_format.substr(30-2*i, 32-2*i);
        data_byte_c = ("0x" + data_byte).c_str();
        flex_data_in[i] = data_byte_c;
      }

      wait(10);
    }
  }
};

SC_MODULE(testbench) {
  flex_sim *flex;
  Source *src;

  sc_signal<sc_biguint<1>> flex_wr_signal;
  sc_signal<sc_biguint<1>> flex_rd_signal;
  sc_signal<sc_biguint<32>> flex_addr_signal;
  sc_signal<sc_biguint<8>> flex_data_signal[16];

  sc_clock clk_sig;

  SC_CTOR(testbench): clk_sig ("clk", 1, SC_NS) {
    flex = new flex_sim("flex_sim");
    src = new Source("src");

    // binding source ports to wires
    src->clk(clk_sig);
    src->flex_wr_in(flex_wr_signal);
    src->flex_rd_in(flex_rd_signal);
    src->flex_addr_in(flex_addr_signal);
    for (int i = 0; i < 16; i++) {
      src->flex_data_in[i](flex_data_signal[i]);
    }

    // binding wires to FlexNLP model
    flex->flex_sim_if_axi_rd_in(flex_rd_signal);
    flex->flex_sim_if_axi_wr_in(flex_wr_signal);
    flex->flex_sim_addr_in_in(flex_addr_signal);
    flex->flex_sim_data_in_0_in(flex_data_signal[0]);
    flex->flex_sim_data_in_1_in(flex_data_signal[1]);
    flex->flex_sim_data_in_2_in(flex_data_signal[2]);
    flex->flex_sim_data_in_3_in(flex_data_signal[3]);
    flex->flex_sim_data_in_4_in(flex_data_signal[4]);
    flex->flex_sim_data_in_5_in(flex_data_signal[5]);
    flex->flex_sim_data_in_6_in(flex_data_signal[6]);
    flex->flex_sim_data_in_7_in(flex_data_signal[7]);
    flex->flex_sim_data_in_8_in(flex_data_signal[8]);
    flex->flex_sim_data_in_9_in(flex_data_signal[9]);
    flex->flex_sim_data_in_10_in(flex_data_signal[10]);
    flex->flex_sim_data_in_11_in(flex_data_signal[11]);
    flex->flex_sim_data_in_12_in(flex_data_signal[12]);
    flex->flex_sim_data_in_13_in(flex_data_signal[13]);
    flex->flex_sim_data_in_14_in(flex_data_signal[14]);
    flex->flex_sim_data_in_15_in(flex_data_signal[15]);
  }

  ~testbench() {
    delete flex;
    delete src;
  }
  
};

int sc_main(int argc, char *argv[]) {
  testbench tb("tb");
  sc_start();
  return;
}

