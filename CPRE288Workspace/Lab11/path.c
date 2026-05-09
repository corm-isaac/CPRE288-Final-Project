/* @file path.c
 *
 * @brief this is my schizoprenic code for making the autonomous part. I broke my code composer
 * at 2AM on Wednesday for some reason (probably because I kept doing git force push and broke my config files)
 * so this is all theoretical that never actually was tried (because I couldn't test it)
 * had a generational crash out
 *
 *  @author Isaac Cormier
 *  @date 5/3/2026
 *
 */

// assuming that everything else works
// IMU reads correct values and we have a map?

// Where do we want to go?
//  home

// 4 pillars

// Find Corner - To build map? (possibly obsolete)

//
// Test field; relative to the test field; tested by isaac
#define TEST_FIELD_NORTH 207.0 // field north is 207° clockwise from true north
#define TEST_FIELD_EAST 297.0
#define TEST_FIELD_SOUTH 27.0
#define TEST_FIELD_WEST 117.0

// True consts idk if we use but looks cool
#define TRUE_NORTH 0.0
#define TRUE_EAST 90.0
#define TRUE_SOUTH 180.0
#define TRUE_WEST 270.0

#include "manual.h"
#include "open_interface.h"
#include "uart-interrupt.h"
#include "logMessage.h"
#include "utility.h"
#include "servo.h"
#include "imu.h"
#include "movement.h"
float target_heading = 0.0;  // updated in corner
#define MIN_GAP_WIDTH_DEG 25 // angular minimum
volatile enum { NORTH,
                EAST,
                SOUTH,
                WEST,
                NORTH_EAST,
                SOUTH_EAST,
                SOUTH_WEST,
                NORTH_WEST,
                UNKNOWN } cybot_direction = UNKNOWN; // State of orientation/direction; possibly important

volatile enum {
    STEP_IDLE,
    STEP_SCAN,
    STEP_GAPS,
    STEP_MOVE
} auto_step = STEP_IDLE;

typedef struct
{
    int start;
    int end;
    int mid;
    int width;
} Gap; // mind the gap
// HELPER FUNCTION - get IMU heading
/*
 * @brief don't make fun of me
 * @author Isaac Cormier
 * @param none
 * @date 04/26/26
 */
float updateHeading()
{
    return imu_get_heading_deg();
}
/*
 * @brief prints Gaps in the scan
 * @author Isaac Cormier
 * @param list of gaps, the gapCount
 * @date 04/26/26
 */
void printGaps(Gap *gaps, int gap_count)
{
    logMessage(40, "\r\n--- Gaps (%d) ---\r\n", gap_count);
    int i;
    for (i = 0; i < gap_count; i++)
    {
        logMessage(40, "Gap %d: start=%d end=%d mid=%d width=%d\r\n",
                   i,
                   gaps[i].start,
                   gaps[i].end,
                   gaps[i].mid,
                   gaps[i].width);
    }
    logMessage(40, "---\r\n");
}

// Point Function; where is our end goal? We must find a corner, then depending on the heading, pick a direction to map.

// BREAD AND BUTTER -> DRIVE AND SCAN METHOD
/*
 * @brief scans the area
 * @author Isaac Cormier
 * @param
 * @date 04/26/26
 */
void auto_scan()
{ // Return number of objects found
    logMessage(20, "\r\n180 Scan\r\n");
    int i;
    for (i = 0; i < 180; i += 2)
    {
        g_object_array[i / 2].angle = i;
        g_object_array[i / 2].IR = IRScan(i);
        g_object_array[i / 2].ping = PingScan(i);
    }
    // lcd_printf("%d", i);
    // logScan(); //prints ScanPointArray
    // Finds objects in theory
}
/*
 * @brief returns the number of objects in the scan and cleans objectArray
 * @author Isaac Cormier
 * @param (degrees)
 * @date 04/26/26
 */
int auto_scanInfo()
{ // returns the number of objects
    // cleans objectArray first
    Obstacle empty_struct = {
        .primary_id = -1, // sentinel value
        .start_angle = 0,
        .end_angle = 0,
        .middle_angle = 0,
        .distance_cm = 0.0f,
        .radial_width = 0};
    int i;
    for (i = 0; i < 30; i++)
    {
        g_object_array[i] = empty_struct;
    }
    // calls auto scan first to populate array
    auto_scan();
    // returns the number of objects in the bot
    int number_of_objects = object_determination();
    print_objects(number_of_objects);
    return number_of_objects;
}

// Corner function
/*
 * @brief aim is to find a corner, works in my mind
 * @author Isaac Cormier
 * @param
 * @date 04/26/26
 */
void auto_corner()
{
    // First we scan!
    int number_of_objects = auto_scan_info(); // scanInfo calls scan

    // Next is Evaluation; Make a new array only with real objects
    Obstacle real_objects[number_of_objects];
    int i;
    for (i = 0; i < number_of_objects; i++)
    {
        real_objects[i] = g_object_array[i];
    }
    target_heading = update_heading();
    // Then we need to proceed in a direction and stick to it; choose a direction and set the target heading. then we can call auto analyze
    // to proceed until we hit tape
    int left_count = 0;  // angles 0-89
    int right_count = 0; // angles 90-178

    for (i = 0; i < number_of_objects; i++)
    {
        if (real_objects[i].mid < 90)
            left_count++;
        else
            right_count++;
    }

    if (left_count <= right_count)
        target_heading += 90; // turn left
    else
        target_heading -= 90; // turn right
}
/*
 * @brief i don't know
 * @author Isaac Cormier
 * @param
 * @date 04/26/26
 */
void auto_boundary()
{
    return;
}
/*
 * @brief when it hits a corner it should do something important
 * @author Isaac Cormier
 * @param
 * @date 04/26/26
 */
// after hitting tape...
void auto_corner_tape()
{
}

// Drive Forward and scan
/*
 * @brief drive forward and scan logic (finds gaps and finds the best one in terms of a heading)
 * @author Isaac Cormier
 * @param oi_sensor interface
 * @date 04/26/26
 */
void auto_analyze(oi_t *sensor)
{ // drive and scan logic

    float current_heading = update_heading();

    // point bot to heading
    float heading_difference = target_heading - current_heading;
    if (heading_difference < 0)
        heading_difference += 360;

    turn_left(heading_difference);

    // First we scan!
    int number_of_objects = auto_scan_info(); // scanInfo calls scan

    // Case 0: no objects
    if (number_of_objects == 0)
    {
        move_forward(sensor, 250); // half of IR range
        return;
    }

    // Next is Evaluation; Make a new array only with real objects
    Obstacle real_objects[number_of_objects];
    int i;
    for (i = 0; i < number_of_objects; i++)
    {
        real_objects[i] = g_object_array[i];
    }

    // Then, we check for gaps. Any gap large enough for the cybot to fit and that fits the best with the target_heading (where we are trying to go) is the best fit.

    Gap gaps[10];
    // Finding gaps based off object array
    // gap before first object
    // gaps between objects
    int gap_start = 0; // start of current gap
    int object_count = 0;
    int gap_count = 0;
    for (i = 0; i <= 178; i += 2)
    {
        if (object_count >= number_of_objects)
            break;
        if (i == real_objects[object_count].start_angle)
        {
            gaps[gap_count].start = gap_start;
            gaps[gap_count].end = i;
            gaps[gap_count].mid = (gap_start + i) / 2;
            gaps[gap_count].width = i - gap_start;
            gap_count++;
            i = real_objects[object_count].end_angle; // skip past object
            gap_start = i;                            // next gap starts here
            object_count++;
        }
    }
    // final gap
    gaps[gap_count].start = gap_start;
    gaps[gap_count].end = 178;
    gaps[gap_count].mid = (gap_start + 178) / 2;
    gaps[gap_count].width = 178 - gap_start;
    gap_count++;

    // gap weights (i.e find the best gap that can fit the cybot and are in the target heading)
    // best case: there is a gap in the same direction as the heading
    // case: there is a large gap not in the same direction but closest
    // case: there is no gaps large enough for the cybot -> then we turn towards the biggest edge case gap and proceed forward
    // first figure out which ones can fit the cybot (cybot length = 32cm)
    // printGaps(gaps, gapCount);

    int best_gap = -1;
    float best_score = -1;

    for (i = 0; i < gap_count; i++)
    {
        if (gaps[i].width < MIN_GAP_WIDTH)
            continue;

        float angle_diff = fabsf(gaps[i].mid - 90.0); // heading check (which is closest to our target heading)
        float score = 180.0 - angle_diff;             // higher = closer to center

        if (score > best_score) // checking minumum
        {
            best_score = score;
            best_gap = i;
        }
    }

    if (best_gap == -1)
        move_backward(sensor, 50);

    if (best_score > 0)
        turn_left(sensor, best_score);
    else if (best_score < 0)
        turn_right(sesnor, best_score);

    move_forward(sensor, 250);
}

// DRIVE FUNCTION - ONLY ONE THAT IS CALLED BY ANYTHING OUTSIDE OF PATH
/*
 * @brief much like man_drive but in auto! idea is to have different phases the user can break out of since C is not multithreaded
 * @author Isaac Cormier
 * @param oi_sensor interface
 * @date 04/26/26
 */
void auto_drive(oi_t *sensor)
{
    uart_sendStr("Autonomous Driving \r\n");
    uart_sendStr("Toggle to Manual - 't'\r\n");

    command_byte = 'n';
    char exit_char = 0;
    while (!exit_char)
    {
        while (command_flag != 1)
            ;
        switch (command_byte)
        {
        case 't': // back to stick
            man_drive(sensor);
            command_byte = 'n';
            break;
        case 'a': // a- carry out next command in algorithim
            switch (auto_step)
            {
            case STEP_IDLE:
                log_message(40, "\r\nStarting: Scanning...\r\n");
                auto_corner(); // sets target heading
                auto_step = STEP_SCAN;
                break;
            case STEP_SCAN:
                log_message(40, "\r\nStep: Finding gaps...\r\n");
                auto_scan_and_gaps(sensor); // just the scan + gap finding, no movement
                auto_step = STEP_GAPS;
                break;
            case STEP_GAPS:
                log_message(40, "\r\nStep: Turning to best gap...\r\n");
                auto_turn_to_gap(sensor); // just the turn
                auto_step = STEP_MOVE;
                break;
            case STEP_MOVE:
                log_message(40, "\r\nStep: Moving forward...\r\n");
                move_forward(sensor, 250);
                auto_step = STEP_SCAN; // loop back to scan next
                break;
            }
            command_byte = 'n';
            break;
            // Don't forget to clear objectArray
        case 'e': // exit
            exit_char = 1;
            log_message(80, "\r\nHypothetically I turn the cybot off\r\n");
            command_byte = 'n';
            break;
        case 'n':
            log_message(15, "\r\nWaiting...\r\n");
            while (command_byte == 'n')
                ;
            break;
        default:
            log_message(40, "\r\nButton not pressed correctly: %c\r\n", command_byte);
            command_byte = 'n';
        }
    }
}
