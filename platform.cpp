#include "platform.h"
#include "2D_graphics.h"

platform::platform(double x, double y, double q, char* file_name) {

	//initialize variables
	this->x = x;
	this->y = y;
	this->q = q;

	//create sprite
	create_sprite(file_name, id_p);
}

void platform::draw() {
	draw_sprite(id_p, x, y, q, scale);		//draw sprite
}

platform::~platform() {}