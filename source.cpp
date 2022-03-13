
#include "2D_graphics.h"	//used for drawing graphics
#include "world.h"			//instance of world being declared

using namespace std;

int main() {

	//Initial values for simulation
	double VX = 0.0;				//velocity in x direction
	double VY = 0.0;				//velocity in y direction
	double W = 0.0;					//angle
	double dt = 1.0 / 60;			//time change
	int N = 5;						//number of platforms/rockets being generated

	initialize_graphics();		
	
	world world1(N, VX, VY, W, "rocket2.png", "platform.png");		//create world with initial values

	//being simulation
	while (1) {
		clear();
		world1.input();						//takes input from keyboard
		world1.gravity();					//causes rocket to fall
		world1.sim_step(dt);				//moves time forward
		world1.collision_map();				//checks if there are any collision
		world1.score();						//checks if rocket has landed on platform 
		world1.draw();						//draws sprites
		update();
	}
}

