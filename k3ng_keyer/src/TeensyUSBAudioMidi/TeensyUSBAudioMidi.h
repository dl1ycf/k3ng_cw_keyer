/* TeensyKeyer for Teensy 4.X
 * Copyright (c) 2021, kf7o, Steve Haynal, steve@softerhardware.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef TeensyUSBAudioMidi_h_
#define TeensyUSBAudioMidi_h_

#include "Arduino.h"
#include "Audio.h"
#include "AudioStream.h"
#include "arm_math.h"
#include "TeensyAudioTone.h"


class TeensyUSBAudioMidi
{
public:
    TeensyUSBAudioMidi(usb_serial_class *serial, usb_midi_class *midi) :
        usbaudioinput(),
        sine(),
        teensyaudiotone(),
        i2s(),
        patchinl (usbaudioinput,   0, teensyaudiotone, 0),
        patchinr (usbaudioinput,   1, teensyaudiotone, 1),
        patchwav (sine,            0, teensyaudiotone, 2),
        patchoutl(teensyaudiotone, 0, i2s,             0),
        patchoutr(teensyaudiotone, 1, i2s,             1),
        sgtl5000() 
    {
        serialp = serial;
        midip = midi; 

    }

    void setup(void);

    void loop(void);

    void key(int state);

    void ptt(int state);


private:
    AudioInputUSB           usbaudioinput;
    AudioSynthWaveformSine  sine;
    TeensyAudioTone         teensyaudiotone;
    AudioOutputI2S          i2s;
    AudioConnection         patchinl;
    AudioConnection         patchinr;
    AudioConnection         patchwav;
    AudioConnection         patchoutl;
    AudioConnection         patchoutr;
    AudioControlSGTL5000    sgtl5000;

    usb_serial_class        *serialp;
    usb_midi_class          *midip;
};

#endif
