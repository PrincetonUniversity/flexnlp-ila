rm -rf *.o
export SYSTEMC_DIR=/home/leeoh/local/systemc-2.3.3
g++ -I $SYSTEMC_DIR/include -L $SYSTEMC_DIR/lib -Wl,-rpath=$SYSTEMC_DIR/lib -std=c++11 -o sc_pg.o systemc_playground.cc -lsystemc
./sc_pg.o
