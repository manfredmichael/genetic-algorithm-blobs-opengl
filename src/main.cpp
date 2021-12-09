// C program to demonstrate
// drawing a circle using
// OpenGL
#include<stdio.h>
#include<GL/glut.h>
#include<stdlib.h> 
#include<math.h>
#define pi 3.142857

class Paint{
	public:
		static void circle(float x, float y, float radius){
			for (float i = 0; i < (2 * pi); i += 0.001)
			{
				// let 200 is radius of circle and as,
				// circle is defined as x=r*cos(i) and y=r*sin(i)
				glVertex2i(x + radius*cos(i),
									 y + radius*sin(i));
			}
			
	}
};

class Blobs{
	public:
		static constexpr float RADIUS=8;
		static constexpr int SPEED=4;

		float x;
		float y;

		Blobs(){
			x = 0;
			y = 0;
		}

		void render(){
			Paint::circle(x, y, RADIUS);
		}

		void move(){
			x += (rand() % (SPEED+1)) - SPEED/2;
			y += (rand() % (SPEED+1)) - SPEED/2;
	}
};



// function to initialize
void myInit (void)
{
	// making background color black as first
	// 3 arguments all are 0.0
	glClearColor(0.0, 0.0, 0.0, 1.0);
	
	// making picture color green (in RGB mode), as middle argument is 1.0
	glColor3f(0.0, 1.0, 0.0);
	
	// breadth of picture boundary is 1 pixel
	glPointSize(1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// setting window dimension in X- and Y- direction
	gluOrtho2D(-780, 780, -420, 420);
}

Blobs b;

void display (void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);

	/* TODO: */ 
	/* * render blobs */
	/* * move blobs with fixed movement sequence */
	b.render();
	b.move();

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

