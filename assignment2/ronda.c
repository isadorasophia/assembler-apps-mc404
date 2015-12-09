/* ********************************************************************* *
 * 
 *                                 Ronda
 * Author: Isadora Sophia Garcia Rodopoulos
 * RA: 158018
 * Description: Uoli's control logic as "Ronda"
 *
 * ********************************************************************* */

#include "bico.h"

#define uint unsigned int

#define DEFAULT_SPEED  25      /* Default speed */

#define ALARM_INT      780     /* Min. interval to stop turning 90 degrees */
#define DEFAULT_INT    150     /* Interval when hitting a wall */

#define MIN_DISTANCE   1200    /* Min. distance to a wall */

#define MAX_UNIT       50      /* Max. unit of time before reset */

#define UNIT           30      /* Unit of time before turning 90 degrees */

#define LEFT_SONAR_ID  3
#define RIGHT_SONAR_ID 4

#define true  1                /* Bool */
#define false 0

#define ever (;;)

uint total_time;

/* Functions signatures */
void set_alarm();
void forward();
void timer();
void turn_90_right();
void turn_right();
void delay(int);

void _start(void) {
    /* Initialize timer */
    total_time = 1;

    /* Set alarm */ 
    set_alarm();

    /* Go! */   
    forward();

    /* Set proximity callbacks */
    register_proximity_callback(LEFT_SONAR_ID, MIN_DISTANCE, &turn_right);
    register_proximity_callback(RIGHT_SONAR_ID, MIN_DISTANCE, &turn_right);

    for ever;
}

void set_alarm() {
    uint i = get_time();

    add_alarm(&timer, i + (total_time * UNIT));
}

void forward() {
    set_motors_speed(DEFAULT_SPEED, DEFAULT_SPEED);
}

/* Function that will be called when the timer is up! */
void timer() {
    turn_90_right();

    total_time++;

    /* Make sure unit is accordingly */
    if (total_time > MAX_UNIT) {
        total_time = 1;
    }

    set_alarm();
}

/* In order to complete a square */
void turn_90_right() {
    set_motors_speed(0, DEFAULT_SPEED/2);

    /* Keeps turning until 90 degrees was made 
     * according to ALARM_INT */
    delay(ALARM_INT);

    forward();
}

/* In order to avoid a wall */
void turn_right() {
    set_motors_speed(0, DEFAULT_SPEED/2);

    /* Keeps turning until 90 degrees was made 
     * according to DEFAULT_INT */
    delay(DEFAULT_INT);

    forward();
}

/* Wait for delay_time seconds */
void delay(int delay_time) {
    uint i, t;

    i = get_time();
    t = get_time();

    /* Keeps turning until 90 degrees was made 
     * according to DEFAULT_INT */
    while (t < i + delay_time) {
        t = get_time();
    }
}
