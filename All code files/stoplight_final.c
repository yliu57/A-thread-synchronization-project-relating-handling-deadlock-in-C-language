/* 
 * stoplight.c
 *
 * 31-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: You can use any synchronization primitives available to solve
 * the stoplight problem in this file.
 */


/*
 * 
 * Includes
 *
 */

#include <types.h>
#include <lib.h>
#include <test.h>
#include <thread.h>


/*
 *
 * Constants
 *
 */

/*
 * Number of vehicles created.
 */

#define NVEHICLES 20
struct lock *portions[3]; //The three intersection portions 0->AB,1->BC,2->CA
int count=0;//The counter to terminate the parent thread of the vehicles

/*
 *
 * Function Definitions
 *
 */



/*
 * turnleft()
 *
 * Arguments:
 *      unsigned long vehicledirection: the direction from which the vehicle
 *              approaches the intersection.
 *      unsigned long vehiclenumber: the vehicle id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a left turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnleft(unsigned long vehicledirection,
		unsigned long vehiclenumber,
		unsigned long vehicletype)
{
	/*
	 * Avoid unused variable warnings.
	 */
	(void) vehicledirection;
	(void) vehiclenumber;
	(void) vehicletype;
	
	unsigned long temp, number1,number2; // variables used for marking the intersection portions
	
	//The three intersection portions 0->AB,1->BC,2->CA
	// 	Route A: 0,1,2; Route B: 1,2,0; Route C: 2,0,1
	number1=vehicledirection;
	number2=(vehicledirection+1)%3;
	
	//Sequence is in ascending order to avoid deadlock
	if(number1>number2)
	{
		temp=number1;
		number1=number2;
		number2=temp;
	}
	// The vehicle approaches(0), turn left(1) and leave the intersection(2)
	display(vehicletype,vehiclenumber,vehicledirection,'L',0);
	lock_acquire(portions[number1]);
	lock_acquire(portions[number2]);
	display(vehicletype,vehiclenumber,vehicledirection,'L',1);
	lock_release(portions[number1]);       
	lock_release(portions[number2]);
	display(vehicletype,vehiclenumber,vehicledirection,'L',2);
	count++;  // count the number of vehicles crossed the intersection
}


/*
 * turnright()
 *
 * Arguments:
 *      unsigned long vehicledirection: the direction from which the vehicle
 *              approaches the intersection.
 *      unsigned long vehiclenumber: the vehicle id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a right turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnright(unsigned long vehicledirection,
		unsigned long vehiclenumber,
		unsigned long vehicletype)
{
	/*
	 * Avoid unused variable warnings.
	 */
	(void) vehicledirection;
	(void) vehiclenumber;
	(void) vehicletype;
	
	unsigned long number1; // variables used for marking the intersection portions
	
	//The three intersection portions 0->AB,1->BC,2->CA
	// 	Route A: 0,1,2; Route B: 1,2,0; Route C: 2,0,1
	number1=vehicledirection;
	
	// The vehicle approaches(0), turn right(1) and leave the intersection(2)
	display(vehicletype,vehiclenumber,vehicledirection,'R',0);
	lock_acquire(portions[number1]);
	display(vehicletype,vehiclenumber,vehicledirection,'R',1);
	lock_release(portions[number1]);       
	display(vehicletype,vehiclenumber,vehicledirection,'R',2);
	count++;  // count the number of vehicles crossed the intersection
}


/*
 * approachintersection()
 *
 * Arguments: 
 *      void * unusedpointer: currently unused.
 *      unsigned long vehiclenumber: holds vehicle id number.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Change this function as necessary to implement your solution. These
 *      threads are created by createvehicles().  Each one must choose a direction
 *      randomly, approach the intersection, choose a turn randomly, and then
 *      complete that turn.  The code to choose a direction randomly is
 *      provided, the rest is left to you to implement.  Making a turn
 *      or going straight should be done by calling one of the functions
 *      above.
 */

static
void
approachintersection(void * unusedpointer,
		unsigned long vehiclenumber)
{
	int vehicledirection, turndirection, vehicletype;

	/*
	 * Avoid unused variable and function warnings.
	 */

	(void) unusedpointer;
	(void) vehiclenumber;
	(void) turnleft;
	(void) turnright;

	/*
	 * vehicledirection is set randomly.
	 */

	vehicledirection = random() % 3;
	turndirection = random() % 2;
	vehicletype = random() % 2;
	
	if(turndirection == 0){		// turndirection 0 -> turn left
		turnleft(vehicledirection,vehiclenumber,vehicletype);
	}
	else{						// turndirection 1 -> turn right
		turnright(vehicledirection,vehiclenumber,vehicletype);
	}
}

/*
* * display()
* *
* * Arguments: 
* *      unsigned long vehicletype:  the vehicle type for printing.
* *      unsigned long vehiclenumber: the vehicle id number for printing.
* *		 unsigned long vehicledirection: the direction from which the vehicle approaches the intersection.
* *      char turndirection:The two directions in which the vehicle can possibly go, 'L'(Left) or 'Right'(Right)
* *      int action: The value of n represents: 
* *				0 -> Approaching Intersection  
* *				1-> Enter the Intersection   
* *				2-> Leave the Intersection
* *	
* * Returns:
* *      nothing.
* *
* * Notes:
* *      The display function is used to display the vehicle type, vehicle number, vehicle approach 
* * 	 direction, turndirection, vehicle leave direction, 
* *		 Action(Approaching Intersection / Enter the Intersection / Leave the Intersection)
* *      
* *     
* *     
* */		
void display(unsigned long vehicletype, unsigned long vehiclenumber, unsigned long vehicledirection, char turndirection,int action)
{	
	// ----------------------------
	char *char_vehicletype; 
	if(vehicletype == 0){
		char_vehicletype = "Car";
	}
	else if(vehicletype == 1){
		char_vehicletype = "Truck";
	}
	else{
		char_vehicletype = "ERROR";
	}
	
	// ----------------------------
	char *char_vehicledirection;
	if(vehicledirection == 0){
		char_vehicledirection = "Route A";
	}
	else if(vehicledirection == 1){
		char_vehicledirection = "Route B";
	}
	else if(vehicledirection == 2){
		char_vehicledirection = "Route C";
	}
	else{
		char_vehicledirection = "ERROR";
	}
	
	// ----------------------------
	char *char_turndirection;
	if(turndirection == 'L'){
		char_turndirection = "Left";
	}
	else if(turndirection == 'R'){
		char_turndirection = "Right";
	}
	else{
		char_turndirection = "ERROR";
	}
	
	// ----------------------------
	char *char_leavedirection;
	if(vehicledirection == 0 && turndirection == 'L'){		 // A -> C
		char_leavedirection = "Route C";
	}
	else if(vehicledirection == 0 && turndirection == 'R'){	 // A -> B
		char_leavedirection = "Route B";
	}
	else if(vehicledirection == 1 && turndirection == 'L'){	 // B -> A
		char_leavedirection = "Route A";
	}
	else if(vehicledirection == 1 && turndirection == 'R'){	 // B -> C
		char_leavedirection = "Route C";
	}
	else if(vehicledirection == 2 && turndirection == 'L'){	 // C -> B
		char_leavedirection = "Route B";
	}
	else if(vehicledirection == 2 && turndirection == 'R'){	 // C -> A
		char_leavedirection = "Route A";
	}
	else{
		char_leavedirection = "ERROR";
	}
	
	// ----------------------------
	char *char_action;
	if(action == 0){
		char_action = "APPROCHES";
	}
	else if(action == 1){
		char_action = "ENTERED";
	}
	else if(action == 2){
		char_action = "LEFT";
	}
	else{
		char_action = "ERROR";
	}
	
	// ----------------------------
	char *action_destination;
	if(action == 0){
		action_destination = "the intersection";
	}
	else if(vehicledirection == 0 && turndirection == 'L'){
		action_destination = "AB, BC";
	}
	else if(vehicledirection == 0 && turndirection == 'R'){
		action_destination = "AB";
	}
	else if(vehicledirection == 1 && turndirection == 'L'){
		action_destination = "BC, CA";
	}
	else if(vehicledirection == 1 && turndirection == 'R'){
		action_destination = "BC";
	}
	else if(vehicledirection == 2 && turndirection == 'L'){
		action_destination = "CA, AB";
	}
	else if(vehicledirection == 2 && turndirection == 'R'){
		action_destination = "CA";
	}
	else{
		action_destination = "ERROR";
	}
	
	// ----------------------------
	kprintf("\n%s %lu from %s to turn %s to %s %s %s\n\n", char_vehicletype, vehiclenumber, char_vehicledirection, char_turndirection, char_leavedirection, char_action, action_destination);
}


/*
 * createvehicles()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Notes:
 *      Driver code to start up the approachintersection() threads.  You are
 *      free to modiy this code as necessary for your solution.
 */

int
createvehicles(int nargs,
		char ** args)
{
	int index, error;
	//Create locks for the three intersection portions in the intersection 	
	portions[0] = lock_create("Portions 0 for AB");
	portions[1] = lock_create("Portions 1 for BC");
	portions[2] = lock_create("Portions 2 for CA");
	/*
	 * Avoid unused variable warnings.
	 */

	(void) nargs;
	(void) args;

	/*
	 * Start NVEHICLES approachintersection() threads.
	 */

	for (index = 0; index < NVEHICLES; index++) {

		error = thread_fork("approachintersection thread",
				NULL,
				index,
				approachintersection,
				NULL
				);

		/*
		 * panic() on error.
		 */

		if (error) {

			panic("approachintersection: thread_fork failed: %s\n",
					strerror(error)
				 );
		}
	}

	while(count<(NVEHICLES)); 	//Wait for the child thread to terminate
	kprintf("\nSuccess! %d vehicles passed the intersection.\n\n",(count));
	count=0;
	// Destroy all the created locks
	lock_destroy(portions[0]);
	lock_destroy(portions[1]);
	lock_destroy(portions[2]);
	return 0;
}
