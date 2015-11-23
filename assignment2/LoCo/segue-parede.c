/* ********************************************************************* *
 * 
 *                              Segue parede (INCOMPLETE)
 * Author: Isadora Sophia Garcia Rodopoulos
 * RA: 158018
 * Description: Uoli's control logic as "Segue parede"
 *
 * ********************************************************************* */

#include <stdio.h>
#include "../BiCo/api_robot2.h"

#define uint unsigned int

#define DEFAULT_SPEED  25      /* Default speed */
#define MIN_INTERVAL   15      /* Min. interval to turn 90 degrees */

#define MAX_UNITY      50      /* Max. unit of time before reset */
#define MIN_DISTANCE   1200    /* Min. distance to a wall */

#define LEFT_SONAR_ID  3
#define RIGHT_SONAR_ID 4

#define true  1                /* Bool */
#define false 0

int main() {
    /* Set proximity callbacks */
    registry_proximity_callback(LEFT_SONAR_ID, MIN_DISTANCE, &near_wall_left);
    registry_proximity_callback(RIGHT_SONAR_ID, MIN_DISTANCE, &near_wall_right);

    /* Go! */
    forward();

    return 0;
}

void busca_parede() {
    /* Set proximity callbacks */
    registry_proximity_callback(LEFT_SONAR_ID, MIN_DISTANCE, &near_wall_left);
    registry_proximity_callback(RIGHT_SONAR_ID, MIN_DISTANCE, &near_wall_right);
}

void segue_parede() {

}

/* Function that will be called when the timer is up */
void timer() {
    turn_90_right();

    total_time++;

    /* Make sure unit is accordingly */
    if (total_time > MAX_UNITY) {
        total_time = 1;
    }

    set_alarm();
}

void left_wall() {
    uint i, t;

    i = get_time();

    set_motors_speed(DEFAULT_SPEED/2, 0);

    t = get_time();

    /* Keeps turning until 90 degrees was made 
     * according to MIN_INTERVAL */
    while (t - i < MIN_INTERVAL) {
        t = get_time();
    }

    forward();
}

void forward() {
    set_motors_speed(DEFAULT_SPEED, DEFAULT_SPEED);
}

void near_wall_left() {
    turn_90_right();

    registry_proximity_callback(LEFT_SONAR_ID, MIN_DISTANCE, &near_wall_left);
}

void near_wall_right() {
    turn_90_right();

    registry_proximity_callback(RIGHT_SONAR_ID, MIN_DISTANCE, &near_wall_right);
}
