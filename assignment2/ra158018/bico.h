/**************************************************************** 
 * Description: Uoli Control Application Programming Interface.
 *
 * Authors: Edson Borin (edson@ic.unicamp.br)
 *
 * Date: 2015
 ***************************************************************/
#ifndef API_ROBOT2_H
#define API_ROBOT2_H

/**************************************************************/
/* Motors                                                     */
/**************************************************************/

/* 
 * Sets motor speed. 
 * Parameters: 
 *   id: the motor id (0 for left motor, 1 for right motor)
 *   speed: the motor speed (Only the last 6 bits are used)
 * Returns:
 *   void
 */
void set_motor_speed(unsigned char id, unsigned char speed);

/* 
 * Sets both motors speed. 
 * Parameters: 
 *   spd_m0: the speed of motor 0 (Only the last 6 bits are used)
 *   spd_m1: the speed of motor 1 (Only the last 6 bits are used)
 * Returns:
 *   void
 */
void set_motors_speed(unsigned char spd_m0, unsigned char spd_m1);

/**************************************************************/
/* Sonars                                                     */
/**************************************************************/
/* 
 * Reads one of the sonars.
 * Parameter: 
 *   dist: pointer to variable that will receive the distance.
 *   sonar_id: the sonar id (ranges from 0 to 15).
 * Returns:
 *   void
 */
void read_sonar(unsigned char sonar_id, unsigned short* dist);

/* 
 * Reads all sonars at once.
 * Parameter: 
 *   sonars: array of 16 unsigned integers. The distances are stored
 *   on the array.
 * Returns:
 *   void
 */
void read_sonars(unsigned int *distances);

/* 
 * Register a function f to be called whenever the robot gets close to an object. The user
 * should provide the id of the sensor that must be monitored (sensor_id), a threshold 
 * distance (dist_threshold) and the user function that must be called. The system will register
 * this information and monitor the sensor distance every DIST_INTERVAL cycles. Whenever the sensor
 * distance becomes smaller than the dist_threshold, the system calls the user function.
 *
 * Parameters: 
 *   sensor_id: id of the sensor that must be monitored.
 *   sensor_threshold: threshold distance.
 *   f: addres of the function that should be called when the robot gets close to an object.
 * Returns:
 *   void
 */
void register_proximity_callback(unsigned char sensor_id, unsigned short dist_threshold, void (*f)());

/**************************************************************/
/* Timer                                                      */
/**************************************************************/
/* 
 * Adds an alarm to the system.
 * Parameter: 
 *   f: function to be called when the alarm triggers.
 *   time: the time to invoke the alarm function.
 * Returns:
 *   void
 */
void add_alarm(void (*f)(), unsigned int time);

/* 
 * Reads the system time.
 * Returns:
 *   the system time.
 */
unsigned int get_time();

/* 
 * Sets the system time.
 * Parameter: 
 *   t: the new system time.
 */
void set_time(unsigned int t);


#endif // API_ROBOT2_H
