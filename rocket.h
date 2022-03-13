class rocket {

public:

	double x, y; // robot position (pixels)
	double q; // robot angle (rad)
	double vx, vy; // robot forward velocity (input #1)
	double w; // robot angular velocity (input #2)
	int id_r; // id number of sprite used to draw the robot
	double scale; // size of the sprite

	rocket(double x, double y, double q, char* file_name);		//constructor
	~rocket();													//destructor
	void draw();												//draw the rocket sprite
	void sim_step(double dt);									//move forward in time
};