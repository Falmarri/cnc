/*
 * main.cpp
 *
 *  Created on: Dec 25, 2010
 *      Author: falmarri
 */



#include "WProgram.h"
#include <stdlib.h>
#include "cnccontrol.h"

int buffer[32];
int cur_read;
int commands[8];
int cur_command;


CncControl controller;

int count = 0;

unsigned long mil;

int main(void){

	init();

	setup();

	for (;;){
		loop();

	}
	Serial.println("Main loop returned...");



}


/**
 *
 * Debug commands:
 *
 * 0: home
 * ret: 1
 *
 * 1: move_x
 * 	n1: inches integer
 * 	n2: inches decimal
 * ret: current x, y
 *
 * 2: move_y
 * 	n1: inches integer
 * n2: inches decimal
 * ret: current x, y
 *
 *
 *
 *
 *
 *
 */

int state = LOW;

void loop(){



	if (millis() - mil > 3000){
		Serial.print("loop ");
		Serial.println(state);
		if (count < 4){


		Serial.println(controller.move_X(1.0));
		Serial.println("Moved pos x");
		//Serial.println(controller.move_Y(1.0));
		//Serial.println("Moved pos y");
		count++;

		}
		else{
			Serial.println(controller.move_X(-1.0));
			Serial.println("Moved neg x");
			//Serial.println(controller.move_Y(-1.0));
			//Serial.println("Moved neg y");
			count--;

		}
		digitalWrite(13, state = ~state);
		Serial.print("--------------\nCurrent State:\nx: ");
		Serial.println(controller.location_x());
		Serial.print("y: ");
		Serial.print(controller.location_y());
		Serial.println("\n--------------");
		delay(700);
		mil = millis();


	}





}




void setup(){

	Serial.begin(9600);
	controller.set_size(4.0, 4.0);
	Serial.print("Controller set. Motor Mask: ");
	Serial.println(controller.new_motors(3,4,5,6));
	pinMode(13, OUTPUT);
	digitalWrite(13, state);
	mil = millis();
	cur_read = 0;
	cur_command= -1;

}





extern "C" void __cxa_pure_virtual()
{
  cli();
  for (;;);
}

__extension__ typedef int __guard __attribute__((mode (__DI__)));

void * operator new(size_t size)
{
  return malloc(size);
}

void operator delete(void * ptr)
{
  free(ptr);
}

void * operator new[](size_t size)
{
    return malloc(size);
}

void operator delete[](void * ptr)
{
	if (ptr)
		free(ptr);
}

int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);}
void __cxa_guard_release (__guard *g) {*(char *)g = 1;}
void __cxa_guard_abort (__guard *) {}
