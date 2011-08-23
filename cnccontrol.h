/*
 * cnccontrol.h
 *
 *  Created on: Dec 26, 2010
 *      Author: falmarri
 */

#ifndef CNCCONTROL_H_
#define CNCCONTROL_H_

#ifndef TESTING
#include "wiring.h"
#endif



class Motor{
public:

	/**
	 *
	 * Motor controller with 4 pins. Pins must be on the same PORT (0-7, 8-11)
	 *
	 */

	/**
	 *
	 * Motor controller with 2 pins. Pins must be on the same PORT (0-7, 8-11)
	 *
	 */


	Motor();

	int set_pins(int pin1, int pin2);

	int set_pins(int pin1, int pin2, int pin3, int pin4);

	int step(int dir);

	void set_port();

	void step_many(int steps, bool dir);

	void step_return();

	int steps();


private:

	int current_step;

	byte seq[4];

	byte mask;

	byte port;

	int n_pins;


};



class CncControl{

public:

	CncControl();

	void set_size(double x_size, double y_size);

	int new_motors(int pin1x, int pin2x, int pin1y, int pin2y);

	void set_steps(int steps_x, int steps_y);

	const double& move_X(double inches, int cut = -1);

	const double& move_Y(double inches, int cut = -1);

	void move_XY(double inchesx, double inchesy, int cut = -1);

	void move_Xsteps(int steps, int cut = -1);

	void move_Ysteps(int steps, int cut = -1);

	void move_to(double xpos, double ypos);

	void arc_cw(double startx, double starty, double endx, double endy, double radius);

	void arc_ccw(double startx, double starty, double endx, double endy, double radius);

	void arc(double startx, double starty, double endx, double endy, double radx, double raxy, int direction);

	void arc(double startx, double starty, double endx, double endy, double radius, int direction);

	void arc(double endx, double endy, double radius);

	const double& location_x() { return this->current_x;};

	const double& location_y() { return this->current_y;};

	void home();

private:

	bool check(double x, double y, double xf, double yf){

		return (((x>0 && xf > 0) || (x<0 && xf < 0)) && (((y>0 && yf > 0) || (y < 0 && yf < 0))) && (((abs(x) > abs(y)) && (abs(xf) > abs(yf))) || ((abs(x) < abs(y)) && (abs(xf) < abs(yf)))));
	}

	double x_size;
	double y_size;
	double current_x;
	double current_y;
	bool cutting;
	int X_STEPS_PER_INCH;
	int Y_STEPS_PER_INCH;

	Motor motor_x;
	Motor motor_y;




};







#endif /* CNCCONTROL_H_ */
