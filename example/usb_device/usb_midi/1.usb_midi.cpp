/*
file   : *.cpp
author : link
version: V1.0
date   : 2016/12/14

Copyright 2016 link. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "USBMIDI.h"

USBMIDI midi;

void setup()
{
    ebox_init();
}
int main(void)
{
    setup();
    while(1)
    {
        for(int i=48; i<83; i++) {     // send some messages!
            midi.write(MIDIMessage::NoteOn(i));
            delay_ms(250);
            midi.write(MIDIMessage::NoteOff(i));
            delay_ms(500);
       }
    }

}


