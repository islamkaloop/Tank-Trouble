#include <glut.h>

#pragma comment(lib, "glut32.lib")

void Display(void);

class Player
{
public:
	int number;
	int health;
	double sprint;
	int x;
	int y;
	int dx;
	int dy;
	Player(int number, int x, int y, int dx, int dy) {
		this->number = number;
		this->x = x;
		this->y = y;
		this->dx = dx;
		this->dy = dy;
		this->health = 100;
		this->sprint = 0;
	}
};

int window_height = 600;
int window_width = 1000;

Player player1(0, 50, window_height * .05, 80, 80);
Player player2(1, window_width - 50, window_height * .05, 80, 80);

void drawBackGround() {
	glPushMatrix();

	glColor3f(0.529, 0.8078, 0.9216);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(0, window_height);
	glVertex2f(window_width, window_height);
	glVertex2f(window_width, 0);
	glEnd();

	glPopMatrix();
}

int main(int argc, char** argr)
{

	glutInit(&argc, argr);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000, window_height);
	glutInitWindowPosition(250, 50);
	glutCreateWindow("Tank Trouble");
	glutDisplayFunc(Display);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	gluOrtho2D(0.0, window_width, 0.0, window_height);

	glutMainLoop();
	return 0;
}


void Display(void)
{
	drawBackGround();
	glFlush();
}