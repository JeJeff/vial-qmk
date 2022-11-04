/*
Copyright 2020 Ben Roesner (keycapsss.com)
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "lily58.h"

#include "quantum.h"




#ifdef RGB_MATRIX_ENABLE

  // Logical Layout
  // Columns
  // Left
  // 0  1  2  3  4  5
  //                   ROWS
  // 12 11 10 09 08 07   0
  //    03    02    01
  // 13 14 15 16 17 18   1
  // 24 23 22 21 20 19   2
  //    04    05    06
  // 25 26 27 28 29 30   3
  //
  //    35 34 33 32 31   4
  //
  // Right
  // 0  1  2  3  4  5
  //                    ROWS
  // 12 11 10 09 08 07   5
  //    03    02    01
  // 13 14 15 16 17 18   6
  // 24 23 22 21 20 19   7
  //    04    05    06
  // 25 26 27 28 29 30   8
  //
  //    35 34 33 32 31   9
  //
  // Physical Layout
  // Columns
  // 0  1  2  3  4  5  6  7  8  9  10 11 12 13
  //                                           ROWS
  // 12 11 10 09 08 07       07 08 09 10 11 12  0
  //    03    02    01       01    02    03
  // 13 14 15 16 17 18       18 17 16 15 14 13  1
  // 24 23 22 21 20 19       19 20 21 22 23 24  2
  //    04             31 31             04
  // 25 26 27 28 29 30       30 29 28 27 26 25  3
  //          05    06       06    05
  //         35 34 33 32   32 33 34 35          4#
  //
  // LED indexes
  // 0  1  2  3  4  5  6  7  8  9  10 11 12 13
  //                                           ROWS
  // 12 11 10 09 08 07       42 43 44 45 46 47  0
  //    03    02    01       36    37    38
  // 13 14 15 16 17 18       53 52 51 50 49 48  1
  // 24 23 22 21 20 19       54 55 56 57 58 59  2
  //    04             31 66             39
  // 25 26 27 28 29 30       65 64 63 62 61 60  3
  //          05    06       41    40
  //         35 34 33 32   67 68 69 70          4

led_config_t g_led_config = { {
  // Key Matrix to LED Index - 0 indexed
    {     11,  10,   9,   8,   7,   6 },
    {     12,  13,  14,  15,  16,  17 },
    {     23,  22,  21,  20,  19,  18 },
    {     24,  25,  26,  27,  28,  29 },
    { NO_LED,  34,  33,  32,  31,  30 },
    {     46,  45,  44,  43,  42,  41 },
    {     47,  48,  49,  50,  51,  52 },
    {     58,  57,  56,  55,  54,  53 },
    {     59,  60,  61,  62,  63,  64 },
    { NO_LED,  69,  68,  67,  66,  65 },
}, {
    
  // LED Index to Physical Position - range { 0..224, 0..64 }
  // Done col 6
  //            0   1   2   3   4   5   6    7    8    9    10   11   12   13
  // colspacing 0, 17, 34, 52, 69, 86, 103, 121, 138, 155, 172, 190, 207, 224
  // rowspacing 0, 16, 32, 48, 64
  //     1             2             3             4             5             6             7             8             9             10
    {  86,   0 }, {  52,   0 }, {  17,   0 }, {  17,  32 }, {  52,  48 }, {  86,  48 }, {  86,   0 }, {  69,   0 }, {  52,   0 }, {  34,   0 }, //  1 - 10
    {  17,   0 }, {   0,   0 }, {   0,  16 }, {  17,  16 }, {  34,  16 }, {  52,  16 }, {  69,  16 }, {  86,  16 }, {  86,  32 }, {  69,  32 }, // 11 - 20 
    {  52,  32 }, {  34,  32 }, {  17,  32 }, {   0,  32 }, {   0,  48 }, {  17,  48 }, {  34,  48 }, {  52,  48 }, {  69,  48 }, {  86,  48 }, // 21 - 30
    { 103,  32 }, { 115,  64 }, {  78,  64 }, {  61,  64 }, {  43,  64 }, { 138,   0 }, { 172,   0 }, { 207,   0 }, { 207,  32 }, { 172,  48 }, // 31 - 40
    { 138,  48 }, { 138,   0 }, { 155,   0 }, { 172,   0 }, { 190,   0 }, { 207,   0 }, { 224,   0 }, { 224,  16 }, { 207,  16 }, { 190,  16 }, // 41 - 50
    { 172,  16 }, { 155,  16 }, { 138,  16 }, { 138,  32 }, { 155,  32 }, { 172,  32 }, { 190,  32 }, { 207,  32 }, { 224,  32 }, { 224,  48 }, // 51 - 60
    { 207,  48 }, { 190,  48 }, { 172,  48 }, { 155,  48 }, { 138,  48 }, { 121,  32 }, { 129,  64 }, { 146,  64 }, { 163,  64 }, { 181,  64 }, // 61 - 70
}, {

  // LED Index to Flag
  // 1 = modifier
  // 2 = underglow
  // 4 = key backlight
  // 8 = indicator
    2, 2, 2, 2, 2, 2, 4, 4, 4, 4,  //  1 - 10
    4, 1, 1, 4, 4, 4, 4, 4, 4, 4,  // 11 - 20
    4, 4, 4, 1, 1, 4, 4, 4, 4, 4,  // 21 - 30
    1, 1, 1, 1, 1, 2, 2, 2, 2, 2,  // 31 - 40
    2, 4, 4, 4, 4, 4, 1, 1, 4, 4,  // 41 - 50
    4, 4, 4, 4, 4, 4, 4, 4, 4, 2,  // 51 - 60
    4, 4, 4, 4, 4, 1, 1, 1, 1, 1,  // 61 - 70
} };

void suspend_power_down_kb(void) {
    rgb_matrix_set_suspend_state(true);
    suspend_power_down_user();
}

void suspend_wakeup_init_kb(void) {
    rgb_matrix_set_suspend_state(false);
    suspend_wakeup_init_user();
}
#endif