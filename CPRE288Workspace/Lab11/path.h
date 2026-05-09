
/* @file path.h
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
#ifndef PATH_H_
#define PATH_H_
#include "open_interface.h"

float update_heading();

void auto_scan();

int auto_scan_info();

void auto_analyze(oi_t *sensor);

void print_gaps(Gap *gaps, int gap_count);

void auto_path(oi_t *sensor);

void auto_drive(oi_t *sensor);

#endif /* PATH_H_ */
