/*
*
*  Michael Farmer Lab Project
*  4/14/2026
*/

#include "oi_song.h"

void load_songs() {
    unsigned char song1_notes[] =   {84, 88, 91, 96, 96, 96, 96, 91, 91, 91, 91, 88, 91, 88, 84};
    unsigned char song1_lengths[] = {10, 10, 10, 15, 5, 5, 5, 15, 5, 5, 5, 10, 10, 10, 20};
    int song1_num_notes = 15;
    int song1_num = 1;

    unsigned char song2_notes[] =   {41, 35};
    unsigned char song2_lengths[] = {71, 71};
    int song2_num_notes = 2;
    int song2_num = 2;

    oi_loadSong(song1_num, song1_num_notes, song1_notes, song1_lengths);
    oi_loadSong(song2_num, song2_num_notes, song2_notes, song2_lengths);
}
