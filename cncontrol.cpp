/*
 * cncontrol.cpp
 *
 *  Created on: Dec 27, 2010
 *      Author: falmarri
 */

#include "cnccontrol.h"

int FAIL = -1;

/*
CncControl::CncControl(double x_sz, double y_sz, Motor* motorx, Motor* motory) :
	x_size(x_sz), y_size(y_sz) {

	this->X_STEPS_PER_INCH = 100;
	this->Y_STEPS_PER_INCH = 100;
	this->current_x = 0;
	this->current_y = 0;
	this->cutting = false;

	if (motorx) {

		this->motor_x = motorx;

	} else {
		this->motor_x = new Motor(2, 3);
	}
	if (motory) {

		this->motor_y = motory;
	}else{
		this->motor_y = new Motor(4, 5);
	}

}
*/

CncControl::CncControl(){

	this->X_STEPS_PER_INCH = 100;
	this->Y_STEPS_PER_INCH = 100;
	this->current_x = 0;
	this->current_y = 0;
	this->cutting = false;

}

void CncControl::set_steps(int steps_x, int steps_y){

	if (steps_x > 0){
		this->X_STEPS_PER_INCH = steps_x;
	}
	if (steps_y > 0){
		this->Y_STEPS_PER_INCH = steps_y;
	}

}

void CncControl::set_size(double x_size, double y_size){

	this->x_size = x_size;
	this->y_size = y_size;

}

int CncControl::new_motors(int pin1x, int pin2x, int pin1y, int pin2y){

	return this->motor_x.set_pins(pin1x, pin2x) | this->motor_y.set_pins(pin1y, pin2y);


}

const double& CncControl::move_X(double inches, int cut) {

	for (int total = this->Y_STEPS_PER_INCH * abs(inches); total; --total) {

		if ((!this->current_x && inches < 0) || (this->current_x
				>= this->x_size && inches > 0)) {
			//Serial.println("Motor x at limit");
			return FAIL;
		}
		//Serial.println("X step");
		this->motor_x.step((inches > 0 ? 1.0 : -1.0));
		this->current_x += (inches > 0 ? 1.0 : -1.0) / this->X_STEPS_PER_INCH;
		delay(10);
	}

	return this->current_x;

}

void CncControl::home(){

	while (current_x > 0 && current_y > 0){

		this->motor_x.step(-1);
		this->motor_y.step(-1);
		delay(10);

	}

}

const double& CncControl::move_Y(double inches, int cut) {

	for (int total = this->Y_STEPS_PER_INCH * abs(inches); total; --total) {

		if ((!this->current_y && inches < 0) || (this->current_y
				>= this->y_size && inches > 0)) {
			//Serial.println("Motor y at limit");
			return FAIL;
		}
		//Serial.println("Y");

		this->motor_y.step((inches > 0 ? 1.0 : -1.0));
		this->current_y += (inches > 0 ? 1.0 : -1.0) / this->Y_STEPS_PER_INCH;
		delay(10);
	}
	return this->current_y;

}

void CncControl::arc(double endx, double endy, double radius) {

}

void CncControl::arc(double startx, double starty, double endx, double endy,
		double radx, double rady, int dir) {

	int sx = (startx - radx) * (this->X_STEPS_PER_INCH);
	int sy = (starty - rady) * (this->Y_STEPS_PER_INCH);
	int cx = sx;
	int cy = sy;
	double r2 = radx * radx + rady * rady;
	int ex = (endx - radx) * (this->X_STEPS_PER_INCH);
	int ey = (endy - rady) * (this->Y_STEPS_PER_INCH);

	int incx = 1;
	int incy = 1;

	while (!((((cx > 0 && ex > 0) || (cx < 0 && ex < 0))
			&& (((cy > 0 && ey > 0) || (cy < 0 && ey < 0))) && (((abs(cx)
			> abs(cy)) && (abs(ex) > abs(ey))) || ((abs(cx) < abs(cy))
			&& (abs(ex) < abs(ey))))) && ((abs(cx) > (abs(cy))) ? cx == ex : cy == ey))) {

		if (cy > 0) {
			incx = -1;
		} else {
			incx = 1;
		}

		if (cx > 0) {
			incy = 1;
		} else {
			incy = -1;
		}

		if (abs(cx) > abs(cy)) {

		}

		if (abs(cx) < abs(cy)) {

		}

	}

}


void CncControl::arc(double startx, double starty, double endx, double endy,
		double radius, int direction) {

	double rx;
	double ry;

	int sx = startx * (this->X_STEPS_PER_INCH % 10 + 1);
	int yx = startx * (this->Y_STEPS_PER_INCH % 10 + 1);

}

Motor::Motor(){
	this->current_step=0;

}



int Motor::set_pins(int pin1, int pin2){

	this->n_pins = 2;

	if (pin1 < 8 && pin2 < 8) {

		port = 'd';
	} else if (pin1 > 7 && pin1 < 13 && pin2 > 7 && pin2 < 13) {

		port = 'b';

	}


	mask = (1 << pin1 | 1 << pin2);

	/**
	 * 01
	 * 11
	 * 10
	 * 00

	 */

	seq[0] = 1 << pin2;

	seq[1] = 1 << pin1 | 1 << pin2;

	seq[2] = 1 << pin1;

	seq[3] = 0;

	pinMode(pin1, OUTPUT);
	pinMode(pin2, OUTPUT);

	this->set_port();

	return mask;


}




/**
 * @param dir: Direction of motor. True = CW, False = CCW
 */
int Motor::step(int dir) {

	if (dir < 0 && this->current_step <= 0) {
		this->current_step = 0;
		return -1;
	}

	if (dir > 0) {

		++(this->current_step);
	} else {

		--(this->current_step);
	}

	this->set_port();



	return this->current_step;
}

void Motor::set_port(){
	if (port == 'd') {

		byte p = PORTD;
		p &= ~this->mask;
		PORTD = p | seq[this->current_step % 4];

	} else if (port == 'b') {

		byte p = PORTB;
		p &= ~this->mask;
		PORTB = p | seq[this->current_step % 4];

	}


}

int Motor::steps() {
	return this->current_step;
}
