#include <fstream>
#include <iostream>
#include <queue>
#include <string>

#include <nlohmann/json.hpp>
#include <systemc>

#include <flex.h>

using json = nlohmann::json;

typedef unsigned long long VAL_T;

typedef struct FlexCmd {
  VAL_T is_rd;
  VAL_T is_wr;
  VAL_T addr;
  VAL_T data[16];
} FlexCmd;

static std::queue<FlexCmd*> commands;

void ParseInput(const std::string& file) {
  std::ifstream fin(file);
  if (!fin.is_open()) {
    return;
  }
  json cmds;
  fin >> cmds;
  fin.close();

  auto RemoveHexPrefix = [](const std::string& org) {
    if (org.size() <= 2) {
      return org;
    }
    auto prefix = org.substr(0, 2);
    return (prefix == "0x") ? org.substr(2, org.size() - 2) : org;
  };

  auto GetHexVal = [](const std::string& str) {
    return std::stoull(str, NULL, 16);
  };

  auto ZeroPad = [](const std::string& str, const int& length) {
    if (str.size() < length) {
      return std::string(length - str.size(), '0') + str;
    }
    return str;
  };

  try {
    for (const auto& cmd : cmds.at("command inputs")) {
      auto addr_str = cmd["addr"].get<std::string>();
      auto data_str = cmd["data"].get<std::string>();
      auto is_rd_str = cmd["is_rd"].get<std::string>();
      auto is_wr_str = cmd["is_wr"].get<std::string>();

      auto new_cmd = new FlexCmd();
      new_cmd->is_rd = GetHexVal(is_rd_str);
      new_cmd->is_wr = GetHexVal(is_wr_str);
      new_cmd->addr = GetHexVal(RemoveHexPrefix(addr_str));
      data_str = RemoveHexPrefix(data_str);
      data_str = ZeroPad(data_str, 32);
      for (auto i = 0; i < 16; i++) {
        new_cmd->data[i] = GetHexVal(data_str.substr(30 - (2 * i), 2));
      }
      commands.push(new_cmd);
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "Fail parsing input command file\n";
    return;
  }
}

SC_MODULE(testbench) {
  SC_HAS_PROCESS(testbench);
  flex flex_i;

  sc_clock clk;
  sc_signal<sc_biguint<1>> flex_wr_signal;
  sc_signal<sc_biguint<1>> flex_rd_signal;
  sc_signal<sc_biguint<32>> flex_addr_signal;
  sc_signal<sc_biguint<8>> flex_data_signal[16];

  testbench(sc_module_name name)
      : sc_module(name), clk("clk", 1, SC_NS), flex_i("flexnlp") {

    // binding the signals for the model
    flex_i.flex_if_axi_rd_in(flex_rd_signal);
    flex_i.flex_if_axi_wr_in(flex_wr_signal);
    flex_i.flex_addr_in_in(flex_addr_signal);
    flex_i.flex_data_in_0_in(flex_data_signal[0]);
    flex_i.flex_data_in_1_in(flex_data_signal[1]);
    flex_i.flex_data_in_2_in(flex_data_signal[2]);
    flex_i.flex_data_in_3_in(flex_data_signal[3]);
    flex_i.flex_data_in_4_in(flex_data_signal[4]);
    flex_i.flex_data_in_5_in(flex_data_signal[5]);
    flex_i.flex_data_in_6_in(flex_data_signal[6]);
    flex_i.flex_data_in_7_in(flex_data_signal[7]);
    flex_i.flex_data_in_8_in(flex_data_signal[8]);
    flex_i.flex_data_in_9_in(flex_data_signal[9]);
    flex_i.flex_data_in_10_in(flex_data_signal[10]);
    flex_i.flex_data_in_11_in(flex_data_signal[11]);
    flex_i.flex_data_in_12_in(flex_data_signal[12]);
    flex_i.flex_data_in_13_in(flex_data_signal[13]);
    flex_i.flex_data_in_14_in(flex_data_signal[14]);
    flex_i.flex_data_in_15_in(flex_data_signal[15]);

    flex_i.instr_log;
    SC_THREAD(run);
  }

  void run() {
    std::ofstream fout;
    fout.open("./test_output.txt", ofstream::out | ofstream::trunc);

    wait(10, SC_NS);
    std::cout << "@" << sc_time_stamp()
              << " ********* simulation start *********" << std::endl;
    wait(10, SC_NS);

    while (!commands.empty()) {
      // get new input command
      auto current_cmd = commands.front();
      flex_wr_signal = current_cmd->is_wr;
      flex_rd_signal = current_cmd->is_rd;
      flex_addr_signal = current_cmd->addr;
      for (auto i = 0; i < 16; i++) {
        flex_data_signal[i] = current_cmd->data[i];
      }
      delete current_cmd;
      commands.pop();

      // debug log
      fout << "@ " << sc_time_stamp() << '\t';
      fout << "is write? :" << '\t' << flex_i.flex_if_axi_wr_in << '\t';
      fout << "addr in:" << '\t' << hex << flex_i.flex_addr_in << '\t';
      fout << "data in:" << '\t';
      for (int k = 0; k < 16; k++) {
        fout << hex << flex_data_signal[15 - k] << ' ';
      }
      fout << endl;
      fout << "flex status:" << '\t';
      fout << "reduce valid: " << '\t' << flex_i.flex_gb_layer_reduce_is_valid
           << '\t';
      fout << "grouping num: " << '\t'
           << flex_i.flex_gb_layer_reduce_grouping_num << '\n'
           << endl;

      // next
      wait(10, SC_NS);
    }

    wait(10000, SC_NS);
    fout << "********* output for large buffer ***********" << endl;

    int entry_addr;
    int index;
    for (int j = 0; j < 0xB00; j++) {
      entry_addr = 16 * j;
      fout << "large buffer @ addr:" << '\t';
      fout << hex << entry_addr << '\t';
      fout << "data:" << '\t';
      for (int k = 0; k < 16; k++) {
        index = 16 * j + 15 - k;
        fout << hex << flex_i.flex_gb_core_large_buffer[index] << ' ';
      }
      fout << endl;
    }

    std::cout << "@" << sc_time_stamp()
              << " *********     sc_stop      *********" << std::endl;
    sc_stop();
  }
};

int sc_main(int argc, char* argv[]) {
  if (argc == 2) {
    ParseInput(std::string(argv[1]));
  } else {
    std::cerr << "No input file specified\n";
    return 0;
  }

  testbench tb("tb");
  sc_start();

  return 0;
}

