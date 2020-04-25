#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

int main() {
    cout << "please enter the start address for read" << endl;
    string start_addr_str;
    long long int start_addr;
    cin >> start_addr_str;
    // cout << start_addr;
    start_addr = stoi(start_addr_str, nullptr, 16);
    cout << hex << start_addr << endl;
    while ((start_addr < 0x33000000) || (start_addr > 0x38000000)) {
        cout << "invalid start addr" << endl;
        cin >> start_addr_str;
        start_addr = stoi(start_addr_str, nullptr, 16);
    }
    cout << "please enter the end addr for read" << endl;

    string end_addr_str;
    cin >> end_addr_str;
    long long int end_addr;
    end_addr = stoi(end_addr_str, nullptr, 16);

    while ((end_addr < 0x33000000) || (end_addr > 0x38000000)) {
        cout << "invalid end addr" << endl;
        cin >> end_addr_str;
        end_addr = stoi(end_addr_str, nullptr, 16);
    }

    long long int i;
    stringstream fs;
    for (i = start_addr; i <= end_addr; i+=16) {
        fs << "2,R,0x" << hex << i << ",0x0\n";
    }

    ifstream fin;
    fin.open("./axi_commands_for_maxpool.csv", ios::in);

    ofstream fout;
    fout.open("./axi_commands_for_maxpool_with_read_commands.txt", ofstream::out | ofstream::trunc);
    fout << fin.rdbuf();
    fout << "2,Q,0x00000000,0x0\n";
    fout << fs.rdbuf();
    fout << "2,W,0x00000000,0x0" << endl;
    fout.close();

    return 0;
    
}