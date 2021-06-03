/* 
 * HPS Timer Controller
 * ------------------------------
 * Description: 
 * Driver for the HPS Private Timer in the DE1-SoC
 * Computer
 *
 * Company: University of Leeds
 * Author: T Carpenter and S Tayyab 
 *
 */

#ifndef HPS_PRIVATE_TIMER_H_
#define HPS_PRIVATE_TIMER_H_

//Required Files
#include <stdbool.h>

//Error Codes
#define TIMER_SUCCESS       0
#define TIMER_ERRORNOINIT  -1
#define TIMER_ADDRTAKEN    -2

//Function to initialise the HPS Timer
// - Returns TIMER_SUCCESS if successful
signed int Timer_initialise(unsigned int base_address);

//Check if driver initialised
// - returns true if initialised
bool Timer_isInitialised( void );

//sets control register directly
// - returns error if occured
signed int Timer_setControlRegister ( unsigned int control );

//sets prescaler only
// -returns error is occured
signed int Timer_setPrescaler(unsigned int prescaler);

//sets Overflow allowing timer reset
// - returns error if occured
signed int Timer_setOverflow( bool state ) ;

//sets Load value 
// -retuns error if occured
signed int Timer_setLoadValue ( unsigned int load);

//reads the Timers current Value
// - Returns signed int is error and unsigned value of the timer
signed int Timer_readCurrent ( void );

//gets the status of interupt flag
//returns true if interupt is active and timer stopped
bool Timer_interuptStatus ( void ) ;

//returns the timer state
bool Timer_isEnabled(void);

//enables timer
signed int Timer_enable( void );

//disables timer
signed int Timer_disable( void );

//resets interupt flag to allow timer to contine
signed int Timer_resetFlag ( void );
   
//Returns activtive timer Number
bool Timer_isAlt( void );

//Swaps to Timer Number
void Timer_switch(bool alt);

#endif
