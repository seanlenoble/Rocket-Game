class platform {

public:

	double x, y, q;					// robot position (pixels)
	int id_p;						// id number of sprite used to draw the robot
	double scale = 1;				// size of the sprite

	platform(double x, double y, double q, char* file_name);	
	~platform();
	void draw();
};
