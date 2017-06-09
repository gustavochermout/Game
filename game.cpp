#include <bits/stdc++.h> 
#include <GL/glut.h> 

double X, Y, Z;
bool bootAtivo;

struct Robo{

	double x; 
	double z;
	double anguloBracoDir;
	double anguloBracoEsq;
	int lado;
	
} r1, r2;

void inicializar(){
	X = 26.75;
	Y = 18.25;
	Z = 19.0;	
	bootAtivo = false;
	
	r1.x = 10.0;
	r1.z = 0.0;
	r1.anguloBracoDir = 90.0; 
	r1.anguloBracoEsq = 90.0;
	r1.lado = 1;
		
	r2.x = -10.0;
	r2.z = 0.0;
	r2.anguloBracoDir = 90.0; 
	r2.anguloBracoEsq = 90.0;
	r2.lado = 3;
}

void base(double tam){
	glBegin(GL_LINE_LOOP);
		glVertex3f( tam, 0.0,  tam);
		glVertex3f(-tam, 0.0,  tam);
		glVertex3f(-tam, 0.0, -tam);
		glVertex3f( tam, 0.0, -tam);
	glEnd();	
}

double ladoParaAngulo(int lado){
	switch(lado){
		case 1: return 0.0;
		case 2: return 270.0;
		case 3: return 180.0;
		case 4: return 90.0;
	}
}

void objeto(double x, double y, double z, double largura, double altura, double profundidade,
double angulo){
	glPushMatrix();
		glTranslatef(x, y/2, z);
		glScalef(profundidade, altura, largura);
		glRotatef(angulo, 0, 0, 1);
		glutWireCube(1.0);
	glPopMatrix();	
}

void corpoDoRobo(double anguloBracoDir, double anguloBracoEsq){
	//Pernas
	objeto(0.0, 1.2, -1.0, 0.7, 1.5, 0.7, -5.0);
	objeto(0.0, 1.2, 1.0, 0.7, 1.5, 0.7, 5.0);
	
	//Corpo
	objeto(0.0, 5.0, 0.0, 2.7, 2.5, 1.5, 0.0);
	
	//Cabeça
	objeto(0.0, 8.5, 0.0, 1.5, 1.0, 1.5, 0.0);

	//Braços	
	objeto(-0.4, 6.3, 1.7, 0.6, 1.9, 0.6, anguloBracoEsq);
	objeto(-0.4, 6.3, -1.7, 0.6, 1.9, 0.6, anguloBracoDir);
}

void robo(double x, double y, double z, int lado, double anguloBracoDir, double anguloBracoEsq){
	glPushMatrix();
		glTranslatef(x, y/2, z);
		glRotatef(ladoParaAngulo(lado), 0, 1, 0);
		corpoDoRobo(anguloBracoDir, anguloBracoEsq);
	glPopMatrix();
}

void myReshape(int w, int h) {
	glViewport (0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, float(w)/h, 1, 90);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void myDisplay(void) {
	glClearColor(0.2, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	gluLookAt(X, Y, Z,
		0, 0, 0,
		0, 1, 0);
		  	  
	glColor3f(1, 1, 1);
	base(15);
  
  	glColor3f(0, 0, 1);
	robo(r1.x, 0.0, r1.z, r1.lado, r1.anguloBracoDir, r1.anguloBracoEsq);
    
	glColor3f(1, 0, 0);
	robo(r2.x, 0.0, r2.z, r2.lado, r2.anguloBracoDir, r2.anguloBracoEsq);		
			
	glutSwapBuffers();
}

#define soco 45.0

void socar(struct Robo *robo, int idBraco){
	if (idBraco)
		robo->anguloBracoDir += soco;
	else
		robo->anguloBracoEsq += soco;			
}

#define giro 0.5
#define passo_camera 0.5
#define passo_robo 0.1

void tecla(unsigned char c, int, int){
	
	bootAtivo = true;
	
	switch(c){
		//Camera
		case '1': {X -= passo_camera; break;};
		case '2': {X += passo_camera; break;};
		
		case '3': {Y -= passo_camera; break;};
		case '4': {Y += passo_camera; break;};
	
		case '5': {Z -= passo_camera; break;};	
		case '6': {Z += passo_camera; break;};
		
		//Robo 1 (Movimento)
		case 'w': {if (r1.lado != 1) r1.lado = 1; r1.x -= passo_robo; break;};
		case 's': {if (r1.lado != 3) r1.lado = 3; r1.x += passo_robo; break;};
		case 'd': {if (r1.lado != 2) r1.lado = 2; r1.z -= passo_robo; break;};
		case 'a': {if (r1.lado != 4) r1.lado = 4; r1.z += passo_robo; break;};
		
		//Robo 1 (Soco)
		case 'k': {socar(&r1, 0); break;};
		case 'l': {socar(&r1, 1); break;};
	}
		
	glutPostRedisplay();
}

void boot(){
	
	if (r2.x < (r1.x - 2))
		r2.x += passo_robo;
	
	socar(&r2, 1);	
}

#define tempo 100

void Timer(int value){
	
	if (bootAtivo)
		boot();
		
	glutPostRedisplay();
	glutTimerFunc(tempo, Timer, 1);
}

int main(int argc, char** argv) {
	
	inicializar();	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
	glutKeyboardFunc(tecla);
	glutTimerFunc(tempo, Timer, 1);
		
	glEnable(GL_DEPTH_TEST);
	
	glutMainLoop();
	
	return 0;
}
