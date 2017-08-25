############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
############################################################
open_project rx
set_top Sync::correlate
add_files rx/src/Sync.cpp
add_files rx/src/Sync.h
add_files rx/src/config.h
add_files rx/src/main.cpp
open_solution "solution1"
set_part {xa7a35tcsg325-2i}
create_clock -period 10 -name default
source "./rx/solution1/directives.tcl"
#csim_design
csynth_design
#cosim_design
export_design -format ip_catalog
