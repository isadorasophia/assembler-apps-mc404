/**************************************************************** 
 * Description: Uoli Robot Application Programming Interface.
 *
 * Authors: Juliano Moscardini Bernardes 
 *          Edson Borin (edson@ic.unicamp.br)
 *
 * Date: 2014
 ***************************************************************/

#ifndef API_ROBOT_H
#define API_ROBOT_H

/* 
 * Sets motor speed. 
 * Parameters: 
 *   speed: the motor speed (Only the last 6 bits are used)
 *   id: the motor id (0 for left motor, 1 for right motor)
 * Returns:
 *   void
 */
void set_speed_motor(unsigned char speed, unsigned char id);

/* 
 * Sets both motors speed. 
 * Parameters: 
 *   spd_m0: the speed of motor 0 (Only the last 6 bits are used)
 *   spd_m1: the speed of motor 1 (Only the last 6 bits are used)
 * Returns:
 *   void
 */
void set_speed_motors(unsigned char spd_m0, unsigned char spd_m1);

/* 
 * Reads one of the sonars.
 * Parameter: 
 *   id: the sonar id (ranges from 0 to 15).
 * Returns:
 *   the distance as an integer from 0 to (2^12)-1
 */
unsigned short read_sonar(unsigned char sonar_id);

/* 
 * Reads all sonars at once.
 * Parameter: 
 *   sonars: array of 16 unsigned integers. The distances are stored
 *   on the array.
 * Returns:
 *   void
 */
void read_sonars(unsigned int *distances);


#endif // API_ROBOT_H