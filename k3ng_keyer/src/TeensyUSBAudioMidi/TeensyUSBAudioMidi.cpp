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
    while (midip->read(2)) {
        if (midip->getType() == midip->ControlChange) {
            serialp->println("Control Change");
        }
    }
}

void TeensyUSBAudioMidi::key(int state)
{
    teensyaudiotone.setTone(state);

}

void TeensyUSBAudioMidi::ptt(int state)
{
    sine.frequency(600);
}


