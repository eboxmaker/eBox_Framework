/*
file   : *.cpp
author : link
version: V1.0
date   : 2016/12/14

Copyright 2016 link. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "USBAudio.h"

// frequency: 48 kHz
#define FREQ 48000

// 1 channel: mono
#define NB_CHA 1

// length of an audio packet: each ms, we receive 48 * 16bits ->48 * 2 bytes. as there is one channel, the length will be 48 * 2 * 1
#define AUDIO_LENGTH_PACKET 48 * 2 * 1

// USBAudio
USBAudio audio(FREQ, NB_CHA);


void setup()
{
    ebox_init();
		uart1.begin(115200);
}

int main() 
{
	int16_t buf[AUDIO_LENGTH_PACKET/2];
	setup();
	while (1) {
		// read an audio packet
		audio.read((uint8_t *)buf);

		
		// print packet received
		uart1.printf("recv: ");
		for(int i = 0; i < AUDIO_LENGTH_PACKET/2; i++) {
				uart1.printf("%d ", buf[i]);
		}
		uart1.printf("\r\n");
	}
}



