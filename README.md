# Instruction-Level Abstraction (ILA) of FlexNLP

This is the ILA model of the [FlexNLP](https://github.com/ttambe/FlexNLP) accelerator (all rights reserved - Harvard University). 

## Directory Structure

-   `include/flex` contains header files for functions and macros (e.g., state name, bit-width, etc.)
    -   `top_config.h` defines the top-level AXI interface
    -   `gb_config.h` defines the GB-related modules (e.g., Control)
    -   `pe_config.h` defines the PE-related modules (e.g., ActUnit)
    -   `flex.h` provides the functions for constructing the ILA model
    -   `adpfloat_spec.h` provides parameters related to adpfloat data format used in the model
    -   `uninterpreted_func.h` defines the uninterpreted functions

-   `src` contains source files for functions to construct the ILA model
    -   `top_input.cc` defines the AXI interface inputs
    -   `config_state_<gb/pe>.cc` defines the configuration states
    -   `config_instr_<gb/pe>.cc` defines the instructions to set the configuration
    -   `arch_state_<gb/pe>.cc` defines the non-config architectural states
    -   `gb_*.cc` defines the non-config instructions of GB (e.g., zero padding)
    -   `pe_*.cc` defines the non-config instructions of PE (e.g., RNN layer sizing)
    -   `flex.cc` is the orchestrating function that construct the ILA model
    -   `util.cc` contains some helper functions

-   `app` contains the `main.cc` for applications using the ILA model

## systemc file path needed for building systemc model

You need to specify the systemc path when building the generated systemc files in build/sim_model/

1. SystemC-2.3.1a or newer is needed
2. SystemC needs to be installed using CMake and specified using c++11
3. under flexnlp-ila/build/sim_model/build, cmake .. -DCMAKE_INSTALL_PREFIX=<systemc_intall_path> (ex. /home/(user_name)/local/systemc-2.3.3) to specify the systemc path.
