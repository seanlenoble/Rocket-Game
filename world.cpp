#include "world.h"
#include "ran.h"
#include "2D_graphics.h"
#include <Windows.h>
#include "iMatrix.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

world::world(int N, double VX, double VY, double W, char* rocket_pic, char* platform_pic) {

	double x_r, y_r, q_r;								//rocket coordinates
	double x_p, y_p, q_p;								//platform coordinates
	double scale_r = 1.0, scale_p = 1.0;;				//sprite scales
	double width = 1500, height = 800;					//directX dimensions
	score_count = 0;									//score
	long int seed = -6;									//seed number for random

	this->VX = VX;										//initialize velocity in X, Y and the angle of rotation
	this->VY = VY;
	this->W = W;

	create_sprite(rocket_pic, id_r);					//create rocket sprite
	create_sprite(platform_pic, id_p);					//create platform sprite

	x_r = 50;											//coordinates of where rocket will first appear
	y_r = 50;
	q_r = 3.1415 / 2;									//rocket is facing upright position at pi/2

	xcord = new double[N];								//allocate memory to xcord and ycord
	ycord = new double[N];

	for (int num = 0; num < N; num++) {

		x_p = ran(seed) * 800 + 500;														//randomly generate coordinates for platforms
		y_p = ran(seed) * 400 + 50;
		q_p = 0;

		platforms[num] = new platform(x_p, y_p, q_p, platform_pic);							//create N platforms
		xcord[num] = x_p;																	//initialize coordinates of all platforms
		ycord[num] = y_p;

		rocket1[num] = new rocket(x_r, y_r, q_r, rocket_pic);								//create N rockets
	}


	rocket1[current_rocket]->vx = VY;														//initialize every rocket
	rocket1[current_rocket]->vy = VY;
	rocket1[current_rocket]->w = W;

	logname = "log.txt";						//log for debugging
	fout.open(logname);							//open file
}

//free memory
world::~world() {
	fout.close();
}
//draws current rocket and plaform
void world::draw() {
	rocket1[current_rocket]->draw();	
	platforms[current_plat]->draw();
}
//move forward in time
//also does some basic object collision to keep the rocket within the direct x window
void world::sim_step(double dt) {
	rocket1[current_rocket]->sim_step(dt);

	//keeps rocket within window boundaries 

	//prevents rocket from going too low
	if (rocket1[current_rocket]->y < 50) {
		rocket1[current_rocket]->y = 50;
		rocket1[current_rocket]->vy = 0;
		//slows rocket down when in contact with ground
		if (rocket1[current_rocket]->vx > 0) {
			rocket1[current_rocket]->vx -= 10;
		}
	}
	//prevents rocket from going too high
	if (rocket1[current_rocket]->y > 750) {
		rocket1[current_rocket]->y = 750;
		rocket1[current_rocket]->vy = 0;
	}
	//prevents rocket from going to far to the right
	if (rocket1[current_rocket]->x > 1450) {
		rocket1[current_rocket]->x = 1450;
		rocket1[current_rocket]->vx = 0;
	}
	//prevents rocket from going too far to the left
	if (rocket1[current_rocket]->x < 30) {
		rocket1[current_rocket]->x = 30;
		rocket1[current_rocket]->vx = 0;
	}
}

void world::input() {
	double dV, dW;						//change in velocity and angle

	if (KEY(VK_UP)) dV = 15;			//increace velocity when up key is pressed
	if (KEY(VK_LEFT)) dW = 2;			//rotate when left and right keys are pressed
	if (KEY(VK_RIGHT)) dW = -2;

	//increased velocity of current rocket in X and Y directions
	//rotates rocket
	rocket1[current_rocket]->vx += dV*cos(rocket1[current_rocket]->q);
	rocket1[current_rocket]->vy += dV*sin(rocket1[current_rocket]->q);
	rocket1[current_rocket]->w = dW;
}

void world::gravity() {
	//takes output from collision_map
	int contact = collision_map();

	//if rocket is not in contact with a platform, apply downwards velocity
	if (contact == 0) {																		//contact == 0 means rocket is not on the platform
		rocket1[current_rocket]->vy -= 7;													//reduce velocity in y direction

		fout << "\nrocket_x: " << rocket1[current_rocket]->x << " ";						//this was used for debugging. I had difficulty getting gravity to
		fout << "rocket_y: " << rocket1[current_rocket]->y << " ";							//always point downwards
		fout << "rocket_q: " << rocket1[current_rocket]->q << " ";
		fout << "rocket_vy: " << rocket1[current_rocket]->vy << " ";
		fout << "rocket_sinq: " << sin(rocket1[current_rocket]->q) << " ";
		

	}

	//friction
	//if it is in contact, reduct the horizontal velocity by a lot
	if (contact == 1) {																	//contact == 1 means rocket is on the platform
		if (rocket1[current_rocket]->vx > 0) {											//brings velocity in x direction to 0
			rocket1[current_rocket]->vx -= 5;
		}
		if (rocket1[current_rocket]->vx < 0) {
			rocket1[current_rocket]->vx += 5;											//brings velocity in x direction to 0
		}
	}
	//drag
	//while flying, reduce horizonal velocity 
	if (rocket1[current_rocket]->vx > 0) {
		rocket1[current_rocket]->vx -= 2;
	}
	if (rocket1[current_rocket]->vx < 0) {
		rocket1[current_rocket]->vx += 2;
	}

	
}

int world::collision_map() {

	const int N_map = 150, M_map = 80;
	
	//divide directX window into small sqaures 
	//values stored in integer matrix
	static iMatrix A(N_map, M_map);

	double x, xmin, xmax, dx;							//values to keep track of where you are in the matrix
	double y, ymin, ymax, dy;
	double is, js;
	int i, j, i_index, j_index;
	double width = 1500, height = 800;					//directX dimensions
	
	double dt = 1.0 / 60;

	xmin = 0;
	xmax = width - 1;

	ymin = 0;
	ymax = height - 1;

	dx = (xmax - xmin) / (N_map - 1);				//how much to increment matrix
	dy = (ymax - ymin) / (M_map - 1);


	for (i = 0; i < N_map; i++) {
		for (j = 0; j < M_map; j++) {
			x = xmin + i * dx;
			y = ymin + j * dy;

			//prevents rocket from flying through bottom of rocket
			//contact == 2 means there is contact with the platform but does not award a point
			if (x >= xcord[0] - 110 && x <= xcord[0] + 110 && y >= ycord[0] - 100 && y <= ycord[0] -25) {
				A.e(i, j) = 2;
			}
			//checks if rocket is on top of platform
			//awards a point if conact == 1
			else if (x >= xcord[0] - 110 && x <= xcord[0] + 110 && y >= ycord[0] && y <= ycord[0] + 83) {
				A.e(i, j) = 1;
			}
			//if contact == 0 there is no contact
			else {
				A.e(i, j) = 0;
			}
		}
	}

	int contact = 0;
	x = rocket1[current_rocket]->x;			//update coordinated
	y = rocket1[current_rocket]->y;

	is = (x - xmin) / dx;					//imarix increments
	js = (y - ymin) / dy;

	i_index = (int)(is + 0.5);				//rounds up
	j_index = (int)(js + 0.5);

	if (A.e(i_index, j_index) == 1) {
		contact = 1;
		rocket1[current_rocket]->vy = 0;		//when rocket is on top of platform, set vertical velocity to 0

		return contact;
	}
	if (A.e(i_index, j_index) == 2) {
		rocket1[current_rocket]->vy = 0;		//when rocket is under platform, set vertical velocity to 0 
		rocket1[current_rocket]->y -= 1;		//and move it down one pixel 

		contact = 0;
		return contact;
	}
	else {
		contact = 0;
		return contact;
	}
}

void world::score() {

	int collision = collision_map();

	//checks if rocket is upright, points will only be awarded if rocket lands upright																		
	if ((collision == 1 && rocket1[current_rocket]->q < 1.7 && rocket1[current_rocket]->q >1.4)) {		
		current_plat++;									//draw next platform
		current_rocket++;								//draw next rocket
		remove_rocket(current_rocket - 1);				//delete old rocket
		remove_plat(current_plat - 1);					//delete old platform
		xcord++;										//set coordinates for new platform
		ycord++;
	}
	
	//this displays the current score on the screen

	int points = current_rocket;		//number of points equal to current rocket
	stringstream strs;
	strs << points;

	//convert string to character array so it can be used in text()
	string temp_str = strs.str();
	char* char_type = (char*)temp_str.c_str();

	//create new character array that says "Score = x"
	char word[] = "Score = ";
	char A[8 * sizeof(char) + 2 * sizeof(int)];		//allow for double digit scores
	char* pc;
	int* pi;
	int i;
	pc = A;
	pi = (int*)pc;
	
	//put "score = " in char array
	for (i = 0; i < 8; i++) {
		pc[i] = word[i];
	}
	//put the actually score in char array
	pc[i] = *char_type;

	//print to directX window
	text(pc, 1070, 700, 2);
}

void world::remove_rocket(int i) {
	if (rocket1[i] == NULL) {
		cout << "\nerror in ~World";
		return;
	}
	else {
		delete rocket1[i];				//remove rocket from memory
	}
}

void world::remove_plat(int i) {
	if (platforms[i] == NULL) {
		cout << "\nerror in ~World";
		return;
	}
	else {
		delete platforms[i];				//remove platform from memory
	}	
}

