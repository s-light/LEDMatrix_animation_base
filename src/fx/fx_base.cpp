// NOLINT(legal/copyright)

/******************************************************************************

    __doc__ = """
    base class for effects
    Enjoy the colors :-)
    """

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

// include own headerfile
// NOLINTNEXTLINE(build/include)
#include "./fx_base.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// functions

// FXBase::FXBase(uint32_t duration_ = 5 * 1000) {
FXBase::FXBase() {
    // duration = duration_;
}

FXBase::~FXBase() {
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// brightness
//
// float FXBase::set_brightness(float brightness_) {
//     brightness = clamp(brightness_, static_cast<float>(0.0), brightness_max);
//     // if (brightness_ <= 0) {
//     //     brightness = 0;
//     // }
//     // if (brightness_ > brightness_max) {
//     //     brightness = brightness_max;
//     // } else {
//     //     brightness = brightness_;
//     // }
//     return brightness;
// }
//
// float FXBase::set_hue(float hue_) {
//     hue = clamp(hue_, static_cast<float>(0.0), static_cast<float>(1.0));
//     return hue;
// }
//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// control

void FXBase::run(bool run_ = true) {
    running = run_;
    // set to infinite loop:
    loopcount = 0;
}

void FXBase::start_singleshot() {
    start_loop_n_times(1);
}

void FXBase::start_loop_n_times(uint16_t count) {
    // run = false;
    reset();
    loopcount = count;
    running = true;
}

uint16_t FXBase::get_loopcount() {
    return loopcount;
}


float FXBase::set_position(float position_) {
    position = clamp01(position_);
    return position;
}

float FXBase::get_position() {
    return position;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// animation

void FXBase::update_position() {
    if (running) {
        position = normalize_to_01(micros(), start, end);

        // loopcount++;
        if (position >  1.0) {
            reset();

            // loopcount == 0 == infinite
            if (loopcount != 0) {
                // loopcount is active
                if (loopcount == 1) {
                    running = false;
                }
                loopcount -= 1;
                // Serial.print("----- fx_base loopcount: ");
                // Serial.print(loopcount);
                // Serial.println();
            }
        }
        // Serial.print("p:");
        // Serial.println(position);
    }
}


void FXBase::reset() {
    position = 0.0;
    // uint32_t duration_us = micros() - start;
    // float duration_seconds =  duration_us / (1000.0 * 1000);
    // float fps = fps_loopcount / duration_seconds;
    // fps_loopcount = 0;
    start = micros();
    end = micros() + (duration * 1000);

    // if (run) {
    //     Serial.print("----- position loop restart (");
    //     Serial.print(fps);
    //     Serial.print("FPS)");
    //     // Serial.print("   !!! millis() timming is off - ");
    //     // Serial.print("so this calculation is currently wrong !!!");
    //     Serial.println();
    // }
}


CHSV FXBase::get_pixel(__attribute__((unused)) PixelPos * pixel_pos) {
    CHSV pixel_hsv = CHSV(hue, saturation, brightness);

    pixel_hsv.value *= visibility;
    return pixel_hsv;
}

// void FXBase::get_all() {
//     // float offset = map_range_01_to(position, 0.0, (PI * 30));
//     // float offset = map_range_01_to(
//     //     position,
//     //     0, MATRIX_ROW_COUNT);
//     float offset = position;
//
//     // Serial.println("");
//
//     // float offset_PI = map_range(offset, 0.0, 1.0, 0.0, (3.14/2));
//     // PI 3,141592653589793
//     // float offset_PI = offset * (3.141592 / 2);
//     // Serial.printf(
//     //     "%2.3f %2.3f "
//     //     "%2.3f %2.3f \r\n",
//     //     // offset,
//     //     // sin(offset),
//     //     offset_PI,
//     //     sin(offset_PI)
//     //     // NOLINTNEXTLINE(whitespace/parens)
//     // );
//
//     for (size_t row_i = 0; row_i < MATRIX_ROW_COUNT; row_i++) {
//         // normalize row
//         float row = map_range(
//             row_i,
//             0, MATRIX_ROW_COUNT-1,
//             -0.5, 0.5);
//         // float row = normalize_to_01(
//         //     row_i,
//         //     0, MATRIX_ROW_COUNT-1);
//         for (size_t col_i = 0; col_i < MATRIX_COL_COUNT; col_i++) {
//             // normalize col
//             // float col = map_range__int2float(
//             float col = map_range(
//                 col_i,
//                 0, MATRIX_COL_COUNT-1,
//                 -0.5, 0.5);
//             // float col = normalize_to_01(
//             //     col_i,
//             //     0, MATRIX_COL_COUNT-1);
//
//             // ------------------------------------------
//             // CHSV pixel_hsv = Matrix2D_get_pixel(
//             //     col, row,
//             //     col_i, row_i,
//             //     offset);
//             CHSV pixel_hsv = Matrix2D_get_pixel(col, row, offset);
//
//             // ------------------------------------------
//             // final conversions
//             // global brightness
//             pixel_hsv.value *= brightness;
//
//             // CHSV pixel_hsv = CHSV(0.5, 0.0, 0.10);
//             // convert to rgb
//             CRGB pixel_rgb = hsv2rgb(pixel_hsv);
//             // gamma & global brightness
//             // fancyled.gamma_adjust(brightness=self.brightness);
//             matrix.tlc.setRGB(
//                 matrix.pmap[col_i][row_i],
//                 // convert float to uint16_t
//                 pixel_rgb.r * 65535,
//                 pixel_rgb.g * 65535,
//                 pixel_rgb.b * 65535);
//             // matrix.tlc.setRGB(
//             //     matrix.pmap[col_i][row_i],
//             //     10000,
//             //     10000,
//             //     0);
//         }
//     }
// }






// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// THE END
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
