############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
############################################################
open_project rx
set_top rx_update
add_files rx/src/Sync.cpp
add_files rx/src/Sync.h
add_files rx/src/config.h
add_files rx/src/ofdm.cpp
add_files rx/src/ofdm.h
add_files rx/src/rom.cpp
add_files rx/src/rx.cpp
add_files rx/src/rx.h
add_files -tb rx/src/tb.h
add_files -tb rx/src/tb.cpp -cflags "-std=c++0x"
add_files -tb rx/src/sync_update_tb.cpp -cflags "-std=c++0x"
add_files -tb rx/src/sync_find_peak_tb.cpp -cflags "-std=c++0x"
add_files -tb rx/src/sync_correlate_tb.cpp -cflags "-std=c++0x"
add_files -tb rx/src/ofdm_ifo_tb.cpp
add_files -tb rx/src/ofdm_ifft_tb.cpp
open_solution "solution1"
set_part {xa7a35tcsg325-2i}
create_clock -period 10 -name default
source "./rx/solution1/directives.tcl"
csim_design -O -compiler gcc
csynth_design
cosim_design
export_design -format ip_catalog
