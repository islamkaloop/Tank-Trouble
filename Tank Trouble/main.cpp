#include <iostream>
#include <math.h>
#include <IRR/irrKlang.h>
#include <glut.h>

#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "irrKlang.lib")

using namespace std;

void Display(void);
void Anim(void);
void mou(int b, int s, int x, int y);

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

irrklang::ISoundEngine* SoundEngine = NULL;
irrklang::ISound* m_pBackground = NULL;
int window_height = 600;
int window_width = 1000;
double cloud1P = 0;
double cloud2P = window_width;
double cloud3P = window_width / 2;
int mouseDown = false;
int shot = false;
int turn = 0;
int defendDirection = true;
double defendMotion = 0;
double shootSpeed = 0;
int movePlayer1 = 0;
int movePlayer2 = 0;
int moveplayerdir = 0;
int colide = 0;

Player player1(0, 50, window_height * .05, 80, 80);
Player player2(1, window_width - 50, window_height * .05, 80, 80);

void print(int x, int y, char* string)
{
	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

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

int* bezier(float t, int* p0, int* p1, int* p2, int* p3)
{
	int res[2];
	res[0] = pow((1 - t), 3) * p0[0] + 3 * t * pow((1 - t), 2) * p1[0] + 3 * pow(t, 2) * (1 - t) * p2[0] + pow(t, 3) * p3[0];
	res[1] = pow((1 - t), 3) * p0[1] + 3 * t * pow((1 - t), 2) * p1[1] + 3 * pow(t, 2) * (1 - t) * p2[1] + pow(t, 3) * p3[1];
	return res;
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

void displaySprint(void) {
	glColor3f(1, 1, 1);

	int x = window_height * .05;

	glBegin(GL_LINE_LOOP);
	{
		glVertex2f(5, x);
		glVertex2f(5, 100 + x);
		glVertex2f(15, 100 + x);
		glVertex2f(15, x);
	}
	glEnd();
	glBegin(GL_LINE_LOOP);
	{
		glVertex2f(window_width - 5, x);
		glVertex2f(window_width - 5, 100 + x);
		glVertex2f(window_width - 15, 100 + x);
		glVertex2f(window_width - 15, x);
	}
	glEnd();
	glColor3f(0, 1, 0);

	glBegin(GL_QUADS);
	{
		glVertex2f(5, x);
		glVertex2f(5, x + player1.sprint);
		glVertex2f(15, x + player1.sprint);
		glVertex2f(15, x);
	}
	glEnd();
	glBegin(GL_QUADS);
	{
		glVertex2f(window_width - 5, x);
		glVertex2f(window_width - 5, x + player2.sprint);
		glVertex2f(window_width - 15, x + player2.sprint);
		glVertex2f(window_width - 15, x);
	}
	glEnd();
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

void displyBezier() {
	glPointSize(1);
	glColor3f(1, 1, 1);
	glBegin(GL_POINTS);

	int p0[2];
	int p1[2];
	int p2[2];
	int p3[2];

	int x1 = player1.x + player1.dx;
	int y1 = player1.y + player1.dy;
	int x2 = player2.x - player2.dx + 5;
	int y2 = player2.y + player2.dy;

	int rangeThrow = window_height * 0.3;

	double changeX = 0;
	double changeY = 0;
	for (float t = 0; t < 1; t += 0.001)
	{
		int* p = 0;
		if (turn == 0) {
			x2 = player2.x;
			y2 = player2.y;

			if (shot) {
				if (player1.sprint < 50) {
					changeX = -1 * (rangeThrow * (50 - player1.sprint) / 50);
					changeY = (rangeThrow * (50 - player1.sprint) / 50);
				}
				else {
					changeX = 2 * (rangeThrow * (player1.sprint - 50) / 50);
					changeY = -0.5 * (rangeThrow * (player1.sprint - 50) / 50);
				}
			}

			p0[0] = x1;
			p0[1] = y1;

			p1[0] = window_width / 2;
			p1[1] = window_height * 0.7 + changeY;

			p2[0] = window_width / 2;
			p2[1] = window_height * 0.7 + changeY;

			p3[0] = x2 + changeX;
			p3[1] = 0;

			p = bezier(t, p0, p1, p2, p3);
		}
		else {
			x1 = player1.x;
			y1 = player1.y;

			if (shot) {
				if (player2.sprint < 50) {
					changeX = (rangeThrow * (50 - player2.sprint) / 50);
					changeY = (rangeThrow * (50 - player2.sprint) / 50);
				}
				else {
					changeX = -2 * (rangeThrow * (player2.sprint - 50) / 50);
					changeY = -0.5 * (rangeThrow * (player2.sprint - 50) / 50);
				}
			}

			p0[0] = x1 + changeX;
			p0[1] = 0;

			p1[0] = window_width / 2;
			p1[1] = window_height * 0.7 + changeY;

			p2[0] = window_width / 2;
			p2[1] = window_height * 0.7 + changeY;

			p3[0] = x2;
			p3[1] = y2;

			p = bezier(t, p0, p1, p2, p3);
		}

		glVertex3f(p[0], p[1], 0);
	}
	glEnd();
}

void drowProjectile(double x) {

	int p0[2];
	int p1[2];
	int p2[2];
	int p3[2];

	int x1 = player1.x + player1.dx;
	int y1 = player1.y + player1.dy;
	int x2 = player2.x - player2.dx + 5;
	int y2 = player2.y + player2.dy;

	int rangeThrow = window_height * 0.3;

	int* p = 0;

	if (turn == 0) {
		x2 = player2.x;
		y2 = player2.y;
		double changeX = 0;
		double changeY = 0;
		if (shot) {
			if (player1.sprint < 50) {
				changeX = -1 * (rangeThrow * (50 - player1.sprint) / 50);
				changeY = (rangeThrow * (50 - player1.sprint) / 50);
			}
			else {
				changeX = 2 * (rangeThrow * (player1.sprint - 50) / 50);
				changeY = -0.5 * (rangeThrow * (player1.sprint - 50) / 50);
			}
		}
		p0[0] = x1;
		p0[1] = y1;

		p1[0] = window_width / 2;
		p1[1] = window_height * 0.7 + changeY;

		p2[0] = window_width / 2;
		p2[1] = window_height * 0.7 + changeY;

		p3[0] = x2 + changeX;
		p3[1] = 0;

		p = bezier(x, p0, p1, p2, p3);
		if (p[0] > player2.x - player2.dx && p[0] < player2.x && p[1] > player2.y && p[1] < player2.y + player2.dy) {
			shot = false;
			shootSpeed = 0;
			player1.sprint = 0;
			player2.sprint = 0;
			if (turn == 0) {
				turn = 1;
			}
			else {
				turn = 0;
			}
			player2.health -= 10;
			colide = 2;
		}
	}
	else {
		x1 = player1.x;
		y1 = player1.y;
		double changeX = 0;
		double changeY = 0;
		if (shot) {
			if (player2.sprint < 50) {
				changeX = (rangeThrow * (50 - player2.sprint) / 50);
				changeY = (rangeThrow * (50 - player2.sprint) / 50);
			}
			else {
				changeX = -2 * (rangeThrow * (player2.sprint - 50) / 50);
				changeY = -0.5 * (rangeThrow * (player2.sprint - 50) / 50);
			}
		}
		p0[0] = x1 + changeX;
		p0[1] = 0;

		p1[0] = window_width / 2;
		p1[1] = window_height * 0.7 + changeY;

		p2[0] = window_width / 2;
		p2[1] = window_height * 0.7 + changeY;

		p3[0] = x2;
		p3[1] = y2;

		x = 1 - x;
		p = bezier(x, p0, p1, p2, p3);
		if (p[0] > player1.x && p[0] < player1.x + player1.dx && p[1] > player1.y && p[1] < player1.y + player1.dy) {
			shot = false;
			shootSpeed = 0;
			player1.sprint = 0;
			player2.sprint = 0;
			if (turn == 0) {
				turn = 1;
			}
			else {
				turn = 0;
			}
			player1.health -= 10;
			colide = 1;
		}
	}
	int d = p[0];
	int f = p[1];
	int c = window_width / 2 + defendMotion;
	int y = window_height * 0.65;
	int c1 = window_width / 2;
	int c3 = window_height * 0.53;
	
	if ((d > c - 100 && d < c + 100 && f > y - 25 && f < y + 25) || (d > c1 - 20 && d < c1 + 20 && f > 0 && f < c3)) {
		shot = false;
		shootSpeed = 0;
		player1.sprint = 0;
		player2.sprint = 0;
		if (turn == 0) {
			turn = 1;
		}
		else {
			turn = 0;
		}
	}

	glPointSize(10);
	glColor3f(0, 0, 0);
	drawCircle(d, f, 4, 4, 360);
	if (turn == 0) {
		glBegin(GL_QUADS);
		{
			glVertex2f(d - 4, f - 4);
			glVertex2f(d - 4, f + 4);
			glVertex2f(d, f + 4);
			glVertex2f(d, f - 4);
		}
		glEnd();
	}
	else {
		glBegin(GL_QUADS);
		{
			glVertex2f(d, f - 4);
			glVertex2f(d, f + 4);
			glVertex2f(d + 4, f + 4);
			glVertex2f(d + 4, f - 4);
		}
		glEnd();
	}
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

void drawDefender() {
	int x = window_width / 2 + defendMotion;
	int y = window_height * 0.65;
	glColor3f(0.4745, 0.412, 0.396);
	glBegin(GL_POLYGON);
	{

		glVertex2f(x - 100, y + 20);
		glVertex2f(x - 95, y + 25);
		glVertex2f(x + 95, y + 25);
		glVertex2f(x + 100, y + 20);
		glVertex2f(x + 100, y - 20);
		glVertex2f(x + 95, y - 25);
		glVertex2f(x - 95, y - 25);
		glVertex2f(x - 100, y - 20);
	}
	glEnd();
	glColor3f(0.6588, 0.8, 0.843);
	glBegin(GL_QUADS);
	glVertex2f(x - 10, y - 15);
	glVertex2f(x + 10, y - 15);
	glVertex2f(x + 10, y + 15);
	glVertex2f(x - 10, y + 15);
	glEnd();
	glBegin(GL_QUADS);
	glVertex2f(x - 35, y - 15);
	glVertex2f(x - 15, y - 15);
	glVertex2f(x - 15, y + 15);
	glVertex2f(x - 35, y + 15);
	glEnd();
	glBegin(GL_QUADS);
	glVertex2f(x + 35, y - 15);
	glVertex2f(x + 15, y - 15);
	glVertex2f(x + 15, y + 15);
	glVertex2f(x + 35, y + 15);
	glEnd();
}

int main(int argc, char** argr)
{

	SoundEngine = irrklang::createIrrKlangDevice();
	if (!SoundEngine)
		return 0;
	m_pBackground = SoundEngine->play2D("src/sound/hitman.wav", true, false, true);

	glutInit(&argc, argr);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000, window_height);
	glutInitWindowPosition(250, 50);
	glutCreateWindow("Tank Trouble");
	glutDisplayFunc(Display);
	glutMouseFunc(mou);
	glutIdleFunc(Anim);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	gluOrtho2D(0.0, window_width, 0.0, window_height);

	glutMainLoop();
	return 0;
}

void Display(void)
{
	drawBackGround();

	displyBezier();
	if (shot) {
		drowProjectile(shootSpeed);
	}

	displayPlayer(0, player1.x + movePlayer1, player1.y, player1.dx, player1.dy);
	displayPlayer(1, player2.x - movePlayer2, player2.y, -1 * player2.dx, player2.dy);

	displayWall();

	displaySprint();
	displayHealth(0, 20, window_height * .93, window_width * .4, window_height * .052);
	displayHealth(1, window_width - 20, window_height * .93, window_width * .4, window_height * .052);
	drawDefender();

	if (player1.health == 0) {
		glColor3f(0.13333, .545, 0.1333);
		glBegin(GL_QUADS);
		glVertex2f(window_width * .35, window_height * .4);
		glVertex2f(window_width * .35, window_height * .6);
		glVertex2f(window_width * .65, window_height * .6);
		glVertex2f(window_width * .65, window_height * .4);
		glEnd();
		glColor3f(1, 0, 0);
		print(window_width * .42, window_height * .5, (char*)"Blue Player win");
	}
	else if (player2.health == 0) {
		glColor3f(0.13333, .545, 0.1333);
		glBegin(GL_QUADS);
		glVertex2f(window_width * .35, window_height * .4);
		glVertex2f(window_width * .35, window_height * .6);
		glVertex2f(window_width * .65, window_height * .6);
		glVertex2f(window_width * .65, window_height * .4);
		glEnd();	
		glColor3f(1, 0, 0);
		print(window_width * .42, window_height * .5, (char*)"Green Player win");
	}

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

	if (colide == 1) {
		if (movePlayer1 < 20 && moveplayerdir == 0) {
			movePlayer1 += 1;
		}
		else {
			moveplayerdir = 1;
			movePlayer1 -= 1;
			if (movePlayer1 == 0) {
				colide = 0;
				moveplayerdir = 0;
			}
		}
	}
	if (colide == 2) {
		if (movePlayer2 < 20 && moveplayerdir == 0) {
			movePlayer2 += 1;
		}
		else {
			moveplayerdir = 1;
			movePlayer2 -= 1;
			if (movePlayer2 == 0) {
				colide = 0;
				moveplayerdir = 0;
			}
		}
	}

	if (defendDirection) {
		if ((int)defendMotion == window_width * .3) {
			defendDirection = false;
			defendMotion -= 0.2;
		}
		else {
			defendMotion += 0.2;
		}
	}
	else {
		if ((int)defendMotion == -window_width * .3) {
			defendDirection = true;
			defendMotion += 0.2;
		}
		else {
			defendMotion -= 0.2;
		}
	}

	if (mouseDown) {
		if (turn == 0) {
			if (player1.sprint < 100)
				player1.sprint = player1.sprint + 0.3;
		}
		else {
			if (player2.sprint < 100)
				player2.sprint = player2.sprint + 0.3;
		}
	}

	if (shot) {
		if (shootSpeed < 1) {
			shootSpeed += 0.003;
		}
		else {
			shot = false;
			shootSpeed = 0;
			player1.sprint = 0;
			player2.sprint = 0;
			if (turn == 0) {
				turn = 1;
			}
			else {
				turn = 0;
			}
		}
	}

	glutPostRedisplay();
}

void mou(int b, int s, int x, int y)
{
	if (player1.health != 0 && player2.health != 0) {
		if (b == GLUT_LEFT_BUTTON && s == GLUT_DOWN && !shot)
		{
			mouseDown = true;
		}
		if (b == GLUT_LEFT_BUTTON && s == GLUT_UP && !shot && mouseDown)
		{
			mouseDown = false;
			shot = true;
		}
	}
}