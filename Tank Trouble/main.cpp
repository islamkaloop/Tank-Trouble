#include <math.h>
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

void drawCircle(double centerX, double centerY, double radiusX, double radiusY, int howMuch)
{
	const float DEG2RAD = 3.14159 / 180;
	glBegin(GL_POLYGON);
	for (int i = -1; i < howMuch; i++) {
		float degInRad = i * DEG2RAD;
		glVertex2d(centerX + cos(degInRad) * radiusX,
			centerY + sin(degInRad) * radiusY);

	}

	glEnd();
}

void displayPlayer(int PlayerN, int x, int y, int dx, int dy) {
	glLineWidth(100);
	glColor3f(0.4, 0.4, 0);
	glBegin(GL_LINES);
	for (int i = -(dy / 28);i < (dy / 28);i++) {
		glVertex2f(x + (dx / 2) + i, y + (dy * 2 / 7));
		glVertex2f(x + dx, y + dy - i);
	}
	glEnd();
	glLineWidth(1);
	glColor3f(0.2, 0.2039, 0);
	drawCircle(x + dx, y + dy, (dy / 14), (dy / 14), 360);
	glColor3f(0, 0.098, 0.2);
	drawCircle(x + (dx / 2), y + (dy * 23 / 28), (abs(dx) / 8), (abs(dx) / 8), 180);
	if (PlayerN == 0) {
		glColor3f(0, 0.2, 0);
	}
	else {
		glColor3f(0.145, 0.22745, 0.388);
	}
	glBegin(GL_TRIANGLE_FAN);
	{
		glVertex2f(x + (dx / 2), y + (dy * 2 / 7));
		glVertex2f(x, y + (dy * 2 / 7));
		glVertex2f(x + (dx / 4), y + (dy * 5 / 7));
		glVertex2f(x + (dx * 0.375), y + (dy * 5 / 7));
		glVertex2f(x + (dx * 0.375), y + (dy * 23 / 28));
		glVertex2f(x + (dx * 0.625), y + (dy * 23 / 28));
		glVertex2f(x + (dx * 0.625), y + (dy * 5 / 7));
		glVertex2f(x + (dx * 0.75), y + (dy * 5 / 7));
		glVertex2f(x + dx, y + (dy * 2 / 7));
	}
	glEnd();

	glColor3f(0, .2, 0.098);
	glBegin(GL_QUADS);
	{
		glVertex2f(x, y);
		glVertex2f(x, y + (dy * 2 / 7));
		glVertex2f(x + dx, y + (dy * 2 / 7));
		glVertex2f(x + dx, y);
	}
	glEnd();
	glColor3f(0, .2, 0.2);
	drawCircle(x + dx, y + (dy / 7), dy / 7, dy / 7, 360);
	drawCircle(x, y + (dy / 7), dy / 7, dy / 7, 360);

}

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

	displayPlayer(0, player1.x, player1.y, player1.dx, player1.dy);
	displayPlayer(1, player2.x, player2.y, -1 * player2.dx, player2.dy);

	glColor3f(0.13333, .545, 0.1333);
	glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(0, window_height * .05);
		glVertex2f(window_width, window_height * .05);
		glVertex2f(window_width, 0);
	glEnd();

	glFlush();
}