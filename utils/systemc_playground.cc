#include "systemc.h"   
#include <iostream>

// using namespace std;
using namespace sc_dt;

int sc_main(int, char* []) {
    sc_biguint<8> in_1 = 0x90;
    sc_biguint<8> in_2 = 0x01;

    sc_bigint<8> in_1_s = in_1;
    sc_bigint<8> in_2_s = in_2;
    
    sc_biguint<8> out_u = in_1 + in_2;
    sc_bigint<8> out_s = in_1_s + in_2_s;
    sc_biguint<8> out_u_s = out_s;
    sc_bigint<8> out_s_u_s = out_u_s;

    cout << "unsigned add: " << dec << out_u << endl;
    cout << "signed add: " << dec << out_s << '\t' << out_u_s << '\t' << out_s_u_s << endl;

    sc_biguint<8> t_u = 0x80;
    sc_bigint<8> t_s = 0x80;

    cout << dec << t_u << '\t' << t_s << '\t' << (sc_biguint<8>) t_s << endl;

    sc_int<8> arg0 = 0xB4;
    sc_int<8> arg1 = 0xCB;
    
    cout << dec << arg0 << '\t' << arg1 << endl;
    cout << "B4 > CB ? : " << (arg0 > arg1) << endl;
    cout << "b4 > 0 ? : " << (arg0 > 0 ) << endl;
    cout << "cb > 0 ? : " << (arg1 > 0 ) << endl;
    return 0;
}

 
