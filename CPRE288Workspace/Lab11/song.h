/*
*
*   uart-interrupt.h
*
*   Used to set up the RS232 connector and WIFI module
*   Uses RX interrupt
*   Functions for communicating between CyBot and PC via UART1
*   Serial parameters: Baud = 115200, 8 data bits, 1 stop bit,
*   no parity, no flow control on COM1, FIFOs disabled on UART1
*
*   @author Dane Larson
*   @date 07/18/2016
*   Phillip Jones updated 9/2019, removed WiFi.h, Timer.h
*   Diane Rover updated 2/2020, added interrupt code
*/

#ifndef OI_SONG_H_
#define OI_SONG_H_

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <open_interface.h>

void load_song_to_bot(int song_index, int num_notes, unsigned char *notes, unsigned char *duration);
void load_songs();
void play_song(int index);



#endif /* OI_SONG_H_ */
