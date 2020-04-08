#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
  int main() {
    ifstream fin;
    fin.open("./axi_commands_10_LSTM_timesteps_with_clustered_weights_in_4PEs_with_comments.txt",
              ios::in);
    
    stringstream f;
    string data;
    int cntr = 0;
    f.str("");

    while(getline(fin, data, '\n')) {
      if (data.substr(0,2) == "//") {
        continue;
      } 
      else if (data.length() < 2) {
        continue;
      }
      f << data << '\n';
      cntr++;
    }
    cout << cntr << endl;
    // input a end instruction at the end
    f << "2,W,0xdeaddead,0x0\n";

    ofstream fout;
    fout.open("./lstm_testbench_input.txt", ofstream::out | ofstream::trunc);
    fout << f.rdbuf();
    fout.close();
    // test the last char at the end of each line
    fin.close();
    fin.open("./lstm_testbench_input.txt", ios::in);
    string temp;
    getline(fin, temp, '\n');
    cout << "the last character of this line is " << int(temp[temp.size()-1]) << endl;
    return 0;
  
} // namespace std