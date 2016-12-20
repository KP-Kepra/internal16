#include "moving.h"

/*
 * Remember to initial IR at the main function
 * Moving from stage 1 to stage2
 * (Including picking up the basket)
 * Add ticks.c if necessary
 */

/*
 * For our shooter robot:
 * Pin_B6 is for #left# IR sensor
 * Pin_B7 is for #right# IR sensor
 * MOTOR1 is for #left# motor
 * MOTOR2 is for #right# motor
 * Dir 1 is forward
 * Dir 0 is backward
 * 0 is for white line
 * 1 is for black line
 * PNEUMATIC1 : grabber claw
 * PNEUMATIC2 : grabber lift
 */
 
int state = 1;
float distance = 0;
int turn_state3 = 0;
int turn_state5 = 0;
int check_wall = 0;
int side = 0; //0 for red, 1 for green
int flag_time = 0;
int auto_speed = 70;

void state_move() {
	switch(state) {
				case 1 : state1_move(); break;
				case 2 : state2_move(); break;
				case 3 : state3_move(); break;
				case 4 : state4_move(); break;
				case 5 : state5_move(); break;
				case 6 : state6_move(); break;
	}
}

/* State 1
	* Action : move forward
	* Stops : when ultrasonic detects basket at less than x cm
	*/
void state1_move(void) {
	if (distance >= 50 ) { //Basket distance is : 9.08~
		move_forward(auto_speed); //previous is 45
	}
	else {
		if (get_real_ticks()-flag_time >= 1000) {
			motor_stop();
			_delay_ms(500);
			state=2;
		}
	}
	
}

/* State 2
	* Action : Grab, lift basket
	* Stops : when ultrasonic doesn't detect basket at most x cm
	*/
void state2_move(void) {
	if (distance <= 50) {
		pneumatic_control(PNEUMATIC6, 1);
		_delay_ms(1000);
		pneumatic_control(PNEUMATIC3, 1);
		_delay_ms(1000);
		flag_time = get_real_ticks();
		state=3;
	}
}

/* State 3
	* Action : Turn right (red) / left (green), move forward
	* Stops : when ultrasonic detects wall at less than x cm
	*/
void state3_move(void) {	
	if (turn_state3 == 0) {
		turn_90degree((side == 0 ? 'R' : 'L'), 45);
		_delay_ms(200);
		turn_state3 = 1;
	}
	
	move_forward(auto_speed);
	if (distance <= 30 && get_real_ticks() - flag_time >= 1000) {
		motor_stop();
		_delay_ms(200);
		state=4; 
	}
}

/* State 4
	* Action : drops basket, move backwards, delift grabber
	* Stops : nothing
	*/
void state4_move(void) {
	pneumatic_control(PNEUMATIC6, 0);
	if (distance <= 30) { //Give small value, bcs of inertia
		move_backward(60);
	}
	else {
		motor_stop();
		_delay_ms(200);
		pneumatic_control(PNEUMATIC3, 0);
		state=5; 
	}
}

/* State 5
	* Action : turn right, move forward, when both IR sensors detect cross, turn left.
	* Stops : ultrasonic detects nothing
	*/
void state5_move(void) {
	if (check_wall == 0) {
		turn_90degree((side == 0 ? 'R' : 'L'), 30);
		move_forward(auto_speed);
		_delay_ms(400);
		turn_90degree((side == 0 ? 'L' : 'R'), 30);
		_delay_ms(200);
		motor_stop();
		check_wall = 1;
	}
	else if (check_wall == 1) {
		if (distance >= 50) {
			state = 6;
			motor_stop();
		}
		else {
			check_wall = 0;
		}
	}
}

/* State 6
	* Action : move forward, until manual mode
	* Stops : Hardcoding
	*/
void state6_move(void) {
	_delay_ms(8000);
	move_forward(auto_speed);
	_delay_ms(4000);
	motor_stop();
	state = 7;
}

/* State 7
	* Action : Manual
	* Stops : Nothing
	*/