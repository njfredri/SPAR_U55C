//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2022.2 (win64) Build 3671981 Fri Oct 14 05:00:03 MDT 2022
//Date        : Wed Aug 23 14:06:37 2023
//Host        : Nathan_Macbook running 64-bit major release  (build 9200)
//Command     : generate_target config_mpsoc_wrapper.bd
//Design      : config_mpsoc_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module config_mpsoc_wrapper
   (fpga_uart0_rxd,
    fpga_uart0_txd,
    refclk_clk_n,
    refclk_clk_p,
    reset_rtl);
  input fpga_uart0_rxd;
  output fpga_uart0_txd;
  input refclk_clk_n;
  input refclk_clk_p;
  input reset_rtl;

  wire fpga_uart0_rxd;
  wire fpga_uart0_txd;
  wire refclk_clk_n;
  wire refclk_clk_p;
  wire reset_rtl;

  config_mpsoc config_mpsoc_i
       (.fpga_uart0_rxd(fpga_uart0_rxd),
        .fpga_uart0_txd(fpga_uart0_txd),
        .refclk_clk_n(refclk_clk_n),
        .refclk_clk_p(refclk_clk_p),
        .reset_rtl(reset_rtl));
endmodule
