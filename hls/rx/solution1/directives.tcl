############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
############################################################
set_directive_pipeline "Sync::correlate"
set_directive_stream -depth 1 -dim 1 "Sync::update" c_out
set_directive_pipeline "Sync::align"
set_directive_pipeline "Sync::findPeak"
set_directive_pipeline "Sync::update"
set_directive_pipeline "sync_correlate"
set_directive_pipeline "sync_align"
set_directive_pipeline "sync_findPeak"
set_directive_pipeline "sync_update"
set_directive_stream -depth 1 -dim 1 "sync_update" c_out
set_directive_pipeline "sync_find_peak"
