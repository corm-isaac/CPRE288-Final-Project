/*
*
*  Michael Farmer Lab Project
*  4/14/2026
*/

#include "song.h"
#include "uart-interrupt.h"

#define OI_OPCODE_SONG 140
#define OI_OPCODE_PLAY 141

void load_song_to_bot(int song_index, int num_notes, unsigned char *notes, unsigned char *duration)
{
    int i;
    uart_sendChar4(OI_OPCODE_SONG);
    uart_sendChar4(song_index);
    uart_sendChar4(num_notes);
    for (i = 0; i < num_notes; i++) {
        uart_sendChar4(notes[i]);
        uart_sendChar4(duration[i]);
    }
}

void load_songs()
{
    int song1_num = 1;
    int song1_num_notes = 20;
    unsigned char song1_notes[] =      {62, 62, 74, 69, 68, 67, 65, 62, 65, 67, 62, 62, 74, 69, 68, 67, 65, 62, 65, 67};
    unsigned char song1_lengths[] =    {10, 10, 20, 30, 20, 20, 20, 10, 10, 10, 10, 10, 20, 30, 20, 20, 20, 10, 10, 10};



    //unsigned char song1_notes[] =   {84, 88, 91, 96, 96, 96, 96, 91, 91, 91, 91, 88, 91, 88, 84};
    //unsigned char song1_lengths[] = {10, 10, 10, 15, 5, 5, 5, 15, 5, 5, 5, 10, 10, 10, 20};
    //int song1_num_notes = 15;
    //int song1_num = 1;

    load_song_to_bot(song1_num, song1_num_notes, song1_notes, song1_lengths);
}

void play_song(int index)
{
    uart_sendChar4(OI_OPCODE_PLAY);
    uart_sendChar4(index);
}
