
#include "rocket.h"				//creates instances of rocket
#include"platform.h"			//creates instance of platforms
#include "fstream"				//uses file IO

using namespace std;

class world {
	//file IO was used to export data for debugging purposes instead of printing to the screen
	ofstream fout;
	string logname;
public:
	double VX, VY, W;				//State variables
	int id_r, id_p;					//id for creating sprites
	int score_count;				//increments when rocket lands on platform
	int N;							//number of platforms/rockets
	double x_p, y_p;				//coordinates of platform in y and x
	rocket* rocket1[100];			//allocate memory for 100 rockers
	platform* platforms[100];		//allocate memory for 100 platforms
	double* xcord, * ycord;			//dynamic array used for storing coordinated of platforms

	int current_plat = 0;			//increments when new platform is created
	int current_rocket = 0;			//increments when new rocket is created

	//function prototypes
	world(int N, double VX, double VY, double W, char* robot_pic, char* platform_pic);		//constructor
	void draw();																			//draws rocket and platform
	void sim_step(double dt);																//moves forward in time
	void input();																			//takes keyboard input
	void gravity();																			//creates gravity, friction and drag forces
	int collision_map();																	//detects object collisions
	void score();																			//keeps track of when rocket lands on platform correctly
	~world();																				//destructor
	void remove_rocket(int i);																//frees memory when rocket is no longer needed
	void remove_plat(int i);																//frees memeory when platform is no longer needed
};