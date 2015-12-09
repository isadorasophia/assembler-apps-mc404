/* ********************************************************************* *
 * 
 *                              Segue parede
 * Author: Isadora Sophia Garcia Rodopoulos
 * RA: 158018
 * Description: Uoli's control logic as "Segue Parede"
 *
 * ********************************************************************* */

#include "bico.h"

#define LEFT_FRONT_SONAR     3
#define RIGHT_FRONT_SONAR    4

#define MIN_DISTANCE      600       /* Min. distance to a wall */
#define FOLLOW_DISTANCE   200       /* Min. distance when following a wall */

#define DEFAULT_INT       10

#define DEFAULT_SPEED     40

#define bool              int 

#define true  1                     /* Bool */
#define false 0

#define ever (;;)

void search_wall(unsigned short, unsigned short);
void follow_wall();
void align();
void delay(int);
void turn_w_wall();
void forward ();
void turn();

void _start(void) {
    search_wall(LEFT_FRONT_SONAR, RIGHT_FRONT_SONAR);

    follow_wall();
}

void search_wall(unsigned short s_1, unsigned short s_2) {
    unsigned short t_1, t_2, t;
    bool found = false;

    /* Go! */   
    forward();

    /* Keep going until a wall is found */
    for ever {
        read_sonar(LEFT_FRONT_SONAR, &t_1);
        read_sonar(RIGHT_FRONT_SONAR, &t_2);

        t = t_1 > t_2 ? t_1 : t_2;

        /* If a wall is detected */
        if (t <= MIN_DISTANCE) {
            return;
        }
    }
}

void follow_wall() {
    register_proximity_callback(LEFT_FRONT_SONAR, FOLLOW_DISTANCE, &turn);
    register_proximity_callback(RIGHT_FRONT_SONAR, FOLLOW_DISTANCE, &turn);

    forward();

    for ever {        
        align();

        turn_w_wall();
    }
}

void align() {
    unsigned short t_1, t_2, t;
    bool wall = true;

    /* Stay aligned with the wall */
    for ever {
        read_sonar(3, &t_1);
        read_sonar(4, &t_2);

        t = t_1 > t_2 ? t_1 : t_2;

        if (t > FOLLOW_DISTANCE) {
            return;
        }
    }
}

/* Turn with the wall */
void turn_w_wall(){
    unsigned short t_1, t_2, t;

    read_sonar(7, &t_1);        /* Right */
    read_sonar(0, &t_2);        /* Left */

    if (t_1 < t_2) {    /* Right */
        set_motors_speed(0, DEFAULT_SPEED/2);

        search_wall(7, 8);
    } else {            /* Left */
        set_motors_speed(DEFAULT_SPEED/2, 0);

        search_wall(0, 15);
    }

    forward();
}

/* Wait for delay_time seconds */
void delay(int delay_time) {
    unsigned int i, t;

    i = get_time();
    t = get_time();

    /* Keeps turning until delay_time */
    while (t < i + delay_time) {
        t = get_time();
    }
}

void forward () {
    set_motors_speed(DEFAULT_SPEED/2, DEFAULT_SPEED/2);
}

void turn () {
    set_motors_speed(DEFAULT_SPEED/2, 0);

    delay(DEFAULT_INT);

    forward();
}
