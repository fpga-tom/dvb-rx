############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
############################################################
open_project rx
set_top sync_update
add_files rx/src/Sync.cpp
add_files rx/src/Sync.h
add_files rx/src/config.h
add_files -tb rx/src/sync_align_tb.cpp
add_files -tb rx/src/sync_correlate_tb.cpp -cflags "-std=c++0x"
add_files -tb rx/src/sync_find_peak_tb.cpp -cflags "-std=c++0x"
add_files -tb rx/src/sync_update_tb.cpp
add_files -tb rx/src/tb.cpp
open_solution "solution1"
set_part {xa7a35tcsg325-2i}
create_clock -period 10 -name default
source "./rx/solution1/directives.tcl"
csim_design -O -compiler gcc
csynth_design
cosim_design
export_design -format ip_catalog
