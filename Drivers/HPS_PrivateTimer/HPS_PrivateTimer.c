#include "HPS_PrivateTimer.h"

//Driver Base Addresses
volatile unsigned int   *timer_base_ptr     = 0x0;  //0xFFFEC600
unsigned int   timer0_address     = 0x0;  //0xFFFEC600
unsigned int   timer1_address     = 0x0;  //0xFFFEC600

//Driver Initialised
bool timer_initialised = false;
volatile bool timer_alt = false;

//Register Offsets
#define TIMER_LOAD      (0x00/sizeof(unsigned int))
#define TIMER_VALUE     (0x04/sizeof(unsigned int))
#define TIMER_CONTROL   (0x08/sizeof(unsigned int))
#define TIMER_INTERRUPT (0x0C/sizeof(unsigned int))

//Function to initialise the Timer
signed int Timer_initialise(unsigned int base_address){
    if (Timer_isInitialised()) {
		if (timer1_address == base_address) return TIMER_ADDRTAKEN;
		timer1_address = base_address;
		timer_alt = true;
	} else {
		timer0_address = base_address;
	}
	//Initialise base address pointers
    timer_base_ptr = (unsigned int *)  base_address;
    //Ensure timer initialises to disabled
    timer_base_ptr[TIMER_CONTROL] = 0;
    //Timer now initialised
    timer_initialised = true;
    return TIMER_SUCCESS;
}

bool Timer_isInitialised() {
    return timer_initialised;
}

//Check if driver initialised
bool Timer_isAlt() {
    return timer_alt;
}

//set the register directly
//all functions have NOINIT Error protection
signed int Timer_setControlRegister ( unsigned int control ) {
	if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
	timer_base_ptr[TIMER_CONTROL] = control;
	return TIMER_SUCCESS;
}

//changes only prescaler part of register
signed int Timer_setPrescaler(unsigned int prescaler) {
	if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
	if (prescaler > 0xFF) {
		timer_base_ptr[TIMER_CONTROL] = timer_base_ptr[TIMER_CONTROL] |(0xFF << 8);
	} else {
		timer_base_ptr[TIMER_CONTROL] = (timer_base_ptr[TIMER_CONTROL] & (0xFFFF00FF)) | (prescaler << 8);
	}
	return TIMER_SUCCESS;
}

//changes only overflow part of register
signed int Timer_setOverflow( bool state ) {
	if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
	timer_base_ptr[TIMER_CONTROL] = (timer_base_ptr[TIMER_CONTROL] & (0xFFFFFFFD)) | (state << 1);
	return TIMER_SUCCESS;
}

//Loads the value directly into timer
signed int Timer_setLoadValue ( unsigned int load) {
	if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
	timer_base_ptr[TIMER_LOAD] = load;
	return TIMER_SUCCESS;
}

//reads the timer value
signed int Timer_readCurrent ( void ) {
	if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
	return timer_base_ptr[TIMER_VALUE];
}

//checks the interupt flag
bool Timer_interuptStatus ( void ) {
	if (!Timer_isInitialised()) return false;
	return (timer_base_ptr[TIMER_INTERRUPT] & 0x1);
}

//in code enabling
signed int Timer_enable( void ) {
	if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
	timer_base_ptr[TIMER_CONTROL] = (timer_base_ptr[TIMER_CONTROL] | 0x1);
	return TIMER_SUCCESS;
}

//easy disbale function
signed int Timer_disable( void ) {
	if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
	timer_base_ptr[TIMER_CONTROL] = (timer_base_ptr[TIMER_CONTROL] & 0xFFFFFFFE);
	return TIMER_SUCCESS;
}

//is enabled
bool Timer_isEnabled(void) {
	if (!Timer_isInitialised()) return false;
	return (timer_base_ptr[TIMER_CONTROL] & 0x1);
}
	
void Timer_switch(bool alt) {
	if (alt != timer_alt) {
		if(alt) {
			timer_base_ptr = (unsigned int *)  timer1_address;
		} else {
			timer_base_ptr = (unsigned int *)  timer0_address;
		}
		timer_alt = alt;
	}
}

//reset interupt flag by setting 1
signed int Timer_resetFlag ( void ) {
	if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
	timer_base_ptr[TIMER_INTERRUPT] = 0x1;
	timer_base_ptr[3];
	return TIMER_SUCCESS;
}	
