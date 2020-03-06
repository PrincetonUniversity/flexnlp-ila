#include <systemc>
#include <iostream>
#include <string>
#include <fstream>

#include "flex_sim.h"

// source module of the testbench
// creating signals for flex_sim model
SC_MODULE(Source) {
  sc_in<bool> clk{"clk"};
  //sc_in<bool> rst;
  
  sc_out< sc_biguint<1> > flex_wr_in;
  sc_out< sc_biguint<1> > flex_rd_in;
  sc_out< sc_biguint<32> > flex_addr_in;
  sc_out< sc_biguint<8> > flex_data_in[16];

//  void source_input();

  SC_CTOR(Source) {
    SC_THREAD(source_input);
    sensitive << clk.pos();
    //reset_signal_is(rst, true);
  }

  void source_input() {
    // reset
    flex_wr_in = 0;
    flex_rd_in = 0;
    flex_addr_in = "0x0";
    for (int i = 0; i < 16; i++) {
      flex_data_in[i] = "0x0";
    }

    wait(10, SC_NS);

    std::ifstream fin;
    std::string mode, addr, data;
    std::string data_format;
    std::string temp;
    std::string hex_hdr = "0x";
    std::string addr_format;
    
    const char *addr_c;
    const char *data_byte_c;

    wait(10, SC_NS);

    fin.open("/u/yl29/3LA/test_input.csv", ios::in);

    while(std::getline(fin, temp, ',')) {
      std::getline(fin, mode, ',');
      std::getline(fin, addr, ',');
      std::getline(fin, data, '\n');

      // cout << mode << '\t' << addr << '\t' << data << endl;
      // set the WR/RD mode
      if (mode.compare("W") == 0) {
        flex_wr_in = 1;
        flex_rd_in = 0;
      } else {
        flex_wr_in = 0;
        flex_rd_in = 1;
      }
      // extract the address
      addr = addr.substr(2);
      addr_format = "0x00" + addr;
      addr_c = addr_format.c_str();
      flex_addr_in = addr_c;

      // extract the data
      data_format.clear();
      if (data.length() <= 34) {
        data_format.append(34 - data.length(), '0');
        data_format.append(data.substr(2));
      } else {
        data_format.append(data.substr(data.length()-32));
      }

      std::string data_byte;
      for (int i = 0; i<16; i++) {
        data_byte = data_format.substr(30-2*i, 2);
        data_byte_c = ("0x" + data_byte).c_str();
 //       cout << data_byte << ' ';
        flex_data_in[i] = data_byte_c;
      }
//      cout << endl;
      
//       cout << "@" << sc_time_stamp() << " :" << '\t' 
//            << "mode:" << mode << '\t'
//            << "addr:" << addr_c << '\t'
//            << "data:" << data_format << '\t';
// //      cout << "sc_addr" << '\t'
// //           << hex << flex_addr_in << '\t'; 
// //           << "sc_mode" << '\t'
// //           << flex_wr_in << '\t'
// //           << flex_rd_in << endl;
// //      for (int j = 0; j < 16; j++) {
// //          cout << hex << flex_data_in[15-j] << ' ';
// //      }
//       cout << endl;


      wait(10, SC_NS);
    }

    // cout << "source created for testbench" << endl;
  }
};

SC_MODULE(testbench) {
  SC_HAS_PROCESS(testbench);
  flex_sim flex;
  Source src;

  sc_clock clk;
  sc_signal< sc_biguint<1> > flex_wr_signal;
  sc_signal< sc_biguint<1> > flex_rd_signal;
  sc_signal< sc_biguint<32> > flex_addr_signal;
  sc_signal< sc_biguint<8> > flex_data_signal[16];

  testbench(sc_module_name name)
  : sc_module(name),
    clk("clk", 1, SC_NS),
    src("source"),
    flex("flexnlp")
  {
    // binding the signals from the source
    src.clk(clk);
    src.flex_wr_in(flex_wr_signal);
    src.flex_rd_in(flex_rd_signal);
    src.flex_addr_in(flex_addr_signal);
    for (int i = 0; i < 16; i++) {
      src.flex_data_in[i](flex_data_signal[i]);
    }

    // binding the signals for the model
    flex.flex_sim_if_axi_rd_in(flex_rd_signal);
    flex.flex_sim_if_axi_wr_in(flex_wr_signal);
    flex.flex_sim_addr_in_in(flex_addr_signal);
    flex.flex_sim_data_in_0_in(flex_data_signal[0]);
    flex.flex_sim_data_in_1_in(flex_data_signal[1]);
    flex.flex_sim_data_in_2_in(flex_data_signal[2]);
    flex.flex_sim_data_in_3_in(flex_data_signal[3]);
    flex.flex_sim_data_in_4_in(flex_data_signal[4]);
    flex.flex_sim_data_in_5_in(flex_data_signal[5]);
    flex.flex_sim_data_in_6_in(flex_data_signal[6]);
    flex.flex_sim_data_in_7_in(flex_data_signal[7]);
    flex.flex_sim_data_in_8_in(flex_data_signal[8]);
    flex.flex_sim_data_in_9_in(flex_data_signal[9]);
    flex.flex_sim_data_in_10_in(flex_data_signal[10]);
    flex.flex_sim_data_in_11_in(flex_data_signal[11]);
    flex.flex_sim_data_in_12_in(flex_data_signal[12]);
    flex.flex_sim_data_in_13_in(flex_data_signal[13]);
    flex.flex_sim_data_in_14_in(flex_data_signal[14]);
    flex.flex_sim_data_in_15_in(flex_data_signal[15]);

    SC_THREAD(run);
  }

  void run() {
    int i = 0;
    bool undone = true;
    int stop_addr = 0xdead;
    std::ofstream fout;
    fout.open("./test_output.txt", ofstream::out | ofstream::trunc);
    
    wait(10, SC_NS);
    std::cout << "@" << sc_time_stamp() << " ********* simulation start *********" << std::endl;
    wait(10, SC_NS);

    while (undone) {
      if (flex.flex_sim_addr_in.to_int() == stop_addr) {
          undone = false;
      }
/*
      fout << "@ " << sc_time_stamp() << '\t';
      fout << "is write? :" << '\t' << flex.flex_sim_if_axi_wr_in << '\t';
      fout << "addr in:" << '\t' << hex << flex.flex_sim_addr_in << '\t';
      fout << "data in:" << '\t';
      for (int k=0; k < 16; k++) {
        fout << hex << flex_data_signal[15-k] << ' ';
      }
      fout << endl;
      fout << "flex status:" << '\t';
      fout << "reduce valid: " << '\t' << flex.flex_sim_gb_layer_reduce_is_valid << '\t';
      fout << "grouping num: " << '\t' << flex.flex_sim_gb_layer_reduce_grouping_num << '\n' << endl;
    fout << "num of timestep: " << flex.flex_sim_gb_layer_reduce_num_timestep_1 << endl;
    fout << "num of vector: " << flex.flex_sim_gb_layer_reduce_num_vector_1 << endl;
*/
      wait(10, SC_NS);
    }

    wait(10000, SC_NS);
    fout << "********* output for large buffer ***********" << endl;
    fout << "num of grouping: " << flex.flex_sim_gb_layer_reduce_grouping_num << endl;
    fout << "num of timestep: " << flex.flex_sim_gb_layer_reduce_num_timestep_1 << endl;
    fout << "num of vector: " << flex.flex_sim_gb_layer_reduce_num_vector_1 << endl;

    int entry_addr;
    int index;
    for (int j = 0; j < 0x040; j++) {
        entry_addr = 16*j;
        fout << "large buffer @ addr:" << '\t';
        fout << hex << entry_addr << '\t';
        fout << "data:" << '\t';
        for (int k = 0; k < 16; k++) {
            index = 16*j + 15 - k;
            fout << hex << flex.flex_sim_gb_core_large_buffer[index] << ' ';
        }
        fout << endl;
    }

    wait(1000, SC_NS);
    //cout << "test for accessing flex:  " << hex << flex.flex_sim_gb_core_large_buffer[12] << endl;
    //cout << "test for uninterpreted function" << endl;
    sc_biguint<8> in0 = "0x98";
    sc_biguint<8> in1 = "0x0c";
    //cout << "test in0: " << (in0.to_int() >> 7) << '\t' << "test in1: " << (in1.to_int() >> 7) << endl;
    //cout << hex << ((~in0.to_int() + 1) & 127 ) << '\t' << ((~in1.to_int() + 1) & 127 )<< endl;
    //cout << hex << (((~in0.to_int() + 1) & 127 ) > ((~in1.to_int() + 1) & 127 ))<< endl;

    std::cout << "@" << sc_time_stamp() << " *********     sc_stop      *********" << std::endl;
    sc_stop();
  }
};

int sc_main(int argc, char *argv[]) {
  cout << "test started" << endl;
  testbench tb("tb");
  sc_start();
  return 0;
}

