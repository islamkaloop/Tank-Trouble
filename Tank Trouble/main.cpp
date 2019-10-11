#include <math.h>
#include <glut.h>

#pragma comment(lib, "glut32.lib")

void Display(void);
void Anim(void);

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
double cloud1P = 0;
double cloud2P = window_width;
double cloud3P = window_width / 2;

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

void drawCloud(int x, int y)
{
	glColor3f(1, 1, 1);
	drawCircle(x, y, 80, 45, 360);
	drawCircle(x + 25, y + 15, 35, 35, 360);
	drawCircle(x + 25, y - 15, 35, 35, 360);
	drawCircle(x - 25, y - 15, 35, 35, 360);
	drawCircle(x - 25, y + 15, 35, 35, 360);
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

	drawCloud(cloud1P, window_height * .9);
	drawCloud(cloud2P, window_height * .8);
	drawCloud(cloud3P, window_height * .85);

	glPopMatrix();
}

void displayWall() {
	int x = window_width / 2;
	int y = window_height * 0.53;
	glColor3f(0.247, 0.306, 0.29);
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex2f(x - 20, window_height * .05);
	glVertex2f(x + 20, window_height * .05);
	glVertex2f(x + 20, y);
	glVertex2f(x - 20, y);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2f(x - 30, window_height * .05);
	glVertex2f(x + 30, window_height * .05);
	glVertex2f(x + 30, window_height * .1);
	glVertex2f(x - 30, window_height * .1);
	glEnd();
	glPopMatrix();
}

void displayHealth(int PlayerN, int x, int y, int width, int height) {
	double h = 0;
	width -= height + 5;

	if (PlayerN == 0) {
		h = (width * ((double)player1.health / 100));
		displayPlayer(0, x + width + 10, y + 2, height, height - 2);
	}
	else {
		h = -(width * ((double)player2.health / 100));
		displayPlayer(1, x - width - 10, y + 2, -height, height - 2);
		width = -width;
	}

	glColor3f(0.388, 0.329, 0.145);
	glBegin(GL_QUADS);
	{
		glVertex2f(x, y);
		glVertex2f(x, y + height);
		glVertex2f(x + width, y + height);
		glVertex2f(x + width, y);
	}
	glEnd();

	if (PlayerN == 0) {
		glColor3f(0, 0.2, 0);
	}
	else {
		glColor3f(0.145, 0.22745, 0.388);
	}

	glBegin(GL_QUADS);
	{
		glVertex2f(x, y);
		glVertex2f(x, y + height);
		glVertex2f(x + h, y + height);
		glVertex2f(x + h, y);
	}
	glEnd();

}

int main(int argc, char** argr)
{

	glutInit(&argc, argr);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000, window_height);
	glutInitWindowPosition(250, 50);
	glutCreateWindow("Tank Trouble");
	glutDisplayFunc(Display);
	glutIdleFunc(Anim);
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

	displayWall();

	displayHealth(0, 20, window_height * .93, window_width * .4, window_height * .052);
	displayHealth(1, window_width - 20, window_height * .93, window_width * .4, window_height * .052);

	glColor3f(0.13333, .545, 0.1333);
	glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(0, window_height * .05);
		glVertex2f(window_width, window_height * .05);
		glVertex2f(window_width, 0);
	glEnd();

	glFlush();
}

void Anim()
{
	cloud1P += 0.2;
	cloud2P -= 0.1;
	cloud3P += 0.3;

	if ((int)cloud1P == window_width + 100) {
		cloud1P = -100;
	}
	if ((int)cloud3P == window_width + 100) {
		cloud3P = -100;
	}
	if ((int)cloud2P == -100) {
		cloud2P = window_width + 100;
	}
	glutPostRedisplay();
}