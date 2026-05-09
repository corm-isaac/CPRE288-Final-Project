/*
 * @file song.c
 * @brief File to load custom songs to the CyBot
 * @author Michael Farmer
 * @date 4/14/2026
 */

#include "song.h"
#include "uart-interrupt.h"
#include "open_interface.h"

#define OI_OPCODE_SONG 140
#define OI_OPCODE_PLAY 141

/*
 * Function to load any given song to the CyBot
 * @author Michael Farmer
 * @param song_index The number ID of song writing to
 * @param num_notes The number of notes in size of notes array 
 * @param notes An integer array of MIDI note pitches
 * @param duration An integer array of duration for each corresponding notes, in 1/16ths of a second
 * @date 4/14/2026
 */
void load_song_to_bot(int song_index, int num_notes, unsigned char *notes, unsigned char *duration)
{
    int i;
    oi_uartSendChar(OI_OPCODE_SONG);
    oi_uartSendChar(song_index);
    oi_uartSendChar(num_notes);
    for (i = 0; i < num_notes; i++) {
        oi_uartSendChar(notes[i]);
        oi_uartSendChar(duration[i]);
    }
}

/*
 * Create song arrays, essentially an init function. Define songs here.
 * @author Michael Farmer
 * @date 4/14/2026
 */
void load_songs()
{
    int song1_num = 1;
    int song1_num_notes = 20;
    unsigned char song1_notes[] =      {62, 62, 74, 69, 68, 67, 65, 62, 65, 67, 62, 62, 74, 69, 68, 67, 65, 62, 65, 67}; //Undertale
    unsigned char song1_lengths[] =    {10, 10, 20, 30, 20, 20, 20, 10, 10, 10, 10, 10, 20, 30, 20, 20, 20, 10, 10, 10};



    //unsigned char song1_notes[] =   {84, 88, 91, 96, 96, 96, 96, 91, 91, 91, 91, 88, 91, 88, 84};
    //unsigned char song1_lengths[] = {10, 10, 10, 15, 5, 5, 5, 15, 5, 5, 5, 10, 10, 10, 20};
    //int song1_num_notes = 15;
    //int song1_num = 1;

    load_song_to_bot(song1_num, song1_num_notes, song1_notes, song1_lengths);
}

/*
 * Tell the CyBot to actually play a song
 * @author Michael Farmer
 * @param index The index of the song to play
 * @date 4/14/2026
 */
void play_song(int index)
{
    oi_uartSendChar(OI_OPCODE_PLAY);
    oi_uartSendChar(index);
}

