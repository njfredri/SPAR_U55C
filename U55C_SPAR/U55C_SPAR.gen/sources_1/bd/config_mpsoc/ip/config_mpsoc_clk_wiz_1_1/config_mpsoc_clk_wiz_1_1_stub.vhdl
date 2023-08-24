-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2022.2 (win64) Build 3671981 Fri Oct 14 05:00:03 MDT 2022
-- Date        : Wed Aug 23 13:23:29 2023
-- Host        : Nathan_Macbook running 64-bit major release  (build 9200)
-- Command     : write_vhdl -force -mode synth_stub -rename_top config_mpsoc_clk_wiz_1_1 -prefix
--               config_mpsoc_clk_wiz_1_1_ config_mpsoc_clk_wiz_1_1_stub.vhdl
-- Design      : config_mpsoc_clk_wiz_1_1
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xcu55c-fsvh2892-2L-e
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity config_mpsoc_clk_wiz_1_1 is
  Port ( 
    clk_out1 : out STD_LOGIC;
    reset : in STD_LOGIC;
    locked : out STD_LOGIC;
    clk_in1_p : in STD_LOGIC;
    clk_in1_n : in STD_LOGIC
  );

end config_mpsoc_clk_wiz_1_1;

architecture stub of config_mpsoc_clk_wiz_1_1 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "clk_out1,reset,locked,clk_in1_p,clk_in1_n";
begin
end;
