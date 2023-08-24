// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2022.2 (win64) Build 3671981 Fri Oct 14 05:00:03 MDT 2022
// Date        : Wed Aug 23 13:23:29 2023
// Host        : Nathan_Macbook running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub -rename_top config_mpsoc_clk_wiz_1_1 -prefix
//               config_mpsoc_clk_wiz_1_1_ config_mpsoc_clk_wiz_1_1_stub.v
// Design      : config_mpsoc_clk_wiz_1_1
// Purpose     : Stub declaration of top-level module interface
// Device      : xcu55c-fsvh2892-2L-e
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
module config_mpsoc_clk_wiz_1_1(clk_out1, reset, locked, clk_in1_p, clk_in1_n)
/* synthesis syn_black_box black_box_pad_pin="clk_out1,reset,locked,clk_in1_p,clk_in1_n" */;
  output clk_out1;
  input reset;
  output locked;
  input clk_in1_p;
  input clk_in1_n;
endmodule