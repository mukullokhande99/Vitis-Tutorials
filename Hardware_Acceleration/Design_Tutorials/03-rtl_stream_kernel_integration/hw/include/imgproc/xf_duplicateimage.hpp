/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#ifndef _XF_Duplicate_HPP_
#define _XF_Duplicate_HPP_

#ifndef __cplusplus
#error C++ is needed to include this header
#endif

#include "hls_stream.h"
#include "common/xf_common.hpp"
#include "common/xf_utility.hpp"

namespace xf {
namespace cv {
template <int ROWS, int COLS, int SRC_T, int DEPTH, int NPC, int WORDWIDTH, int XFPDEPTH>
void xFDuplicate(xf::cv::Mat<SRC_T, ROWS, COLS, NPC>& _src,
                 xf::cv::Mat<SRC_T, ROWS, COLS, NPC>& _dst1,
                 xf::cv::Mat<SRC_T, ROWS, COLS, NPC, XFPDEPTH>& _dst2,
                 uint16_t img_height,
                 uint16_t img_width) {
    img_width = img_width >> XF_BITSHIFT(NPC);

    ap_uint<13> row, col;
    int readindex = 0, writeindex1 = 0, writeindex2 = 0;
Row_Loop:
    for (row = 0; row < img_height; row++) {
// clang-format off
#pragma HLS LOOP_TRIPCOUNT min=ROWS max=ROWS
#pragma HLS LOOP_FLATTEN off
    // clang-format on
    Col_Loop:
        for (col = 0; col < img_width; col++) {
// clang-format off
#pragma HLS LOOP_TRIPCOUNT min = 240 max = 240
#pragma HLS pipeline
            // clang-format on
            XF_TNAME(SRC_T, NPC) tmp_src;
            tmp_src = _src.read(readindex++);
            _dst1.write(writeindex1++, tmp_src);
            _dst2.write(writeindex2++, tmp_src);
        }
    }
}

template <int SRC_T, int ROWS, int COLS, int NPC, int XFPDEPTH>
void duplicateMat(xf::cv::Mat<SRC_T, ROWS, COLS, NPC>& _src,
                  xf::cv::Mat<SRC_T, ROWS, COLS, NPC>& _dst1,
                  xf::cv::Mat<SRC_T, ROWS, COLS, NPC, XFPDEPTH>& _dst2) {
// clang-format off
    #pragma HLS inline off
    // clang-format on

    xFDuplicate<ROWS, COLS, SRC_T, XF_DEPTH(SRC_T, NPC), NPC, XF_WORDWIDTH(SRC_T, NPC), XFPDEPTH>(_src, _dst1, _dst2,
                                                                                                  _src.rows, _src.cols);
}

template <int ROWS, int COLS, int SRC_T, int DEPTH, int NPC, int WORDWIDTH>
void xFDuplicate(xf::cv::Mat<SRC_T, ROWS, COLS, NPC>& _src,
                 xf::cv::Mat<SRC_T, ROWS, COLS, NPC>& _dst1,
                 xf::cv::Mat<SRC_T, ROWS, COLS, NPC>& _dst2,
                 uint16_t img_height,
                 uint16_t img_width) {
    img_width = img_width >> XF_BITSHIFT(NPC);

    ap_uint<13> row, col;
    int readindex = 0, writeindex1 = 0, writeindex2 = 0;
Row_Loop:
    for (row = 0; row < img_height; row++) {
// clang-format off
#pragma HLS LOOP_TRIPCOUNT min=ROWS max=ROWS
#pragma HLS LOOP_FLATTEN off
    // clang-format on
    Col_Loop:
        for (col = 0; col < img_width; col++) {
// clang-format off
#pragma HLS LOOP_TRIPCOUNT min = 240 max = 240
#pragma HLS pipeline
            // clang-format on
            XF_TNAME(SRC_T, NPC) tmp_src;
            tmp_src = _src.read(readindex++);
            _dst1.write(writeindex1++, tmp_src);
            _dst2.write(writeindex2++, tmp_src);
        }
    }
}

template <int SRC_T, int ROWS, int COLS, int NPC>
void duplicateMat(xf::cv::Mat<SRC_T, ROWS, COLS, NPC>& _src,
                  xf::cv::Mat<SRC_T, ROWS, COLS, NPC>& _dst1,
                  xf::cv::Mat<SRC_T, ROWS, COLS, NPC>& _dst2) {
// clang-format off
    #pragma HLS inline off
    // clang-format on

    xFDuplicate<ROWS, COLS, SRC_T, XF_DEPTH(SRC_T, NPC), NPC, XF_WORDWIDTH(SRC_T, NPC)>(_src, _dst1, _dst2, _src.rows,
                                                                                        _src.cols);
}
} // namespace cv
} // namespace xf
#endif
