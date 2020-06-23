cmake_minimum_required(VERSION 3.9.6)

# ---------------------------------------------------------------------------- #
# PROJECT
# name version language
# ---------------------------------------------------------------------------- #
project(flex VERSION 0.1 LANGUAGES CXX)

# ---------------------------------------------------------------------------- #
# VARIABLE
# alias and configurations
# ---------------------------------------------------------------------------- #
set(MyTarget ${PROJECT_NAME})

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# ---------------------------------------------------------------------------- #
# External dependencies
# ---------------------------------------------------------------------------- #
##
## ilang
##
find_package(ilang REQUIRED)

# ---------------------------------------------------------------------------- #
# TARGET
# executable
# ---------------------------------------------------------------------------- #
add_executable(${MyTarget} 
  app/main.cc
  src/arch_state_shared.cc
  src/arch_state_gb.cc
  src/arch_state_pe.cc
  src/config_instr_gb.cc
  src/config_instr_pe.cc
  src/config_state_gb.cc
  src/config_state_pe.cc
  src/flex.cc
  src/init_conditions.cc
  src/gb_attention.cc
  src/gb_control.cc
  src/gb_core_store.cc
  src/gb_layer_norm.cc
# src/gb_layer_reduce.cc
  src/gb_layer_reduce_fsm.cc
  src/gb_zero_padding.cc
  src/gb_internal.cc
  src/pe_act.cc
  src/pe_core.cc
  src/pe_store.cc
  src/pe_rnn_layer_sizing.cc
  src/pe_internal.cc
  src/top_input.cc
  src/util.cc
)

target_include_directories(${MyTarget} PRIVATE include)

target_link_libraries(${MyTarget} ilang::ilang)
