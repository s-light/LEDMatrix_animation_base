/******************************************************************************

    matrix.h
        all things led matrix related

    libraries used:
        ~

    written by stefan krueger (s-light),
        github@s-light.eu, http://s-light.eu, https://github.com/s-light/

******************************************************************************/
/******************************************************************************
The MIT License (MIT)

Copyright (c) 2021 Stefan Krüger

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/



#ifndef matrix_H_
#define matrix_H_

// include Core Arduino functionality
#include <Arduino.h>


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// pixel map
// LEDBoard_4x4_16bit mapping

static const uint16_t LEDBOARD_COL_COUNT = 4;
static const uint16_t LEDBOARD_ROW_COUNT = 4;
static const uint16_t LEDBOARD_PIXEL_COUNT = (
    LEDBOARD_ROW_COUNT * LEDBOARD_COL_COUNT);
static const uint16_t LEDBOARD_CHIP_COUNT = LEDBOARD_PIXEL_COUNT / 4;

static const uint8_t LEDBOARD_SINGLE
        [4][LEDBOARD_ROW_COUNT][LEDBOARD_COL_COUNT] = {
    // with all 4 rotations
    {
        // 0 =  0° → socket at bottom
        { 0,  1,  4,  5},
        { 2,  3,  6,  7},
        { 8,  9, 12, 13},
        {10, 11, 14, 15},
    },
    {
        // 1 = 90° → socket at left
        {10,  8,  2,  0},
        {11,  9,  3,  1},
        {14, 12,  6,  4},
        {15, 13,  7,  5},
    },
    {
        // 2 = 180° → socket at top
        {15, 14, 11, 10},
        {13, 12,  9,  8},
        { 7,  6,  3,  2},
        { 5,  4,  1,  0},
    },
    {
        // 3 = 270° → socket at right
        { 5,  7, 13, 15},
        { 4,  6, 12, 14},
        { 1,  3,  9, 11},
        { 0,  2,  8, 10},
    },
};

static const uint8_t BOARDS_COL_COUNT = 5;
static const uint8_t BOARDS_ROW_COUNT = 7;
static const uint8_t BOARDS_COUNT = BOARDS_COL_COUNT * BOARDS_ROW_COUNT;

static const uint16_t CHIPS_COUNT = BOARDS_COUNT * LEDBOARD_CHIP_COUNT;

static const uint8_t BOARDS_ORDER
        [BOARDS_ROW_COUNT][BOARDS_COL_COUNT] = {
    {30, 31, 32, 33, 34},
    {25, 26, 27, 28, 29},
    {20, 21, 22, 23, 24},
    {3, 7, 11, 15, 19},
    {2, 6, 10, 14, 18},
    {1, 5, 9, 13, 17},
    {0, 4, 8, 12, 16},
};

static const uint8_t BOARDS_ROTATION
        [BOARDS_ROW_COUNT][BOARDS_COL_COUNT] = {
    {2, 2, 2, 2, 2},
    {2, 2, 2, 2, 2},
    {2, 2, 2, 2, 2},
    {3, 3, 3, 3, 3},
    {3, 3, 3, 3, 3},
    {3, 3, 3, 3, 3},
    {3, 3, 3, 3, 3},
};

static const uint8_t MATRIX_COL_COUNT = LEDBOARD_COL_COUNT * BOARDS_COL_COUNT;
static const uint8_t MATRIX_ROW_COUNT = LEDBOARD_ROW_COUNT * BOARDS_ROW_COUNT;
static const uint16_t MATRIX_PIXEL_COUNT = MATRIX_COL_COUNT * MATRIX_ROW_COUNT;


uint16_t pmap[MATRIX_COL_COUNT][MATRIX_ROW_COUNT];


uint16_t mymap_LEDBoard_4x4_16bit(
    uint8_t col, uint8_t row
) {
    // """Map row and col to pixel_index."""
    // get Board position
    uint8_t board_col = col / LEDBOARD_COL_COUNT;
    uint8_t board_row = row / LEDBOARD_ROW_COUNT;
    uint8_t board_sub_col = col % LEDBOARD_COL_COUNT;
    uint8_t board_sub_row = row % LEDBOARD_ROW_COUNT;

    uint8_t board_offset = BOARDS_ORDER[board_row][board_col];
    uint8_t board_rotation = BOARDS_ROTATION[board_row][board_col];
    uint8_t pixel_offset =
    LEDBOARD_SINGLE[board_rotation][board_sub_row][board_sub_col];

    uint8_t pixel_index = (pixel_offset * BOARDS_COUNT) + board_offset;

    return pixel_index;
}

void pmap_init() {
    // """Prepare Static Map."""
    for (size_t row_index = 0; row_index < MATRIX_ROW_COUNT; row_index++) {
        for (size_t col_index = 0; col_index < MATRIX_COL_COUNT; col_index++) {
            pmap[col_index][row_index] =
                // mymap_LEDBoard_4x4_HD(
                // mymap_LEDBoard_4x4_HD_CrystalLightGuide(
                mymap_LEDBoard_4x4_16bit(
                    col_index, row_index);
        }
    }
}


#endif  // matrix_H_
