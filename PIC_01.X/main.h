#ifndef MAZE_H
#define	MAZE_H

#define CONFIGURE_BLUETOOTH 0

#define MAZE_SIZE_X 8
#define MAZE_SIZE_Y 6

#define CONTROL_KP 25
#define CONTROL_KI 1
#define CONTROL_KD 0

#define START_POSITION_X 0 // i changed the start position to 0
#define START_POSITION_Y 0 // because in array you start from 0?

#define WEST 0
#define NORTH 1
#define EAST 2
#define SOUTH 3

#define MO1_MAPPING_GOING_TO_CENTRE 1
#define MO2_MAPPING_GOING_TO_START_POS 2
#define MO3_QUICKEST_PATH_GOING_TO_CENTRE 3
#define MO4_FINISHED 4
#define MO5_RETURNING 5

#define CELEBRATION_TIME_INTERVAL 100

#define FALSE 0
#define TRUE 1

void update_distance_value(void);
void find_the_next_movement(unsigned char current_x, unsigned char current_y, unsigned char * no_cells, unsigned char * direction);
void newxy_from_xy(unsigned char direction, unsigned char no_cells, unsigned char current_x, unsigned char current_y, unsigned char * new_x, unsigned char * new_y);
void find_open_neighbour_cell(unsigned char current_x, unsigned char current_y, unsigned char ignore_visited_cells, unsigned char * direction);
unsigned char direction_from_xy(unsigned char current_x, unsigned char current_y, unsigned char new_x, unsigned char new_y);
void store_walls(unsigned char current_x, unsigned char current_y);
void defineWall(unsigned char x, unsigned char y, unsigned char direction, unsigned char wall);
unsigned char invert_direction(unsigned char direction);


#endif	/* MAZE_H */

