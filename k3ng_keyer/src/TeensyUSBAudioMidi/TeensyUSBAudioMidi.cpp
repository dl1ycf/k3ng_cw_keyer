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

#include <Arduino.h>
#include "TeensyUSBAudioMidi.h"
#include "utility/dspinst.h"
#include "../../keyer_settings_teensy_usbaudiomidi.h"

void TeensyUSBAudioMidi::setup(void)
{
    AudioMemory(16);
    AudioNoInterrupts();

    sine.frequency(600);
    sine.amplitude(1.0);
    sgtl5000.enable();
    sgtl5000.volume(0.8);

    AudioInterrupts();

}

void TeensyUSBAudioMidi::loop(void)
{
    uint cmd, data;
    static uint lsb_data = 0;

    // Expect connection on channel 2
    while (usbMIDI.read(2)) {
        if (usbMIDI.getType() == usbMIDI.ControlChange) {
            cmd  = usbMIDI.getData1();
            data = usbMIDI.getData2();

            switch(cmd) {
                case 0 :
                    // Set lsb_data for multi transfer
                    lsb_data = data;
                    //Serial.print("lsb ");
                    //Serial.println(lsb_data);
                    break;

                case 4 :
                    // Set WPM
                    speed_set(data);
                    //winkey_port_write(((data - pot_wpm_low_value)|128),0);
                    break;

                case 5 :
                    // Set sidetone amplitude
                    lsb_data = (data << 7) | lsb_data;
                    sine.amplitude(float(lsb_data)/16384.0);
                    //Serial.print("ampl ");
                    //Serial.print(data);
                    //Serial.print(" ");
                    //Serial.println(lsb_data);
                    break;

                case 6 :
                    // Set sidetone frequency
                    lsb_data = (data << 7) | lsb_data;
                    sine.frequency(float(lsb_data));
                    //Serial.print("freq ");
                    //Serial.print(data);
                    //Serial.print(" ");
                    //Serial.println(lsb_data);
                    break;

                case 16 :
                    // Set i2s volume, multi transfer
                    lsb_data = (data << 7) | lsb_data;
                    sgtl5000.volume(float(lsb_data)/16384.0);
                    break;

                default :
                    Serial.print("Unrecognized MIDI command ");
                    Serial.println(cmd);

            }
        }
    }
}

void TeensyUSBAudioMidi::key(int state)
{
    teensyaudiotone.setTone(state);
    if (state) {
        usbMIDI.sendNoteOn(1, 99, 1);
    } else {
        usbMIDI.sendNoteOff(1, 0, 1);
    }
}

void TeensyUSBAudioMidi::ptt(int state)
{
    ;
}


