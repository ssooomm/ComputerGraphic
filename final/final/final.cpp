#include <gl/glut.h>
#include <math.h>
#include <vector>
using namespace std;


int MX = 300, MY = 300;
int i = 0;

float timer = 0;

float turn = 0;
float x = 0, y = 0;

float m1 = 0, m2 = 0;

float car_move = 6.0;
int wheel_rotate = 0;

GLfloat red[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat brown[] = { 0.54, 0.27, 0.07, 1.0 };
GLfloat green[] = { 0.13, 0.54, 0.13, 1.0 };
GLfloat blue[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat chocolate[] = { 0.8, 0.54, 0.25, 1.0 };
GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };

class vec3 {
public:
	float m[3];
	vec3(float x = 0, float y = 0, float z = 0) {
		m[0] = x; m[1] = y; m[2] = z;
	}
	vec3 operator+(vec3 x) {
		return vec3(m[0] + x.m[0],
			m[1] + x.m[1],
			m[2] + x.m[2]);
	}

	vec3 operator-(vec3 x) {
		return vec3(m[0] - x.m[0],
			m[1] - x.m[1],
			m[2] - x.m[2]);
	}

	vec3 operator*(float x) {
		return vec3(m[0] * x, m[1] * x, m[2] * x);
	}

	float Length() {
		return sqrtf(m[0] * m[0] + m[1] + m[2] * m[2]);
	}

};

void MyMouseMove(GLint x, GLint y) { //카메라 위치 바꾸기
	MX = x;
	MY = y;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(MX / 300.0, MY / 300.0, 1.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);

	glutPostRedisplay();
}

void MySpecial(int key, int X, int Y) {
	if (key == GLUT_KEY_UP) {
		glLoadIdentity();
		switch (i) {
		case 0:
			gluLookAt(MX / 300.0, MY / 300.0, 1.0, MX / 300.0, 0.0, 0.0, 0.0, 1.0, 0.0);
			i++;
			break;
		case 1:
			gluLookAt(MX / 300.0, MY / 300.0, 1.0, 0.0, MY / 300.0, 0.0, 0.0, 1.0, 0.0);
			i++;
			break;
		case 2:
			gluLookAt(MX / 300.0, MY / 300.0, 1.0, -MX / 300.0, 0.0, 0.0, 0.0, 1.0, 0.0);
			i++;
			break;
		case 3:
			gluLookAt(MX / 300.0, MY / 300.0, 1.0, 0.0, -MY / 300.0, 0.0, 0.0, 1.0, 0.0);
			i = 0;
			break;
		}
	}

	if (key == GLUT_KEY_DOWN) {
	}

	//차 움직임
	if (key == GLUT_KEY_LEFT) {
		x += 0.5f;
	}
	if (key == GLUT_KEY_RIGHT) {
		x -= 0.5f;
	}

	glutPostRedisplay();
}

void MyIdle() {
	timer += 1.0;
	m1 -= 0.005f;
	m2 += 0.007f;

	wheel_rotate += 1;
	car_move -= 0.001;

	if (car_move < -9) {
		exit(0);
	}

	glutPostRedisplay();
}

void MyReshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-15.0, 15.0, -15.0, 15.0, -100.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(MX / 300.0, MY / 300.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);  //시점변환
}

void InitLight() {
	GLfloat mat_diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_ambient[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat mat_shininess[] = { 15.0 };
	GLfloat light_specular[] = { 1.0, 0.8, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat light_position[] = { -3, 6, 3.0, 0.0 };
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glEnable(GL_NORMALIZE);
}

bool HitCheck() { //동물과 자동차 부딪힘
	vec3 moving_car(car_move, 2.0, x);
	vec3 moving_animal1(2.0, 2.0, (15 + m1));
	vec3 moving_animal2(0.0, 2.0, (-20 + m2));
	vec3 diff1 = moving_animal1 - moving_car;
	vec3 diff2 = moving_animal2 - moving_car;

	if ((diff1.Length() < 1.5) || (diff2.Length() < 1.5)) //1.5이하면 부딪힘
		return true;

	return false;
}

void tree() {
	for (int i = 0; i < 6; i++) {
		glTranslatef(0.0, 0.4, 0.0);
		glutSolidCube(0.5);
	}
	glRotatef(180, 0.0, 1.0, 1.0);
	glutSolidCone(1.0, 1.5, 10, 10);
	glTranslatef(0.0, 0.0, 1);
	glutSolidCone(1.0, 1.5, 10, 10);
}

void wheel1() {
	glPushMatrix();
	glTranslatef(-0.7, -0.6, 0.5);
	glRotatef((GLfloat)wheel_rotate, 0.0, 0.0, 1.0);
	glutSolidTorus(0.2, 0.1, 5, 5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.7, -0.6, 0.5);
	glRotatef((GLfloat)wheel_rotate, 0.0, 0.0, 1.0);
	glutSolidTorus(0.2, 0.1, 5, 5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.7, -0.6, -0.5);
	glRotatef((GLfloat)wheel_rotate, 0.0, 0.0, 1.0);
	glutSolidTorus(0.2, 0.1, 5, 5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.7, -0.6, -0.5);
	glRotatef((GLfloat)wheel_rotate, 0.0, 0.0, 1.0);
	glutSolidTorus(0.2, 0.1, 5, 5);
	glPopMatrix();
}

void car() {
	//차체
	glScalef(2.0, 0.7, 1.0);
	glutSolidCube(1.0);
	//차 윗부분
	glPushMatrix();
	glTranslatef(0, 0.7, 0);
	glScalef(0.5, 1.0, 1.0);
	glutSolidCube(1.0);
	glPopMatrix();
	//바퀴
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glPushMatrix();
	glScalef(0.5, 1.0, 1.0);
	wheel1();
	glPopMatrix();
}

void animal() {
	//하체
	glScalef(1.0, 1.0, 1.5);
	glutSolidSphere(1.5, 50, 50);

	//다리1
	glPushMatrix(); //허벅지
	glRotatef(sin(timer * 0.01) * 5 - 10, 1.0, 0.0, 0.0);
	glTranslatef(0.5, -1.5, 0.5);
	glScalef(0.4, 1.0, 0.4);
	glutSolidCube(1.0);
	glPushMatrix();//종아리
	glRotatef(cos(timer * 0.005) * 10 + 20, 1.0, 0.0, 0.0);
	glTranslatef(0, -1.0, 0);
	glScalef(1.0, 2.0, 1.0);
	glutSolidCube(0.5);
	glPushMatrix();//발
	glTranslatef(0, -0.3, 0.3);
	glScalef(2.2, 0.3, 2.5);
	glutSolidSphere(0.3, 50, 50);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//다리2
	glPushMatrix(); //허벅지
	glRotatef(-sin(timer * 0.01) * 5 - 10, 1.0, 0.0, 0.0);
	glTranslatef(-0.5, -1.5, 0.5);
	glScalef(0.4, 1.0, 0.4);
	glutSolidCube(1.0);
	glPushMatrix(); //종아리
	glRotatef(cos(timer * 0.005) * 10 + 20, 1.0, 0.0, 0.0);
	glTranslatef(0, -1.0, 0);
	glScalef(1.0, 2.0, 1.0);
	glutSolidCube(0.5);
	glPushMatrix();//발
	glTranslatef(0, -0.3, 0.3);
	glScalef(2.2, 0.3, 2.5);
	glutSolidSphere(0.3, 50, 50);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//다리3
	glPushMatrix(); //허벅지
	glRotatef(sin(timer * 0.01) * 5 - 10, 1.0, 0.0, 0.0);
	glTranslatef(-0.5, -1.5, -0.5);
	glScalef(0.4, 1.0, 0.4);
	glutSolidCube(1.0);
	glPushMatrix(); //종아리
	glRotatef(cos(timer * 0.005) * 10 + 20, 1.0, 0.0, 0.0);
	glTranslatef(0, -1.0, 0);
	glScalef(1.0, 2.0, 1.0);
	glutSolidCube(0.5);
	glPushMatrix();	//발
	glTranslatef(0, -0.3, 0.3);
	glScalef(2.2, 0.3, 2.5);
	glutSolidSphere(0.3, 50, 50);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//다리4
	glPushMatrix(); //허벅지
	glRotatef(-sin(timer * 0.01) * 5 - 10, 1.0, 0.0, 0.0);
	glTranslatef(0.5, -1.5, -0.5);
	glScalef(0.4, 1.0, 0.4);
	glutSolidCube(1.0);
	glPushMatrix();	//종아리
	glRotatef(cos(timer * 0.005) * 10 + 20, 1.0, 0.0, 0.0);
	glTranslatef(0, -1.0, 0);
	glScalef(1.0, 2.0, 1.0);
	glutSolidCube(0.5);
	glPushMatrix();	//발
	glTranslatef(0, -0.3, 0.3);
	glScalef(2.2, 0.3, 2.5);
	glutSolidSphere(0.3, 50, 50);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//상체
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	glTranslatef(0.0, -0.5, 0.5);
	glutSolidCone(1, 3, 50, 50);
	//머리
	glPushMatrix();
	glTranslatef(0, -0.5, 2.5);
	glScalef(1.0, 1.0, 0.9);
	glutSolidSphere(1.0, 50, 50);
	//귀
	glPushMatrix();
	glScalef(1.0, 0.5, 0.5);
	glTranslatef(0.4, 0.5, 1.7);
	glutSolidCone(0.3, 1.5, 50, 50);
	glTranslatef(-0.7, 0.0, 0.0);
	glutSolidCone(0.3, 1.5, 50, 50);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//꼬리
	glPushMatrix();
	glRotatef(sin(timer * 0.005) * 10 - 20, 1.0, 0.0, 0.0);
	glTranslatef(0, 1.0, -1.5);
	glScalef(1.0, 1.0, 2.5);
	glutSolidSphere(0.2, 50, 50);
	glPopMatrix();
}

void MyDisplay() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//판
	//glMaterialfv(GL_FRONT, GL_AMBIENT, chocolate);
	glPushMatrix();
	glScalef(50, 0.5, 50);
	glutSolidCube(1.0);
	glPopMatrix();

	//나무
	glMaterialfv(GL_FRONT, GL_AMBIENT, green);
	glPushMatrix();
	glScalef(2.0, 2.0, 2.0);
	glTranslatef(-6, 0, 10);
	for (int i = 0; i < 6; i++) {
		glTranslatef(0, 0, -3);
		glPushMatrix();
		tree();
		glPopMatrix();
	}
	glPopMatrix();

	//자동차
	glMaterialfv(GL_FRONT, GL_AMBIENT, red);
	glPushMatrix();
	glScalef(2.0, 2.0, 2.0);
	glTranslatef(car_move, 0.8, 0);
	glTranslatef(0, 0, x); //차가 움직임
	car();
	glPopMatrix();

	//동물
	glMaterialfv(GL_FRONT, GL_AMBIENT, brown);
	glPushMatrix();
	glTranslatef(2.0, 1.5, 15);
	glTranslatef(0.0, 0.0, m1);
	glRotatef(180, 0, 1, 0);
	glScalef(0.4, 0.4, 0.4);
	animal();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 2.2, -20);
	glTranslatef(0.0, 0.0, m2);
	glScalef(0.6, 0.6, 0.6);
	animal();
	glPopMatrix();

	glFlush();

	bool hit = HitCheck();
	if (hit)
		exit(0); //부딪히면 강제 종료
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutCreateWindow("OpenGL Drawing Example");
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutIdleFunc(MyIdle);
	glutSpecialFunc(MySpecial); //키값 받는
	glutMotionFunc(MyMouseMove); //마우스 움직이는
	glClearColor(0.4, 0.4, 0.4, 1);
	InitLight();

	glutMainLoop(); //프로그램 시작

	return 0;
}