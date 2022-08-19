#include <gl/glut.h> 
#include <stdio.h>  
#include <iostream>
#include "glaux.h" 

#pragma warning(disable:4996)
#pragma comment(lib, "legacy_stdio_definitions.lib")

using namespace std;

GLfloat angle, fAspect;
GLint obsX = 0;
GLint obsY = 800;
GLint obsZ = 0;
GLint alvoX = 0;
GLint alvoY = 0;
GLint alvoZ = 0;
GLint vetX = 0;
GLint vetY = 0;
GLint vetZ = 1;
GLfloat cam_near = 0.1;
GLfloat cam_far = 1800.0;
GLuint idTextura_Terra, idTextura_Lua;
AUX_RGBImageRec* imagemTerra, *imagemLua;


//Parametros para animacao do planeta e do satelite
GLfloat anguloRotacaoPlaneta = 0, anguloRotacaoSatelite = 0;
GLint raioSatelite = 300;

//Função para Carregar uma imagem .BMP
AUX_RGBImageRec* LoadBMP(const char* Filename) {
	FILE* File = NULL;
	if (!Filename) {
		return NULL;
	}
	File = fopen(Filename, "r");
	if (File)	// Se o arquivo existe
	{
		fclose(File);
		return auxDIBImageLoadA(Filename);//Retorna a imagem
	}
	return NULL;
}

void Desenha(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Planeta
	glBindTexture(GL_TEXTURE_2D, idTextura_Terra);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glRotated(anguloRotacaoPlaneta, 0, 1, 0);
	glRotated(-90, 1, 0, 0);
	GLUquadric* quadric = gluNewQuadric();	//Utilizado para adicionar texturas em obj esfericos
	gluQuadricDrawStyle(quadric, GLU_FILL);	//Especifica estilo de desenho do quadrics
	gluQuadricNormals(quadric, GLU_SMOOTH);	//Especifica o tipo de normais desejadas
	gluQuadricTexture(quadric, GL_TRUE);	//Coordenadas da textura devem ser geradas para o quadric
	gluSphere(quadric, 100.0, 50.0, 20.0);
	gluDeleteQuadric(quadric);
	glPopMatrix();

	//Satelite
	glBindTexture(GL_TEXTURE_2D, idTextura_Lua);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glRotated(anguloRotacaoSatelite, 0, 1, 0);
	glTranslated(raioSatelite, 0, 0);
	glRotated(-90, 1, 0, 0);
	quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);
	gluSphere(quadric, 25.0, 50.0, 20.0);
	gluDeleteQuadric(quadric);
	glPopMatrix();
	
	glutSwapBuffers();
}

void Timer(int value) {
	anguloRotacaoPlaneta = value;
	anguloRotacaoSatelite = value / 3.0;

	glutPostRedisplay();
	glutTimerFunc(20, Timer, value + 1);
}

void Def_Iluminacao() {
	// Configuração de Iluminação
	GLfloat luzAmbiente[4] = { 0.2,0.2,0.2,1.0 };
	GLfloat luzDifusa[4] = { 0.5,0.5,0.5,1.0 };
	GLfloat luzEspecular[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat posicaoLuz[4] = { -900.0, 00.0, 0.0, 1.0 };

	// Capacidade de brilho do material
	GLfloat especularidade[4] = { 1.0, 1.0,1.0,1.0 }; // define a cor do brilho no material, geralmente branco
	GLint expoenteEspecular = 50; // define o brilho do material. varia de 0-128. Mais espelhado ou menos espelhado.

	// Especifica que a cor de fundo da janela ser� preta
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Habilita o modelo de coloriza��o de Gouraud
	glShadeModel(GL_SMOOTH);

	// Define a reflet�ncia do material 
	glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
	// Define a concentra��o do brilho
	glMateriali(GL_FRONT, GL_SHININESS, expoenteEspecular);

	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os par�metros da luz de n�mero 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

	// Habilita a defini��o da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de ilumina��o
	glEnable(GL_LIGHTING);
	// Habilita a luz de n�mero 0
	glEnable(GL_LIGHT0);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);
}

void Inicializa(void)
{
	//Define iluminacao
	Def_Iluminacao();

	// Comandos de inicialização para textura
	imagemTerra = LoadBMP("Imagens/earthmap1k.bmp");
	glGenTextures(1, &idTextura_Terra);
	glBindTexture(GL_TEXTURE_2D, idTextura_Terra);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imagemTerra->sizeX,
		imagemTerra->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE,
		imagemTerra->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Uma para minificação 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//Outra para maximizacao
	glEnable(GL_TEXTURE_2D);

	// Comandos de inicialização para textura
	imagemLua = LoadBMP("Imagens/moonmap1k.bmp");
	glGenTextures(1, &idTextura_Lua);
	glBindTexture(GL_TEXTURE_2D, idTextura_Lua);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imagemLua->sizeX,
		imagemLua->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE,
		imagemLua->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Uma para minificação 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//Outra para maximizacao
	glEnable(GL_TEXTURE_2D);

	angle = 45;
}

void EspecificaParametrosVisualizacao(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(angle, fAspect, cam_near, cam_far);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(obsX, obsY, obsZ, alvoX, alvoY, alvoZ, vetX, vetY, vetZ);
}

void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	if (h == 0) h = 1;

	glViewport(0, 0, w, h);

	fAspect = (GLfloat)w / (GLfloat)h;

	EspecificaParametrosVisualizacao();
}

void GerenciaMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		vetX = 1;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		vetX = -1;
	}
	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

void GerenciaTeclado(unsigned char key, int x, int y) {
	switch (key) {
	case ' ': // restaura posição inicial da camera
		obsX = 0;
		obsY = 800;
		obsZ = 0;
		alvoX = 0;
		alvoY = 0;
		alvoZ = 0;
		vetX = 0;
		vetY = 0;
		vetZ = 1;
		break;
		// movimentacao do observador
	case 'd':
		obsX = obsX - 50;
		break;
	case 'a':
		obsX = obsX + 50;
		break;
	case 'w':
		obsY = obsY - 50;
		break;
	case 's':
		obsY = obsY + 50;
		break;
	case 'q':
		obsZ = obsZ + 50;
		break;
	case 'e':
		obsZ = obsZ - 50;
		break;
	}
	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

void TeclasEspeciais(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) {
		alvoZ = alvoZ + 50;
	}
	if (key == GLUT_KEY_DOWN) {
		alvoZ = alvoZ - 50;
	}
	if (key == GLUT_KEY_RIGHT) {
		alvoX = alvoX - 50;
	}
	if (key == GLUT_KEY_LEFT) {
		alvoX = alvoX + 50;
	}
	if (key == GLUT_KEY_F1) {
		cam_far -= 30;
	}
	if (key == GLUT_KEY_F2) {
		cam_far += 30;
	}

	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

int main(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(400, 350);

	glutCreateWindow("Aula 11");

	glutMouseFunc(GerenciaMouse);		// Define qual funcao gerencia o comportamento do mouse
	glutKeyboardFunc(GerenciaTeclado);	// Define qual funcao gerencia o comportamento do teclado
	glutSpecialFunc(TeclasEspeciais);	// Define qual funcao gerencia as teclas especiais

	glutDisplayFunc(Desenha);
	glutReshapeFunc(AlteraTamanhoJanela);

	// Define a funcao timer ==============
	glutTimerFunc(60, Timer, 1);

	Inicializa();
	glutMainLoop();
}