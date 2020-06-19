#include "systemc.h"
#include "flex_sim.h"
void flex_sim::compute() {
  flex_sim_if_axi_wr = flex_sim_if_axi_wr_in.read();
  flex_sim_if_axi_rd = flex_sim_if_axi_rd_in.read();
  flex_sim_addr_in = flex_sim_addr_in_in.read();
  flex_sim_data_in_0 = flex_sim_data_in_0_in.read();
  flex_sim_data_in_1 = flex_sim_data_in_1_in.read();
  flex_sim_data_in_2 = flex_sim_data_in_2_in.read();
  flex_sim_data_in_3 = flex_sim_data_in_3_in.read();
  flex_sim_data_in_4 = flex_sim_data_in_4_in.read();
  flex_sim_data_in_5 = flex_sim_data_in_5_in.read();
  flex_sim_data_in_6 = flex_sim_data_in_6_in.read();
  flex_sim_data_in_7 = flex_sim_data_in_7_in.read();
  flex_sim_data_in_8 = flex_sim_data_in_8_in.read();
  flex_sim_data_in_9 = flex_sim_data_in_9_in.read();
  flex_sim_data_in_10 = flex_sim_data_in_10_in.read();
  flex_sim_data_in_11 = flex_sim_data_in_11_in.read();
  flex_sim_data_in_12 = flex_sim_data_in_12_in.read();
  flex_sim_data_in_13 = flex_sim_data_in_13_in.read();
  flex_sim_data_in_14 = flex_sim_data_in_14_in.read();
  flex_sim_data_in_15 = flex_sim_data_in_15_in.read();
  if (decode_flex_sim_CONFIG_GB_ATTENTION()) {
    sc_biguint<3> flex_sim_gb_attention_adpbias_1_next = decode_flex_sim_CONFIG_GB_ATTENTION_update_flex_sim_gb_attention_adpbias_1();
    sc_biguint<3> flex_sim_gb_attention_adpbias_2_next = decode_flex_sim_CONFIG_GB_ATTENTION_update_flex_sim_gb_attention_adpbias_2();
    sc_biguint<3> flex_sim_gb_attention_adpbias_3_next = decode_flex_sim_CONFIG_GB_ATTENTION_update_flex_sim_gb_attention_adpbias_3();
    sc_biguint<3> flex_sim_gb_attention_adpbias_4_next = decode_flex_sim_CONFIG_GB_ATTENTION_update_flex_sim_gb_attention_adpbias_4();
    sc_biguint<1> flex_sim_gb_attention_is_valid_next = decode_flex_sim_CONFIG_GB_ATTENTION_update_flex_sim_gb_attention_is_valid();
    sc_biguint<3> flex_sim_gb_attention_memory_index_1_next = decode_flex_sim_CONFIG_GB_ATTENTION_update_flex_sim_gb_attention_memory_index_1();
    sc_biguint<3> flex_sim_gb_attention_memory_index_2_next = decode_flex_sim_CONFIG_GB_ATTENTION_update_flex_sim_gb_attention_memory_index_2();
    sc_biguint<16> flex_sim_gb_attention_num_timestep_1_next = decode_flex_sim_CONFIG_GB_ATTENTION_update_flex_sim_gb_attention_num_timestep_1();
    sc_biguint<8> flex_sim_gb_attention_num_vector_1_next = decode_flex_sim_CONFIG_GB_ATTENTION_update_flex_sim_gb_attention_num_vector_1();
    flex_sim_gb_attention_adpbias_1 = flex_sim_gb_attention_adpbias_1_next;
    flex_sim_gb_attention_adpbias_2 = flex_sim_gb_attention_adpbias_2_next;
    flex_sim_gb_attention_adpbias_3 = flex_sim_gb_attention_adpbias_3_next;
    flex_sim_gb_attention_adpbias_4 = flex_sim_gb_attention_adpbias_4_next;
    flex_sim_gb_attention_is_valid = flex_sim_gb_attention_is_valid_next;
    flex_sim_gb_attention_memory_index_1 = flex_sim_gb_attention_memory_index_1_next;
    flex_sim_gb_attention_memory_index_2 = flex_sim_gb_attention_memory_index_2_next;
    flex_sim_gb_attention_num_timestep_1 = flex_sim_gb_attention_num_timestep_1_next;
    flex_sim_gb_attention_num_vector_1 = flex_sim_gb_attention_num_vector_1_next;
  }
  if (decode_flex_sim_CONFIG_GB_CONTROL()) {
    sc_biguint<1> flex_sim_gb_control_is_rnn_next = decode_flex_sim_CONFIG_GB_CONTROL_update_flex_sim_gb_control_is_rnn();
    sc_biguint<1> flex_sim_gb_control_is_valid_next = decode_flex_sim_CONFIG_GB_CONTROL_update_flex_sim_gb_control_is_valid();
    sc_biguint<3> flex_sim_gb_control_memory_index_1_next = decode_flex_sim_CONFIG_GB_CONTROL_update_flex_sim_gb_control_memory_index_1();
    sc_biguint<3> flex_sim_gb_control_memory_index_2_next = decode_flex_sim_CONFIG_GB_CONTROL_update_flex_sim_gb_control_memory_index_2();
    sc_biguint<3> flex_sim_gb_control_mode_next = decode_flex_sim_CONFIG_GB_CONTROL_update_flex_sim_gb_control_mode();
    sc_biguint<16> flex_sim_gb_control_num_timestep_1_next = decode_flex_sim_CONFIG_GB_CONTROL_update_flex_sim_gb_control_num_timestep_1();
    sc_biguint<8> flex_sim_gb_control_num_vector_1_next = decode_flex_sim_CONFIG_GB_CONTROL_update_flex_sim_gb_control_num_vector_1();
    sc_biguint<8> flex_sim_gb_control_num_vector_2_next = decode_flex_sim_CONFIG_GB_CONTROL_update_flex_sim_gb_control_num_vector_2();
    flex_sim_gb_control_is_rnn = flex_sim_gb_control_is_rnn_next;
    flex_sim_gb_control_is_valid = flex_sim_gb_control_is_valid_next;
    flex_sim_gb_control_memory_index_1 = flex_sim_gb_control_memory_index_1_next;
    flex_sim_gb_control_memory_index_2 = flex_sim_gb_control_memory_index_2_next;
    flex_sim_gb_control_mode = flex_sim_gb_control_mode_next;
    flex_sim_gb_control_num_timestep_1 = flex_sim_gb_control_num_timestep_1_next;
    flex_sim_gb_control_num_vector_1 = flex_sim_gb_control_num_vector_1_next;
    flex_sim_gb_control_num_vector_2 = flex_sim_gb_control_num_vector_2_next;
  }
  if (decode_flex_sim_CONFIG_GB_CORE_MEM_MNGR_LARGE()) {
    sc_biguint<16> flex_sim_gb_core_mem_mngr_large_base_large_0_next = decode_flex_sim_CONFIG_GB_CORE_MEM_MNGR_LARGE_update_flex_sim_gb_core_mem_mngr_large_base_large_0();
    sc_biguint<16> flex_sim_gb_core_mem_mngr_large_base_large_1_next = decode_flex_sim_CONFIG_GB_CORE_MEM_MNGR_LARGE_update_flex_sim_gb_core_mem_mngr_large_base_large_1();
    sc_biguint<16> flex_sim_gb_core_mem_mngr_large_base_large_2_next = decode_flex_sim_CONFIG_GB_CORE_MEM_MNGR_LARGE_update_flex_sim_gb_core_mem_mngr_large_base_large_2();
    sc_biguint<16> flex_sim_gb_core_mem_mngr_large_base_large_3_next = decode_flex_sim_CONFIG_GB_CORE_MEM_MNGR_LARGE_update_flex_sim_gb_core_mem_mngr_large_base_large_3();
    sc_biguint<8> flex_sim_gb_core_mem_mngr_large_num_vector_0_next = decode_flex_sim_CONFIG_GB_CORE_MEM_MNGR_LARGE_update_flex_sim_gb_core_mem_mngr_large_num_vector_0();
    sc_biguint<8> flex_sim_gb_core_mem_mngr_large_num_vector_1_next = decode_flex_sim_CONFIG_GB_CORE_MEM_MNGR_LARGE_update_flex_sim_gb_core_mem_mngr_large_num_vector_1();
    sc_biguint<8> flex_sim_gb_core_mem_mngr_large_num_vector_2_next = decode_flex_sim_CONFIG_GB_CORE_MEM_MNGR_LARGE_update_flex_sim_gb_core_mem_mngr_large_num_vector_2();
    sc_biguint<8> flex_sim_gb_core_mem_mngr_large_num_vector_3_next = decode_flex_sim_CONFIG_GB_CORE_MEM_MNGR_LARGE_update_flex_sim_gb_core_mem_mngr_large_num_vector_3();
    flex_sim_gb_core_mem_mngr_large_base_large_0 = flex_sim_gb_core_mem_mngr_large_base_large_0_next;
    flex_sim_gb_core_mem_mngr_large_base_large_1 = flex_sim_gb_core_mem_mngr_large_base_large_1_next;
    flex_sim_gb_core_mem_mngr_large_base_large_2 = flex_sim_gb_core_mem_mngr_large_base_large_2_next;
    flex_sim_gb_core_mem_mngr_large_base_large_3 = flex_sim_gb_core_mem_mngr_large_base_large_3_next;
    flex_sim_gb_core_mem_mngr_large_num_vector_0 = flex_sim_gb_core_mem_mngr_large_num_vector_0_next;
    flex_sim_gb_core_mem_mngr_large_num_vector_1 = flex_sim_gb_core_mem_mngr_large_num_vector_1_next;
    flex_sim_gb_core_mem_mngr_large_num_vector_2 = flex_sim_gb_core_mem_mngr_large_num_vector_2_next;
    flex_sim_gb_core_mem_mngr_large_num_vector_3 = flex_sim_gb_core_mem_mngr_large_num_vector_3_next;
  }
  if (decode_flex_sim_CONFIG_GB_CORE_MEM_MNGR_SMALL()) {
    sc_biguint<16> flex_sim_gb_core_mem_mngr_small_base_small_0_next = decode_flex_sim_CONFIG_GB_CORE_MEM_MNGR_SMALL_update_flex_sim_gb_core_mem_mngr_small_base_small_0();
    sc_biguint<16> flex_sim_gb_core_mem_mngr_small_base_small_1_next = decode_flex_sim_CONFIG_GB_CORE_MEM_MNGR_SMALL_update_flex_sim_gb_core_mem_mngr_small_base_small_1();
    sc_biguint<16> flex_sim_gb_core_mem_mngr_small_base_small_2_next = decode_flex_sim_CONFIG_GB_CORE_MEM_MNGR_SMALL_update_flex_sim_gb_core_mem_mngr_small_base_small_2();
    sc_biguint<16> flex_sim_gb_core_mem_mngr_small_base_small_3_next = decode_flex_sim_CONFIG_GB_CORE_MEM_MNGR_SMALL_update_flex_sim_gb_core_mem_mngr_small_base_small_3();
    sc_biguint<16> flex_sim_gb_core_mem_mngr_small_base_small_4_next = decode_flex_sim_CONFIG_GB_CORE_MEM_MNGR_SMALL_update_flex_sim_gb_core_mem_mngr_small_base_small_4();
    sc_biguint<16> flex_sim_gb_core_mem_mngr_small_base_small_5_next = decode_flex_sim_CONFIG_GB_CORE_MEM_MNGR_SMALL_update_flex_sim_gb_core_mem_mngr_small_base_small_5();
    sc_biguint<16> flex_sim_gb_core_mem_mngr_small_base_small_6_next = decode_flex_sim_CONFIG_GB_CORE_MEM_MNGR_SMALL_update_flex_sim_gb_core_mem_mngr_small_base_small_6();
    sc_biguint<16> flex_sim_gb_core_mem_mngr_small_base_small_7_next = decode_flex_sim_CONFIG_GB_CORE_MEM_MNGR_SMALL_update_flex_sim_gb_core_mem_mngr_small_base_small_7();
    flex_sim_gb_core_mem_mngr_small_base_small_0 = flex_sim_gb_core_mem_mngr_small_base_small_0_next;
    flex_sim_gb_core_mem_mngr_small_base_small_1 = flex_sim_gb_core_mem_mngr_small_base_small_1_next;
    flex_sim_gb_core_mem_mngr_small_base_small_2 = flex_sim_gb_core_mem_mngr_small_base_small_2_next;
    flex_sim_gb_core_mem_mngr_small_base_small_3 = flex_sim_gb_core_mem_mngr_small_base_small_3_next;
    flex_sim_gb_core_mem_mngr_small_base_small_4 = flex_sim_gb_core_mem_mngr_small_base_small_4_next;
    flex_sim_gb_core_mem_mngr_small_base_small_5 = flex_sim_gb_core_mem_mngr_small_base_small_5_next;
    flex_sim_gb_core_mem_mngr_small_base_small_6 = flex_sim_gb_core_mem_mngr_small_base_small_6_next;
    flex_sim_gb_core_mem_mngr_small_base_small_7 = flex_sim_gb_core_mem_mngr_small_base_small_7_next;
  }
  if (decode_flex_sim_CONFIG_GB_LAYER_NORM()) {
    sc_biguint<3> flex_sim_gb_layer_norm_adpbias_1_next = decode_flex_sim_CONFIG_GB_LAYER_NORM_update_flex_sim_gb_layer_norm_adpbias_1();
    sc_biguint<3> flex_sim_gb_layer_norm_adpbias_3_next = decode_flex_sim_CONFIG_GB_LAYER_NORM_update_flex_sim_gb_layer_norm_adpbias_3();
    sc_biguint<3> flex_sim_gb_layer_norm_adpbias_4_next = decode_flex_sim_CONFIG_GB_LAYER_NORM_update_flex_sim_gb_layer_norm_adpbias_4();
    sc_biguint<1> flex_sim_gb_layer_norm_is_valid_next = decode_flex_sim_CONFIG_GB_LAYER_NORM_update_flex_sim_gb_layer_norm_is_valid();
    sc_biguint<3> flex_sim_gb_layer_norm_memory_index_1_next = decode_flex_sim_CONFIG_GB_LAYER_NORM_update_flex_sim_gb_layer_norm_memory_index_1();
    sc_biguint<3> flex_sim_gb_layer_norm_mode_next = decode_flex_sim_CONFIG_GB_LAYER_NORM_update_flex_sim_gb_layer_norm_mode();
    sc_biguint<16> flex_sim_gb_layer_norm_num_timestep_1_next = decode_flex_sim_CONFIG_GB_LAYER_NORM_update_flex_sim_gb_layer_norm_num_timestep_1();
    sc_biguint<8> flex_sim_gb_layer_norm_num_vector_1_next = decode_flex_sim_CONFIG_GB_LAYER_NORM_update_flex_sim_gb_layer_norm_num_vector_1();
    flex_sim_gb_layer_norm_adpbias_1 = flex_sim_gb_layer_norm_adpbias_1_next;
    flex_sim_gb_layer_norm_adpbias_3 = flex_sim_gb_layer_norm_adpbias_3_next;
    flex_sim_gb_layer_norm_adpbias_4 = flex_sim_gb_layer_norm_adpbias_4_next;
    flex_sim_gb_layer_norm_is_valid = flex_sim_gb_layer_norm_is_valid_next;
    flex_sim_gb_layer_norm_memory_index_1 = flex_sim_gb_layer_norm_memory_index_1_next;
    flex_sim_gb_layer_norm_mode = flex_sim_gb_layer_norm_mode_next;
    flex_sim_gb_layer_norm_num_timestep_1 = flex_sim_gb_layer_norm_num_timestep_1_next;
    flex_sim_gb_layer_norm_num_vector_1 = flex_sim_gb_layer_norm_num_vector_1_next;
  }
  if (decode_flex_sim_CONFIG_GB_LAYER_REDUCE()) {
    sc_biguint<1> flex_sim_gb_layer_reduce_is_valid_next = decode_flex_sim_CONFIG_GB_LAYER_REDUCE_update_flex_sim_gb_layer_reduce_is_valid();
    sc_biguint<3> flex_sim_gb_layer_reduce_memory_index_1_next = decode_flex_sim_CONFIG_GB_LAYER_REDUCE_update_flex_sim_gb_layer_reduce_memory_index_1();
    sc_biguint<3> flex_sim_gb_layer_reduce_mode_next = decode_flex_sim_CONFIG_GB_LAYER_REDUCE_update_flex_sim_gb_layer_reduce_mode();
    sc_biguint<16> flex_sim_gb_layer_reduce_num_timestep_1_next = decode_flex_sim_CONFIG_GB_LAYER_REDUCE_update_flex_sim_gb_layer_reduce_num_timestep_1();
    sc_biguint<8> flex_sim_gb_layer_reduce_num_vector_1_next = decode_flex_sim_CONFIG_GB_LAYER_REDUCE_update_flex_sim_gb_layer_reduce_num_vector_1();
    flex_sim_gb_layer_reduce_is_valid = flex_sim_gb_layer_reduce_is_valid_next;
    flex_sim_gb_layer_reduce_memory_index_1 = flex_sim_gb_layer_reduce_memory_index_1_next;
    flex_sim_gb_layer_reduce_mode = flex_sim_gb_layer_reduce_mode_next;
    flex_sim_gb_layer_reduce_num_timestep_1 = flex_sim_gb_layer_reduce_num_timestep_1_next;
    flex_sim_gb_layer_reduce_num_vector_1 = flex_sim_gb_layer_reduce_num_vector_1_next;
  }
  if (decode_flex_sim_CONFIG_GB_ZERO_PADDING()) {
    sc_biguint<1> flex_sim_gb_zero_padding_is_valid_next = decode_flex_sim_CONFIG_GB_ZERO_PADDING_update_flex_sim_gb_zero_padding_is_valid();
    sc_biguint<3> flex_sim_gb_zero_padding_memory_index_1_next = decode_flex_sim_CONFIG_GB_ZERO_PADDING_update_flex_sim_gb_zero_padding_memory_index_1();
    sc_biguint<16> flex_sim_gb_zero_padding_num_timestep_1_next = decode_flex_sim_CONFIG_GB_ZERO_PADDING_update_flex_sim_gb_zero_padding_num_timestep_1();
    sc_biguint<16> flex_sim_gb_zero_padding_num_timestep_2_next = decode_flex_sim_CONFIG_GB_ZERO_PADDING_update_flex_sim_gb_zero_padding_num_timestep_2();
    sc_biguint<8> flex_sim_gb_zero_padding_num_vector_1_next = decode_flex_sim_CONFIG_GB_ZERO_PADDING_update_flex_sim_gb_zero_padding_num_vector_1();
    flex_sim_gb_zero_padding_is_valid = flex_sim_gb_zero_padding_is_valid_next;
    flex_sim_gb_zero_padding_memory_index_1 = flex_sim_gb_zero_padding_memory_index_1_next;
    flex_sim_gb_zero_padding_num_timestep_1 = flex_sim_gb_zero_padding_num_timestep_1_next;
    flex_sim_gb_zero_padding_num_timestep_2 = flex_sim_gb_zero_padding_num_timestep_2_next;
    flex_sim_gb_zero_padding_num_vector_1 = flex_sim_gb_zero_padding_num_vector_1_next;
  }
  if (decode_flex_sim_GBCore_Store_Large()) {
    decode_flex_sim_GBCore_Store_Large_update_flex_sim_gb_core_large_buffer(decode_flex_sim_GBCore_Store_Large_update_flex_sim_gb_core_large_buffer_map);
    for (std::map<int, int>::iterator it = decode_flex_sim_GBCore_Store_Large_update_flex_sim_gb_core_large_buffer_map.begin(); it != decode_flex_sim_GBCore_Store_Large_update_flex_sim_gb_core_large_buffer_map.end(); it++) {
      flex_sim_gb_core_large_buffer[it->first] = it->second;
    }
  }
  if (decode_flex_sim_GBCore_Store_Small()) {
    decode_flex_sim_GBCore_Store_Small_update_flex_sim_gb_core_small_buffer(decode_flex_sim_GBCore_Store_Small_update_flex_sim_gb_core_small_buffer_map);
    for (std::map<int, int>::iterator it = decode_flex_sim_GBCore_Store_Small_update_flex_sim_gb_core_small_buffer_map.begin(); it != decode_flex_sim_GBCore_Store_Small_update_flex_sim_gb_core_small_buffer_map.end(); it++) {
      flex_sim_gb_core_small_buffer[it->first] = it->second;
    }
  }
  if (decode_flex_sim_Start_GBLayer_Reduce()) {
    sc_biguint<1> flex_sim_gb_layer_reduce_byte_level_flag_next = decode_flex_sim_Start_GBLayer_Reduce_update_flex_sim_gb_layer_reduce_byte_level_flag();
    sc_biguint<1> flex_sim_gb_layer_reduce_group_level_flag_next = decode_flex_sim_Start_GBLayer_Reduce_update_flex_sim_gb_layer_reduce_group_level_flag();
    sc_biguint<16> flex_sim_gb_layer_reduce_grouping_index_next = decode_flex_sim_Start_GBLayer_Reduce_update_flex_sim_gb_layer_reduce_grouping_index();
    sc_biguint<16> flex_sim_gb_layer_reduce_grouping_num_next = decode_flex_sim_Start_GBLayer_Reduce_update_flex_sim_gb_layer_reduce_grouping_num();
    sc_biguint<16> flex_sim_gb_layer_reduce_grouping_rem_next = decode_flex_sim_Start_GBLayer_Reduce_update_flex_sim_gb_layer_reduce_grouping_rem();
    sc_biguint<20> flex_sim_gb_layer_reduce_memory_block_size_next = decode_flex_sim_Start_GBLayer_Reduce_update_flex_sim_gb_layer_reduce_memory_block_size();
    sc_biguint<20> flex_sim_gb_layer_reduce_memory_min_addr_offset_next = decode_flex_sim_Start_GBLayer_Reduce_update_flex_sim_gb_layer_reduce_memory_min_addr_offset();
    sc_biguint<1> flex_sim_gb_layer_reduce_start_flag_next = decode_flex_sim_Start_GBLayer_Reduce_update_flex_sim_gb_layer_reduce_start_flag();
    sc_biguint<1> flex_sim_gb_layer_reduce_timestep_level_flag_next = decode_flex_sim_Start_GBLayer_Reduce_update_flex_sim_gb_layer_reduce_timestep_level_flag();
    sc_biguint<1> flex_sim_gb_layer_reduce_vector_level_flag_next = decode_flex_sim_Start_GBLayer_Reduce_update_flex_sim_gb_layer_reduce_vector_level_flag();
    flex_sim_gb_layer_reduce_byte_level_flag = flex_sim_gb_layer_reduce_byte_level_flag_next;
    flex_sim_gb_layer_reduce_group_level_flag = flex_sim_gb_layer_reduce_group_level_flag_next;
    flex_sim_gb_layer_reduce_grouping_index = flex_sim_gb_layer_reduce_grouping_index_next;
    flex_sim_gb_layer_reduce_grouping_num = flex_sim_gb_layer_reduce_grouping_num_next;
    flex_sim_gb_layer_reduce_grouping_rem = flex_sim_gb_layer_reduce_grouping_rem_next;
    flex_sim_gb_layer_reduce_memory_block_size = flex_sim_gb_layer_reduce_memory_block_size_next;
    flex_sim_gb_layer_reduce_memory_min_addr_offset = flex_sim_gb_layer_reduce_memory_min_addr_offset_next;
    flex_sim_gb_layer_reduce_start_flag = flex_sim_gb_layer_reduce_start_flag_next;
    flex_sim_gb_layer_reduce_timestep_level_flag = flex_sim_gb_layer_reduce_timestep_level_flag_next;
    flex_sim_gb_layer_reduce_vector_level_flag = flex_sim_gb_layer_reduce_vector_level_flag_next;
  }
  init_GBLayerReduce_Group_Level();
  init_GBLayerReduce_Done_Flag();
  init_GBLayerReduce_Timestep_Level();
  init_GBLayerReduce_Vector_Level();
  init_GBLayerReduce_Byte_Level();
  while (1) {
    int schedule_counter = 0;
    if (decode_GBLayerReduce_Group_Level_gb_layer_reduce_group_level_op()) {
      sc_biguint<20> GBLayerReduce_Group_Level_gb_layer_reduce_group_level_base_addr_next = decode_GBLayerReduce_Group_Level_gb_layer_reduce_group_level_op_update_GBLayerReduce_Group_Level_gb_layer_reduce_group_level_base_addr();
      sc_biguint<1> flex_sim_gb_layer_reduce_group_level_flag_next = decode_GBLayerReduce_Group_Level_gb_layer_reduce_group_level_op_update_flex_sim_gb_layer_reduce_group_level_flag();
      sc_biguint<20> GBLayerReduce_Group_Level_gb_layer_reduce_group_level_out_addr_next = decode_GBLayerReduce_Group_Level_gb_layer_reduce_group_level_op_update_GBLayerReduce_Group_Level_gb_layer_reduce_group_level_out_addr();
      sc_biguint<5> GBLayerReduce_Group_Level_gb_layer_reduce_group_level_ts_num_next = decode_GBLayerReduce_Group_Level_gb_layer_reduce_group_level_op_update_GBLayerReduce_Group_Level_gb_layer_reduce_group_level_ts_num();
      sc_biguint<16> flex_sim_gb_layer_reduce_grouping_index_next = decode_GBLayerReduce_Group_Level_gb_layer_reduce_group_level_op_update_flex_sim_gb_layer_reduce_grouping_index();
      sc_biguint<5> GBLayerReduce_Group_Level_gb_layer_reduce_timestep_level_cntr_next = decode_GBLayerReduce_Group_Level_gb_layer_reduce_group_level_op_update_GBLayerReduce_Group_Level_gb_layer_reduce_timestep_level_cntr();
      sc_biguint<1> flex_sim_gb_layer_reduce_timestep_level_flag_next = decode_GBLayerReduce_Group_Level_gb_layer_reduce_group_level_op_update_flex_sim_gb_layer_reduce_timestep_level_flag();
      GBLayerReduce_Group_Level_gb_layer_reduce_group_level_base_addr = GBLayerReduce_Group_Level_gb_layer_reduce_group_level_base_addr_next;
      flex_sim_gb_layer_reduce_group_level_flag = flex_sim_gb_layer_reduce_group_level_flag_next;
      GBLayerReduce_Group_Level_gb_layer_reduce_group_level_out_addr = GBLayerReduce_Group_Level_gb_layer_reduce_group_level_out_addr_next;
      GBLayerReduce_Group_Level_gb_layer_reduce_group_level_ts_num = GBLayerReduce_Group_Level_gb_layer_reduce_group_level_ts_num_next;
      flex_sim_gb_layer_reduce_grouping_index = flex_sim_gb_layer_reduce_grouping_index_next;
      GBLayerReduce_Group_Level_gb_layer_reduce_timestep_level_cntr = GBLayerReduce_Group_Level_gb_layer_reduce_timestep_level_cntr_next;
      flex_sim_gb_layer_reduce_timestep_level_flag = flex_sim_gb_layer_reduce_timestep_level_flag_next;
      schedule_counter++;
      std::cout << hex << "group level on, group result addr: " << GBLayerReduce_Group_Level_gb_layer_reduce_group_level_out_addr << '\t';
      std::cout << "ts_num: " << GBLayerReduce_Group_Level_gb_layer_reduce_group_level_ts_num << std::endl;
    }
    if (decode_GBLayerReduce_Done_Flag_gb_layer_reduce_done()) {
      sc_biguint<1> flex_sim_gb_layer_reduce_start_flag_next = decode_GBLayerReduce_Done_Flag_gb_layer_reduce_done_update_flex_sim_gb_layer_reduce_start_flag();
      flex_sim_gb_layer_reduce_start_flag = flex_sim_gb_layer_reduce_start_flag_next;
      schedule_counter++;
    }
    if (decode_GBLayerReduce_Timestep_Level_gb_layer_timestep_level_op()) {
      sc_biguint<20> GBLayerReduce_Timestep_Level_gb_layer_reduce_timestep_level_base_addr_0_next = decode_GBLayerReduce_Timestep_Level_gb_layer_timestep_level_op_update_GBLayerReduce_Timestep_Level_gb_layer_reduce_timestep_level_base_addr_0();
      sc_biguint<20> GBLayerReduce_Timestep_Level_gb_layer_reduce_timestep_level_base_addr_1_next = decode_GBLayerReduce_Timestep_Level_gb_layer_timestep_level_op_update_GBLayerReduce_Timestep_Level_gb_layer_reduce_timestep_level_base_addr_1();
      sc_biguint<20> GBLayerReduce_Timestep_Level_gb_layer_reduce_timestep_level_base_addr_result_next = decode_GBLayerReduce_Timestep_Level_gb_layer_timestep_level_op_update_GBLayerReduce_Timestep_Level_gb_layer_reduce_timestep_level_base_addr_result();
      sc_biguint<5> GBLayerReduce_Group_Level_gb_layer_reduce_timestep_level_cntr_next = decode_GBLayerReduce_Timestep_Level_gb_layer_timestep_level_op_update_GBLayerReduce_Group_Level_gb_layer_reduce_timestep_level_cntr();
      sc_biguint<1> flex_sim_gb_layer_reduce_timestep_level_flag_next = decode_GBLayerReduce_Timestep_Level_gb_layer_timestep_level_op_update_flex_sim_gb_layer_reduce_timestep_level_flag();
      sc_biguint<1> flex_sim_gb_layer_reduce_vector_level_flag_next = decode_GBLayerReduce_Timestep_Level_gb_layer_timestep_level_op_update_flex_sim_gb_layer_reduce_vector_level_flag();
      sc_biguint<16> GBLayerReduce_Timestep_Level_gb_layer_reduce_vector_level_op_cntr_next = decode_GBLayerReduce_Timestep_Level_gb_layer_timestep_level_op_update_GBLayerReduce_Timestep_Level_gb_layer_reduce_vector_level_op_cntr();
      GBLayerReduce_Timestep_Level_gb_layer_reduce_timestep_level_base_addr_0 = GBLayerReduce_Timestep_Level_gb_layer_reduce_timestep_level_base_addr_0_next;
      GBLayerReduce_Timestep_Level_gb_layer_reduce_timestep_level_base_addr_1 = GBLayerReduce_Timestep_Level_gb_layer_reduce_timestep_level_base_addr_1_next;
      GBLayerReduce_Timestep_Level_gb_layer_reduce_timestep_level_base_addr_result = GBLayerReduce_Timestep_Level_gb_layer_reduce_timestep_level_base_addr_result_next;
      GBLayerReduce_Group_Level_gb_layer_reduce_timestep_level_cntr = GBLayerReduce_Group_Level_gb_layer_reduce_timestep_level_cntr_next;
      flex_sim_gb_layer_reduce_timestep_level_flag = flex_sim_gb_layer_reduce_timestep_level_flag_next;
      flex_sim_gb_layer_reduce_vector_level_flag = flex_sim_gb_layer_reduce_vector_level_flag_next;
      GBLayerReduce_Timestep_Level_gb_layer_reduce_vector_level_op_cntr = GBLayerReduce_Timestep_Level_gb_layer_reduce_vector_level_op_cntr_next;
      schedule_counter++;
      std::cout << hex << "timestep level on, timestep result addr: " << GBLayerReduce_Timestep_Level_gb_layer_reduce_timestep_level_base_addr_result << '\t';
      std::cout << "ts_cntr is : " << GBLayerReduce_Group_Level_gb_layer_reduce_timestep_level_cntr << std::endl;
    }
    if (decode_GBLayerReduce_Vector_Level_gb_layer_vector_level_op()) {
      sc_biguint<5> GBLayerReduce_Vector_Level_gb_layer_reduce_byte_level_cntr_next = decode_GBLayerReduce_Vector_Level_gb_layer_vector_level_op_update_GBLayerReduce_Vector_Level_gb_layer_reduce_byte_level_cntr();
      sc_biguint<1> flex_sim_gb_layer_reduce_byte_level_flag_next = decode_GBLayerReduce_Vector_Level_gb_layer_vector_level_op_update_flex_sim_gb_layer_reduce_byte_level_flag();
      sc_biguint<20> GBLayerReduce_Vector_Level_gb_layer_reduce_vector_level_addr_0_next = decode_GBLayerReduce_Vector_Level_gb_layer_vector_level_op_update_GBLayerReduce_Vector_Level_gb_layer_reduce_vector_level_addr_0();
      sc_biguint<20> GBLayerReduce_Vector_Level_gb_layer_reduce_vector_level_addr_0_result_next = decode_GBLayerReduce_Vector_Level_gb_layer_vector_level_op_update_GBLayerReduce_Vector_Level_gb_layer_reduce_vector_level_addr_0_result();
      sc_biguint<20> GBLayerReduce_Vector_Level_gb_layer_reduce_vector_level_addr_1_next = decode_GBLayerReduce_Vector_Level_gb_layer_vector_level_op_update_GBLayerReduce_Vector_Level_gb_layer_reduce_vector_level_addr_1();
      sc_biguint<1> flex_sim_gb_layer_reduce_vector_level_flag_next = decode_GBLayerReduce_Vector_Level_gb_layer_vector_level_op_update_flex_sim_gb_layer_reduce_vector_level_flag();
      sc_biguint<16> GBLayerReduce_Timestep_Level_gb_layer_reduce_vector_level_op_cntr_next = decode_GBLayerReduce_Vector_Level_gb_layer_vector_level_op_update_GBLayerReduce_Timestep_Level_gb_layer_reduce_vector_level_op_cntr();
      GBLayerReduce_Vector_Level_gb_layer_reduce_byte_level_cntr = GBLayerReduce_Vector_Level_gb_layer_reduce_byte_level_cntr_next;
      flex_sim_gb_layer_reduce_byte_level_flag = flex_sim_gb_layer_reduce_byte_level_flag_next;
      GBLayerReduce_Vector_Level_gb_layer_reduce_vector_level_addr_0 = GBLayerReduce_Vector_Level_gb_layer_reduce_vector_level_addr_0_next;
      GBLayerReduce_Vector_Level_gb_layer_reduce_vector_level_addr_0_result = GBLayerReduce_Vector_Level_gb_layer_reduce_vector_level_addr_0_result_next;
      GBLayerReduce_Vector_Level_gb_layer_reduce_vector_level_addr_1 = GBLayerReduce_Vector_Level_gb_layer_reduce_vector_level_addr_1_next;
      flex_sim_gb_layer_reduce_vector_level_flag = flex_sim_gb_layer_reduce_vector_level_flag_next;
      GBLayerReduce_Timestep_Level_gb_layer_reduce_vector_level_op_cntr = GBLayerReduce_Timestep_Level_gb_layer_reduce_vector_level_op_cntr_next;
      schedule_counter++;
      std::cout << hex << "vector level on, vector level result addr: " << GBLayerReduce_Vector_Level_gb_layer_reduce_vector_level_addr_0_result << std::endl;
    }
    if (decode_GBLayerReduce_Byte_Level_gb_layer_byte_level_op()) {
      decode_GBLayerReduce_Byte_Level_gb_layer_byte_level_op_update_flex_sim_gb_core_large_buffer(decode_GBLayerReduce_Byte_Level_gb_layer_byte_level_op_update_flex_sim_gb_core_large_buffer_map);
      sc_biguint<5> GBLayerReduce_Vector_Level_gb_layer_reduce_byte_level_cntr_next = decode_GBLayerReduce_Byte_Level_gb_layer_byte_level_op_update_GBLayerReduce_Vector_Level_gb_layer_reduce_byte_level_cntr();
      sc_biguint<1> flex_sim_gb_layer_reduce_byte_level_flag_next = decode_GBLayerReduce_Byte_Level_gb_layer_byte_level_op_update_flex_sim_gb_layer_reduce_byte_level_flag();
      for (std::map<int, int>::iterator it = decode_GBLayerReduce_Byte_Level_gb_layer_byte_level_op_update_flex_sim_gb_core_large_buffer_map.begin(); it != decode_GBLayerReduce_Byte_Level_gb_layer_byte_level_op_update_flex_sim_gb_core_large_buffer_map.end(); it++) {
        flex_sim_gb_core_large_buffer[it->first] = it->second;
      }
      GBLayerReduce_Vector_Level_gb_layer_reduce_byte_level_cntr = GBLayerReduce_Vector_Level_gb_layer_reduce_byte_level_cntr_next;
      flex_sim_gb_layer_reduce_byte_level_flag = flex_sim_gb_layer_reduce_byte_level_flag_next;
      schedule_counter++;
    }
    if (schedule_counter == 0) 
      break;
  }
};
