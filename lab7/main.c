/* ********************************************************************* *
 * 
 *                         		    Lab. 7
 * Author: Isadora Sophia Garcia Rodopoulos
 * RA: 158018
 * Description: Uoli's control logic
 *
 * ********************************************************************* */

#include "api_robot.h" /* Robot control API */

#define true 1
#define false 0

#define uint unsigned int

#define MIN_DISTANCE 1200
#define DEFAULT_SPEED 25

uint min(uint, uint);

/* Main function! */
void _start(void) 
{
  uint distance;

  while (true) {
  	/* Check distance. */
  	distance = min(read_sonar(3), read_sonar(4));

  	if (distance < MIN_DISTANCE) {
  		/* Too close! Turn away. */
  		set_speed_motors(DEFAULT_SPEED/2, 0);
  	} else {
  		/* While not close to anything. */
  		set_speed_motors(DEFAULT_SPEED, DEFAULT_SPEED);
  	}
  }
}

uint min(uint a, uint b) {
	return a < b ? a : b;
}