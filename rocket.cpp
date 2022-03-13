#include "rocket.h"
#include "2D_graphics.h"
#include <cmath>

rocket::rocket(double x, double y, double q, char* file_name) {

	//initialize all variables
	this->x = x;
	this->y = y;
	this->q = q;
	vx = 0;
	vy = 0;
	w = 0;
	scale = .5;

	//create sprite
	create_sprite(file_name, id_r);
}
rocket::~rocket() {}

void rocket::draw() {
	draw_sprite(id_r, x, y, q, scale);			//draw sprite
}

//kinematic equations for rocket simulation
void rocket::sim_step(double dt)
{
	x = x + vx * dt;
	y = y + vy * dt;
	q = q + w * dt;
}