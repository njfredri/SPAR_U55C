# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  ipgui::add_param $IPINST -name "ARRAY_DIM"
  ipgui::add_param $IPINST -name "MAX_WORD_LENGTH"
  ipgui::add_param $IPINST -name "TILE_DIM"
  ipgui::add_param $IPINST -name "C_S00_AXI_HIGHADDR"
  ipgui::add_param $IPINST -name "C_S00_AXI_BASEADDR"

}

proc update_PARAM_VALUE.ARRAY_DIM { PARAM_VALUE.ARRAY_DIM } {
	# Procedure called to update ARRAY_DIM when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.ARRAY_DIM { PARAM_VALUE.ARRAY_DIM } {
	# Procedure called to validate ARRAY_DIM
	return true
}

proc update_PARAM_VALUE.MAX_WORD_LENGTH { PARAM_VALUE.MAX_WORD_LENGTH } {
	# Procedure called to update MAX_WORD_LENGTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.MAX_WORD_LENGTH { PARAM_VALUE.MAX_WORD_LENGTH } {
	# Procedure called to validate MAX_WORD_LENGTH
	return true
}

proc update_PARAM_VALUE.TILE_DIM { PARAM_VALUE.TILE_DIM } {
	# Procedure called to update TILE_DIM when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.TILE_DIM { PARAM_VALUE.TILE_DIM } {
	# Procedure called to validate TILE_DIM
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_DATA_WIDTH { PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to update C_S00_AXI_DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_DATA_WIDTH { PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to validate C_S00_AXI_DATA_WIDTH
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_ADDR_WIDTH { PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to update C_S00_AXI_ADDR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_ADDR_WIDTH { PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to validate C_S00_AXI_ADDR_WIDTH
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_BASEADDR { PARAM_VALUE.C_S00_AXI_BASEADDR } {
	# Procedure called to update C_S00_AXI_BASEADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_BASEADDR { PARAM_VALUE.C_S00_AXI_BASEADDR } {
	# Procedure called to validate C_S00_AXI_BASEADDR
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_HIGHADDR { PARAM_VALUE.C_S00_AXI_HIGHADDR } {
	# Procedure called to update C_S00_AXI_HIGHADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_HIGHADDR { PARAM_VALUE.C_S00_AXI_HIGHADDR } {
	# Procedure called to validate C_S00_AXI_HIGHADDR
	return true
}


proc update_MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH { MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S00_AXI_DATA_WIDTH}] ${MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH { MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S00_AXI_ADDR_WIDTH}] ${MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH}
}

proc update_MODELPARAM_VALUE.ARRAY_DIM { MODELPARAM_VALUE.ARRAY_DIM PARAM_VALUE.ARRAY_DIM } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.ARRAY_DIM}] ${MODELPARAM_VALUE.ARRAY_DIM}
}

proc update_MODELPARAM_VALUE.TILE_DIM { MODELPARAM_VALUE.TILE_DIM PARAM_VALUE.TILE_DIM } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.TILE_DIM}] ${MODELPARAM_VALUE.TILE_DIM}
}

proc update_MODELPARAM_VALUE.MAX_WORD_LENGTH { MODELPARAM_VALUE.MAX_WORD_LENGTH PARAM_VALUE.MAX_WORD_LENGTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.MAX_WORD_LENGTH}] ${MODELPARAM_VALUE.MAX_WORD_LENGTH}
}

