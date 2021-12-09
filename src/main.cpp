// C program to demonstrate
// drawing a circle using
// OpenGL
#include<stdio.h>
#include<GL/glut.h>
#include<math.h>
#define pi 3.142857

class Paint{
	public:
		static void circle(float x, float y, float radius){
			float i;
			for (i = 0; i < (2 * pi); i += 0.001)
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
		const float R = 25;
		float x_center;
		float y_center;

		Blobs(){
			x_center = 0;
			y_center = 0;
		}

		void render(){
			Paint::circle(x_center, y_center, R);
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

	/* TODO: render blobs */
	b.render();

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

