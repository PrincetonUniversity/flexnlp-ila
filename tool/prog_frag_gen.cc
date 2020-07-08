#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <list>

#include "json_helper/nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

int main() {
  cout << "which json file to generate? (f/r/am) " << endl;
  string tmp;
  cin >> tmp;
  
  std::list<string> type_list{"f", "r", "am"};

  while (find(begin(type_list), end(type_list), tmp) == end(type_list)) {
    cout << "invalid program fragment type. Please re-enter!" << endl;
    cin >> tmp;
  }

  string file_in_path;

  cout << "please specify the test_input file path" << endl;
  cin >> file_in_path;

  string file_out_path;
  cout << "please specify the output path with file name" << endl;
  cin >> file_out_path;

  // generate program fragment from flexnlp testbench input
  if (tmp == "f") {
    /*
    while (file_in_path.find("flex") == string::npos) {
      cout << "wrong test_input file given" << endl;
      cin >> file_in_path;
    }
*/
    int instr_cntr = 0;
    
    ifstream fin;
    fin.open(file_in_path, ios::in);

    json json_out;
    vector<json> command_v;
    
    string temp, mode, addr, data;
  
    while(getline(fin, temp, ',')) {
      getline(fin, mode, ',');
      getline(fin, addr, ',');
      getline(fin, data, '\n');

      if (addr == "0xdeaddead")
        break;
      if (stoi(addr, nullptr, 16) == 0x0dead) {
        break;
      }
      // json command;
      if (mode == "W") {
        json command;
        command = {{"_instr_No.", instr_cntr},
                   {"is_rd", "0"}, {"is_wr", "1"}, {"addr", addr}, {"data", data}};
        command_v.push_back(command);
        instr_cntr++;
      } else if (mode == "R") {
        json command;
        command = {{"_instr_No.", instr_cntr},
                   {"is_rd", "1"}, {"is_wr", "0"}, {"addr", addr}, {"data", data}};
        command_v.push_back(command);
        instr_cntr++;
      } else {
        cout << "UNSUPPORTED TYPE!!!" << endl;
      }
    }

    json_out["command inputs"] = command_v;
    // cout << setw(4) << json_out;
    ofstream fout;
    //fout.open("./prog_frag/prog_frag_flex.json", ios::out | ios::trunc);
    fout.open(file_out_path, ios::out | ios::trunc);
    fout << setw(4) << json_out;
    fout.close();
    fin.close();
  }

  // generate program fragment from relay testbench input
  if (tmp == "r") {
    /*
    while (file_in_path.find("relay") == string::npos) {
      cout << "wrong test_input file given" << endl;
      cin >> file_in_path;
    }
*/
    ifstream fin;
    fin.open(file_in_path, ios::in);

    json json_out;
    vector<json> command_v;
    // string 
    string func_run, func_id, data_in_y, data_in_x, pool_y, pool_x, stride_y, stride_x;
    string temp;

    int instr_cntr = 0;

    while(getline(fin, temp, ',')) {
      getline(fin, func_run, ',');
      getline(fin, func_id, ',');
      getline(fin, data_in_y, ',');
      getline(fin, data_in_x, ',');
      getline(fin, pool_y, ',');
      getline(fin, pool_x, ',');
      getline(fin, stride_y, ',');
      getline(fin, stride_x, '\n');

      if (stoi(data_in_y, nullptr, 16) == 0x0dead)
        break;
      
      // maxpooling instruction
      if (func_id.compare("1") == 0) {
        json command;
        // command = {{"func_run", func_run}, {"func_id", func_id}, {"data_in", "0"},
        //            {"data_in_y", data_in_y}, {"data_in_x", data_in_x},
        //            {"pool_size_y", pool_y}, {"pool_size_x", pool_x},
        //            {"stride_y", stride_y}, {"stride_x", stride_x},
        //            {"data_in_batch", "N/A"}, {"data_in_channel", "N/A"},
        //            {"padding_y", "N/A"}, {"padding_x", "N/A"},
        //            {"layout", "N/A"}, {"ceil_mode", "N/A"}};
        command = {{"_instr_No.", instr_cntr},
                   {"func_run", func_run}, {"func_id", func_id}, {"data_in", "0"},
                   {"data_in_y", data_in_y}, {"data_in_x", data_in_x},
                   {"pool_size_y", pool_y}, {"pool_size_x", pool_x},
                   {"stride_y", stride_y}, {"stride_x", stride_x}};
        command_v.push_back(command);
        instr_cntr++;
      } 
      else if (func_id.compare("2") == 0) {
        json command;
        // command = {{"func_run", func_run}, {"func_id", func_id}, {"data_in", data_in_x},
        //     {"data_in_y", data_in_y}, {"data_in_x", "0"},
        //     {"pool_size_y", pool_y}, {"pool_size_x", pool_x},
        //     {"stride_y", stride_y}, {"stride_x", stride_x},
        //     {"data_in_batch", "N/A"}, {"data_in_channel", "N/A"},
        //     {"padding_y", "N/A"}, {"padding_x", "N/A"},
        //     {"layout", "N/A"}, {"ceil_mode", "N/A"}};
        command = { {"_instr_No.", instr_cntr},
                    {"func_run", func_run}, {"func_id", func_id}, {"data_in", data_in_x},
                    {"data_in_y", data_in_y}, {"data_in_x", "0"},
                    {"pool_size_y", pool_y}, {"pool_size_x", pool_x},
                    {"stride_y", stride_y}, {"stride_x", stride_x}};
        command_v.push_back(command);
        instr_cntr++;
      } else {
        cout << "unsupported function for relay" << endl;
      }
      
    }
    
    json_out["command inputs"] = command_v;
    // cout << setw(4) << json_out;
    ofstream fout;
 //   fout.open("./prog_frag/prog_frag_relay.json", ios::out | ios::trunc);
    fout.open(file_out_path, ios::out | ios::trunc);
    fout << setw(4) << json_out;
    fout.close();
    fin.close();
    
  }
  
  // generate json file for address mapping
  if (tmp == "am") {
    while (file_in_path.find("out") == string::npos) {
      cout << "wrong test_input file given" << endl;
      cin >> file_in_path;
    }
    ifstream fin;
    fin.open(file_in_path, ios::in);

    json json_out;
    vector<json> json_object_v;

    string line_in;
    string relay_addr, flex_addr;

    while(getline(fin, line_in, '\n')) {
      if ( (line_in.find("relay") == string::npos) ||
           (line_in.find("flex") == string::npos)) {
        continue;
      }
      int r_idx_l = line_in.find("relay");
      int r_idx_h = r_idx_l + 7;
      int f_idx_l = line_in.find("flex");
      int f_idx_h = f_idx_l + 9;

      relay_addr = line_in.substr(r_idx_h, f_idx_l - r_idx_h - 1);
      flex_addr = line_in.substr(f_idx_h, 10);

      json addr_pair = { {"relay_addr", relay_addr}, {"flex_addr", flex_addr} };
      json_object_v.push_back(addr_pair);
    }

    json_out["address mapping"] = json_object_v;
    ofstream fout;
    //fout.open("./addr_mapping.json", ios::out | ios::trunc );
    fout.open(file_out_path, ios::out | ios::trunc);
    fout << setw(4) << json_out;
    fout.close();
    fin.close();
  }

}
