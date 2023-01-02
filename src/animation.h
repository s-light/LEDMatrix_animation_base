 /******************************************************************************

    __doc__ = """
    animation.py - TLC5971 & FancyLED & 2D Array / Mapping.

    it combines the TLC5971 library with FancyLED and 2D Array / Mapping.

    Enjoy the colors :-)
    """

    libraries used:
        ~ slight_DebugMenu
        ~ slight_FaderLin
        ~ slight_TLC5957
            written by stefan krueger (s-light),
                github@s-light.eu, http://s-light.eu, https://github.com/s-light/
            license: MIT

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



#ifndef MyAnimation_H_
#define MyAnimation_H_

// include Core Arduino functionality
#include <Arduino.h>

#include <slight_DebugMenu.h>
#include <slight_FaderLin.h>

#include "./color.h"
#include "./mapping.h"
#include "./easing.h"
#include "./ledmatrix.h"

#include "./fx/fx_base.h"
#include "./fx/fx_rainbow.h"
#include "./fx/fx_line.h"
#include "./fx/fx_wave.h"
#include "./fx/fx_plasma.h"
#include "./fx/fx_points.h"
#include "./fx/fx_sparkle.h"

class MyAnimation {
public:

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // structs

    // typedef void (* tAnimationUpdatePixel) ();
    // typedef void (* tAnimationUpdatePosition) ();
    //
    // struct animation_t {
    //   const tAnimationUpdatePixel update_pixel;
    //   const tAnimationUpdatePosition update_position;
    //   const uint16_t mask;
    //   const uint16_t defaultv;
    // };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // attributes

    // pin_output_enable: 2
    MyLEDMatrix matrix = MyLEDMatrix(2);

    // enum fx_names {
    //     // FX_base,
    //     FX_line,
    //     // FX_rainbow,
    //     // FX_sparkle,
    //     FX_plasma,
    //     FX_wave,
    //     FX_points,
    // };
    //
    // FXBase * fx_list[] = {
    //     // new FXBase(),
    //     new FXLine(),
    //     // new FXRainbow(),
    //     // new FXSparkle(),
    //     new FXPlasma(),
    //     new FXWave(),
    //     new FXPoints(),
    // };

    // we need to do the `FXBase *` pointer thing -
    // otherwise the newly created class is *sliced*
    // and only the parts of the base class is available...
    // FXBase * fx_base = new FXBase();
    // FXBase * fx_line = new FXLine();
    // FXBase * fx_rainbow = new FXRainbow();
    // FXBase * fx_sparkle = new FXSparkle();
    // FXBase * fx_plasma = new FXPlasma();
    FXBase * fx_wave = new FXWave();
    FXBase * fx_points = new FXPoints();


    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // constructor

    MyAnimation();
    ~MyAnimation();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // public functions

    // basic library api
    void begin(Stream &out);
    void update();
    void end();

    // menu & helper
    void menu__set_fps(Print &out, char *command);
    void menu__start_loop_n_times(Print &out, char *command);

    void menu__set_pixel_index(Print &out, char *command);
    void menu__set_pixel(Print &out, char *command);
    void menu__set_all_pixel(Print &out, char *command);
    void menu__test_buffer(Print &out);
    void menu__time_meassurements(Print &out);

    void menu__set_hue(Print &out, char *command);
    void menu__set_saturation(Print &out, char *command);
    void menu__set_contrast(Print &out, char *command);
    void menu__set_brightness(Print &out, char *command);

    void menu__test_colors(Print &out);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // helper
    float set_brightness(float brightness_);
    float set_hue(float hue_);
    float set_saturation(float saturation_);
    uint16_t set_fps(uint16_t fps_);
    uint16_t get_fps();

    void start_loop_n_times(uint16_t count);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // configurations

    bool animation_run = true;
    uint16_t animation_loopcount = 0;

    uint16_t effect_duration = 3 * 1000; //ms

    // lounge blue - night
    // float hue = 0.7;
    // float contrast = 1.5;
    // float brightness = 0.01;

    // forest green - daylight
    float hue = 0.05;
    float saturation = 1.0;
    float contrast = 1.0;
    float brightness = 0.006;
    const float brightness_max = 0.37;
    const uint16_t brightness_max_i = 25000;

    // const float PI = 3.141592;
    // is already defined by arduino or some other defaults...

private:

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // private functions

    // animation
    void animation_init(Stream &out);
    void animation_update();
    void calculate_effect_position();
    void set_effect_position(float position);
    void animation_reset();


    void effect__pixel_checker();
    void effect__line();

    PixelPos * pixel_pos = new PixelPos();
    // ^ used by effect_Matrix2D -
    // defined globally maybe for performance reasons.
    // (test did not show difference.)
    void effect_Matrix2D();
    CHSV effect_Matrix2D_get_pixel( __attribute__((unused)) PixelPos * pixel_pos);
    // CHSV effect_Matrix2D_get_pixel(float col, float row, float offset);
    // CHSV effect_Matrix2D_get_pixel(
    //     float col, float row,
    //     uint16_t col_i, uint16_t row_i,
    //     float offset);
    // others

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // attributes
    bool ready;

    uint32_t effect_start = 0;
    uint32_t effect_end = 0;
    uint32_t effect_fps_loopcount = 0;
    // double effect_position = 0.0;
    float effect_position = 0.0;

    const uint16_t fps_min = 1;
    const uint16_t fps_max = 10000;
    uint16_t fps = 10000;
    uint32_t effect_update_last_us = 0;
    uint32_t effect_update_delay_us = 100;

};  // class MyAnimation

#endif  // MyAnimation_H_
