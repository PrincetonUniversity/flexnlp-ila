SYSTEMC_DIR=/usr/local/Cellar/systemc/2.3.2

cd build
make
mkdir sim_model/
./flex ./sim_model/ $SYSTEMC_DIR
cd co_sim_model/
chmod +x ./mk.sh
./mk.sh