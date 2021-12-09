// C program to demonstrate
// drawing a circle using
// OpenGL
#include<stdio.h>
#include<GL/glut.h>
#include<stdlib.h> 
#include<math.h>
#define pi 3.142857

/* Project Configurations */
const int N_BLOB = 100;
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

		static void point(float x, float y, float radius){
			glBegin(GL_POINTS);
					glColor3f(0.3, 0.3, 0.3);
					glPointSize(radius);
					glVertex2i(x, y);
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
		float movement_sequence[STEPS][2];
		int step=0;

		Blobs(){
			reset();
			for(int i = 0; i < STEPS; i++) { 
				/* x, y coordinate movement */	
				movement_sequence[i][0] = (rand() % (SPEED+1)) - SPEED/2;
				movement_sequence[i][1] = (rand() % (SPEED+1)) - SPEED/2;
			}
		}

		void render(){
			glColor3f(151.0/255, 191.0/255, 180.0/255);
			Paint::circle(x, y, RADIUS);
		}

		void move(){
			x += movement_sequence[step][0];
			y += movement_sequence[step][1];
			step += 1;
		}

		void reset(){
			x = -400;
			y = 0;
			step = 0;
	}
};



Blobs blobs [N_BLOB];
// function to initialize
void myInit (void)
{
	for(int i = 0; i < N_BLOB; i++) { 
		/* x, y coordinate movement */	
		blobs[i] = Blobs();
	}

	// making background color black as first
	// 3 arguments all are 0.0
	glClearColor(1, 1, 1, 1.0);
	
	
	// breadth of picture boundary is 1 pixel
	glPointSize(1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// setting window dimension in X- and Y- direction
	gluOrtho2D(-780, 780, -420, 420);


}


void display (void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	/* TODO: */ 
	/* * render blobs */
	/* * move blobs with fixed movement sequence */
	for(int i = 0; i < N_BLOB; i++) { 
		/* x, y coordinate movement */	
		blobs[i].render();
		blobs[i].move();
	}

	glEnd();
	glFlush();
}

int main (int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	
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

