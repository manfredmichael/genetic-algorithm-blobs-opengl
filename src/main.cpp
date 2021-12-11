// C program to demonstrate
// drawing a circle using
// OpenGL
#include<stdio.h>
#include<GL/glut.h>
#include<stdlib.h> 
#include<math.h>
#include<vector>
#include<random>
#include<time.h>
#define pi 3.142857

/* Project Configurations */
const int N_BLOB = 100;
const int N_OBSTACLE = 25;
/* Project Configurations */


class Paint{
	public:
		static void circle(GLfloat x, GLfloat y, GLfloat radius){
			int i;
			int triangleAmount = 20; //# of triangles used to draw circle
			
			//GLfloat radius = 0.8f; //radius
			GLfloat twicePi = 2.0f * pi;
			
			glBegin(GL_TRIANGLE_FAN);
				glVertex2f(x, y); // center of circle
				for(i = 0; i <= triangleAmount;i++) { 
					glVertex2f(
							x + (radius * cos(i *  twicePi / triangleAmount)), 
							y + (radius * sin(i * twicePi / triangleAmount))
					);
				}
			glEnd();
		}

		static void point(float x, float y, float radius) {
			glBegin(GL_POINTS);
					glColor3f(0.3, 0.3, 0.3);
					glPointSize(radius);
					glVertex2i(x, y);
			glEnd();    
		}

		static void rect(float x, float y, float w, float h) {
			glBegin(GL_TRIANGLE_FAN);
				glVertex2f(x, y); 
				glVertex2f(x+w/2, y+h/2); 
				glVertex2f(x-w/2, y+h/2); 
				glVertex2f(x-w/2, y-h/2); 
				glVertex2f(x+w/2, y-h/2); 
				glVertex2f(x+w/2, y+h/2); 
			glEnd();

		}
};

class Blobs{
	public:
		static constexpr float RADIUS = 8;
		static constexpr int SPEED = 4;
		static constexpr int STEPS = 3000;

		float x;
		float y;
		float movement_sequence[STEPS][2];    // movement sequence for each step
		int step=0;    // step counter 

		Blobs(){
			reset();
			for(int i = 0; i < STEPS; i++) { 
				/* x, y coordinate movement */	
				movement_sequence[i][0] = (rand() % (SPEED+1)) - SPEED/2;
				movement_sequence[i][1] = (rand() % (SPEED+1)) - SPEED/2;
			}
		}

		void render(){
			/* draw filled circle */
			glColor3f(151.0/255, 191.0/255, 180.0/255);
			Paint::circle(x, y, RADIUS);
		}

		void move(){
			/* change blob position with movement sequence on each step */
			x += movement_sequence[step][0];
			y += movement_sequence[step][1];
			step += 1;
		}

		void reset(){
			/* reset blob positiona and step counter */
			x = -400;
			y = 0;
			step = 0;
	}
};

class Obstacle{
	public:
		float x;
		float y;
		float w;
		float h;

		Obstacle(float x_, float y_, float w_, float h_) {
			x = x_;
			y = y_;
			w = w_;
			h = h_;
		}

		void render(){
			glColor3f(221/255.0, 74/255.0, 72/255.0);
			Paint::rect(x, y, w, h);
		}
};


class ObstacleFactory{
	public:
		static constexpr int MAX_X = 700;
		static constexpr int MAX_Y = 400;
		static constexpr int MIN_X = -300;
		static constexpr int MIN_Y = -400;

		static constexpr int MAX_W = 100;
		static constexpr int MAX_H = 100;
		static constexpr int MIN_W = 20;
		static constexpr int MIN_H = 20;
		
		std::vector <Obstacle> obstacles;

		ObstacleFactory() {
			regenerate();
		}

		void regenerate(){
			/* set random seed */
			unsigned int time_ui = static_cast<unsigned int>( time(NULL) ); 
			srand( time_ui );

			/* generate N_OBSTACLE new obstacles */
			for(int i = 0; i < N_OBSTACLE; i++) { 
				generate();
			}
		}

		void generate(){
				float x = MIN_X + (rand() % (int)(MAX_X - MIN_X));
				float y = MIN_Y + (rand() % (int)(MAX_Y - MIN_Y));
				float w = MIN_W + (rand() % (int)(MAX_W - MIN_W));
				float h = MIN_H + (rand() % (int)(MAX_H - MIN_H));
				obstacles.push_back(Obstacle(x, y, w, h));
		}

		void render(){
			for(int i = 0; i < N_BLOB; i++) { 
				obstacles[i].render();
			}
		}

};

class Collision{
	std::vector <Obstacle> obstacles;

	public:
		Collision(std::vector <Obstacle> new_obstacles){
			add_obstacles(new_obstacles);
		}

		void add_obstacles(std::vector <Obstacle> new_obstacles){
			for(int i = 0; i < (int) new_obstacles.size(); i++) { 
				obstacles.push_back(new_obstacles[i]);
			}
		}
};

class Simulation{
	public:
		Blobs blobs [N_BLOB];
		ObstacleFactory obstacleFactory;
		int steps;    // step counter

		Simulation(){
			reset();
			obstacleFactory = ObstacleFactory();
			/* initialize all blobs */
			for(int i = 0; i < N_BLOB; i++) { 
				blobs[i] = Blobs();
			}
		}

		void simulate(){
			if (steps < Blobs::STEPS){
				take_next_step();
			} else {    // reset simulation when maximum step was taken 
				reset();
			}
		}

		void take_next_step(){
			/* show obstacles */
			obstacleFactory.render();
			/* move and show blobs */
			for(int i = 0; i < N_BLOB; i++) { 
				blobs[i].render();
				blobs[i].move();
			}
			steps += 1;
		}

		void reset(){
			/* reset all blobs and step counter */
			steps = 0;
			for(int i = 0; i < N_BLOB; i++) { 
				blobs[i].reset();
			}
		}
};

Simulation simulation = Simulation();

// function to initialize
void myInit (void)
{

	// making background color black as first
	// 3 arguments all are 0.0
	glClearColor(245/255.0, 238/255.0, 220/255.0, 1.0);
	
	
	// breadth of picture boundary is 1 pixel
	/* glMatrixMode(GL_PROJECTION); */
	/* glLoadIdentity(); */
	
	// setting window dimension in X- and Y- direction
	gluOrtho2D(-780, 780, -420, 420);
	glutSwapBuffers();
}


void display (void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	/* TODO: */ 
	/* * add walls */
	/* * add collisions */
	/* * add blob death on collision */
	/* * add finish zone, with color: glColor3f(79/255.0, 9/255.0, 29/255.0); */


	simulation.simulate();

	glEnd();
	glFlush();
}

int main (int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glDisable(GL_DEPTH_TEST);
	
	// giving window size in X- and Y- direction
	glutInitWindowSize(1366, 768);
	glutInitWindowPosition(0, 0);
	
	// Giving name to window
	glutCreateWindow("Circle Drawing");
	myInit();
	
	glutIdleFunc(display);
	glutDisplayFunc(display);
	glutMainLoop();
}
