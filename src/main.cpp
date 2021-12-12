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
const int STEPS = 3000;
const int SPEED = 20;

const float MUTATION_RATE = 0.04;
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


class Obstacle{
	public:
		float x;
		float y;
		float w;
		float h;

		Obstacle() {}

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


class Target: public Obstacle{
	public:
		static constexpr int MAX_X = 700;
		static constexpr int MAX_Y = 300;
		static constexpr int MIN_X = 300;
		static constexpr int MIN_Y = -300;

		Target(){
			w = 50;
			h = 50;
			reset();
		}

		void render(){
			glColor3f(154/255.0, 230/255.0, 110/255.0);
			Paint::rect(x, y, w, h);
		}

		void reset(){
			x = MIN_X + (rand() % (int)(MAX_X - MIN_X));
			y = MIN_Y + (rand() % (int)(MAX_Y - MIN_Y));
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


class Gene{
	public:
		float reward;
		float movement_sequence[STEPS][2];    // movement sequence for each step

		Gene(){
			reset();
			initialize_movement_sequence();
		}

		void initialize_movement_sequence(){
			for(int i = 0; i < STEPS; i++) { 
				/* x, y coordinate movement */	
				movement_sequence[i][0] = (rand() % (SPEED+1)) - SPEED/2;
				movement_sequence[i][1] = (rand() % (SPEED+1)) - SPEED/2;
			}
		}

		/* --------------- */
		/* 	 	 Rewards     */    
		/* --------------- */

		void add_step_reward(){
			reward += 1;
		}
		
		void add_kill_reward(){
			reward -= 500;
		}

		void add_finish_reward(){
			reward += 3000;
		}

		void add_distance_to_target_reward(Target target){
			/* punish blobs with distance from target */
			float distance = sqrt(target.x*target.x + target.y*target.y); 
			reward -= distance * 2;
		}

		float* get_movement_step(int step){
			return movement_sequence[step];
		}

		float get_reward(){
			if (reward<0) return 0;
			return reward;
		}

		void reset(){
			reward = 0;
		}
};


class Blobs{
	public:
		static constexpr float RADIUS = 8;
		int step=0;    // step counter 

		float x;
		float y;
		bool is_dead=false;

		Gene gene;

		Blobs(){
			reset();
		}

		void render(){
			/* draw filled circle */
			glColor3f(151.0/255, 191.0/255, 180.0/255);
			Paint::circle(x, y, RADIUS);
		}

		void move(){
			/* change blob position with movement sequence on each step */
			if (!is_dead){
				x += gene.get_movement_step(step)[0];
				y += gene.get_movement_step(step)[1];
				gene.add_step_reward();
			}
			step += 1;
		}

		void reset(){
			/* reset blob position and step counter */
			x = -400;
			y = 0;
			step = 0;
			is_dead=false;
			gene.reset();
		}

		void kill(){
			/* scary method */
			if (!is_dead){
				gene.add_kill_reward();
				is_dead=true;
			}
		}

		void finish(){
			/* glorious method */
			if (!is_dead){
				gene.add_finish_reward();
				is_dead=true;
			}
		}

		void add_distance_reward(Target target){
			if(is_dead)
				gene.add_distance_to_target_reward(target);
		}
};


class Collision{
	std::vector <Obstacle> obstacles;
	Target target;

	public:
		bool static is_colliding(Obstacle obstacle, Blobs blobs){
				float x_obs = obstacle.x;
				float y_obs = obstacle.y;
				float w_obs_half = obstacle.w/2;
				float h_obs_half = obstacle.h/2;

				if(blobs.x > x_obs - w_obs_half && \
					 blobs.x < x_obs + w_obs_half && \
					 blobs.y > y_obs - h_obs_half && \
					 blobs.y < y_obs + h_obs_half ) 
					 return true;
				return false;
		}

		void add_obstacles(std::vector <Obstacle> new_obstacles){
			for(int i = 0; i < (int) new_obstacles.size(); i++) { 
				obstacles.push_back(new_obstacles[i]);
			}
		}

		void add_target(Target new_target){
			target = new_target;
		}

		/* check collision between blob & all obstacles */
		void collide(Blobs* blobs){
			if (is_colliding_with_target(*blobs))
				blobs->finish();
			if (is_colliding_with_obstacles(*blobs))
				blobs->kill();
		}

		bool is_colliding_with_target(Blobs blobs) {
			if (Collision::is_colliding(target, blobs))
				return true;
			return false;
		}

		bool is_colliding_with_obstacles(Blobs blobs) {
			for(int i = 0; i < (int) obstacles.size(); i++) { 
				if (Collision::is_colliding(obstacles[i], blobs))
					return true;
			}
			return false;
		}
};


class GeneticAlgorithm{
	public:
		static void generate_next_population(Blobs* population) {
			for(int i = 0; i < N_BLOB; i++) {
				GeneticAlgorithm::mutate(&population[i]);
			}
		}

		static void mutate(Blobs* blobs) {
			for(int i = 0; i < STEPS; i++) { 
				if(will_mutate()) {
				/* mutatex, y coordinate movement */	
					blobs->gene.movement_sequence[i][0] = (rand() % (SPEED+1)) - SPEED/2;
					blobs->gene.movement_sequence[i][1] = (rand() % (SPEED+1)) - SPEED/2;
				}
			}
		}

		static bool will_mutate(){
			float r = ((double) rand() / (RAND_MAX));
			if (r<MUTATION_RATE)
				return true;
			return false;
		}

		static float get_population_reward_total(Blobs* population) {
			float total = 0;
			for(int i = 0; i < N_BLOB; i++) {
				total += population->gene.get_reward();
			}
			return total;
		}

		static std::vector <Blobs> get_parent_pool(Blobs* population) {
			float population_reward = get_population_reward_total(population);
			std::vector <Blobs> parent_pool;

			/* Add blobs to parent pool by their reward score */
			for(int i = 0; i < N_BLOB; i++) {
				int n_selected = (int) N_BLOB * population[i].gene.get_reward() / population_reward;
				for(int j = 0; j < n_selected; j++)
					parent_pool.push_back(population[i]);
			}

			return parent_pool;
		}	
};


class Simulation{
	public:
		ObstacleFactory obstacleFactory;
		Target target;
		Blobs blobs [N_BLOB];
		int steps;    // step counter

		Collision collision = Collision();

		Simulation(){
			reset();
			obstacleFactory = ObstacleFactory();

			/* add all obstacles to collision manager */
			collision.add_obstacles(obstacleFactory.obstacles);
			collision.add_target(target);

			/* initialize all blobs */
			for(int i = 0; i < N_BLOB; i++) { 
				blobs[i] = Blobs();
			}
		}

		void simulate(){
			if (steps < STEPS){
				take_next_step();
			} else {    // reset simulation when maximum step was taken 
				reset();
			}
		}

		void take_next_step(){
			/* show obstacles & target */
			obstacleFactory.render();
			target.render();

			/* move and show all blobs */
			for(int i = 0; i < N_BLOB; i++) { 
				blobs[i].render();
				blobs[i].move();
				collision.collide(&blobs[i]);
			}
			steps += 1;
		}

		void reset(){
			/* reset all blobs and step counter */
			steps = 0;
			for(int i = 0; i < N_BLOB; i++) { 
				blobs[i].add_distance_reward(target);
			}
			GeneticAlgorithm::generate_next_population(blobs);
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
	/* * add crossover in genetic algorithm class */


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
