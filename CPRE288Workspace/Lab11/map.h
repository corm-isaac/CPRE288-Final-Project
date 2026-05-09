/*
 * @file map.h
 * @brief
 * if you haven't read path.c yet you probably should but this is supposed to go with path.c in the sense the autonomous bot
 * is supposed to determine the object coordinates. the map was supposed to have a 2D map with coordinates of every object
 * but i didn't have enough time so unfortunately the only glimpse into my mind is this method. this is also untested for same reasons
 * @author Isaac Cormier
 * @date 5/3/2026
 */

#ifndef MAP_H_
#define MAP_H_

typedef struct
{
    float x;
    float y;
} Point;

Point determine_object_coordinates(int object_id);

#endif /* MAP_H_ */
