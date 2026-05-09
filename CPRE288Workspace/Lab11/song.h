/*
 * @file song.h
 * @brief File to load custom songs to the CyBot
 * @author Michael Farmer
 * @date 4/14/2026
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
