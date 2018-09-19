#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
  glEnable(GL_DEPTH_TEST);
  init_camera (); //inicialitza càmera inicial OBS, VRP, up, FOV, zN, zF, raw
}

void MyGLWidget::paintGL () 
{
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  pintaPatricio1();
  pintaPatricio2();
  pintaPatricio3();

  pintaTerra();

  glBindVertexArray (0);

}


void MyGLWidget::pintaTerra () {
  // Carreguem la transformació de model
  modelTransformTerra ();

  // Activem el VAO per a pintar la caseta 
  glBindVertexArray (VAO_Terra);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray (0);
}

void MyGLWidget::pintaPatricio1 () {
  // Carreguem la transformació de model
  modelTransformPatricio1 ();

  // Activem el VAO per a pintar el patricio
  glBindVertexArray (VAO_Patricio);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, patricio.faces().size()*3);
  glBindVertexArray (0);
}

void MyGLWidget::pintaPatricio2 () {
  // Carreguem la transformació de model
  modelTransformPatricio2 ();

  // Activem el VAO per a pintar el patricio
  glBindVertexArray (VAO_Patricio);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, patricio.faces().size()*3);
  glBindVertexArray (0);

}
void MyGLWidget::pintaPatricio3 () {
  // Carreguem la transformació de model
  modelTransformPatricio3 ();

  // Activem el VAO per a pintar el patricio
  glBindVertexArray (VAO_Patricio);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, patricio.faces().size()*3);
  glBindVertexArray (0);

}

void MyGLWidget::modelTransformPatricio1 () 
{
  // Matriu de transformació de model
  // Calcul per moure la base del Patricio al (0,0,0) 
  CalculCapsaPatricio();

  scale2 = 1/(ymax-ymin);

  float cx = (xmax + xmin)/2;
  float cz = (zmax + zmin)/2;
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, glm::vec3(-2.f,0.f,-2.f));
  transform = glm::scale(transform, glm::vec3(scale2));
  transform = glm::rotate(transform,float(M_PI), glm::vec3(0.f,1.f,0.f));
  transform = glm::translate(transform, glm::vec3(-cx,-ymin,-cz));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformPatricio2 () 
{
  // Matriu de transformació de model
  // Calcul per moure la base del Patricio al (0,0,0) 
  CalculCapsaPatricio();

  scale2 = 1/(ymax-ymin);

  float cx = (xmax + xmin)/2;
  float cz = (zmax + zmin)/2;
  glm::mat4 transform (1.0f);
  transform = glm::rotate(transform,float(M_PI/2), glm::vec3(0.f,1.f,0.f));
  transform = glm::scale(transform, glm::vec3(scale2));
  transform = glm::translate(transform, glm::vec3(-cx,-ymin,-cz));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}
void MyGLWidget::modelTransformPatricio3 () 
{
  // Matriu de transformació de model
  // Calcul per moure la base del Patricio al (0,0,0) 
  CalculCapsaPatricio();

  scale2 = 1/(ymax-ymin);

  float cx = (xmax + xmin)/2;
  float cz = (zmax + zmin)/2;
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, glm::vec3(2.f,0.f,2.f));
  transform = glm::scale(transform, glm::vec3(scale2));
  transform = glm::translate(transform, glm::vec3(-cx,-ymin,-cz));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}




void MyGLWidget::modelTransformTerra () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  //scale
  //transform = glm::scale(transform, glm::vec3(1.f));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::projectTransform () 
{
  // Matriu de projeccio de model
  glm::mat4 Proj = glm::perspective(FOV_a,raw_actual,zNear,zFar);

  /*
  Al fer resize, aquests valors es modifiquen, llavors no poden
  ser fixos
  left = -radi_escena;
  right = radi_escena;
  bottom = -radi_escena;
  top = radi_escena;
  */

  if(ortogonal) Proj = glm::ortho (left, right, bottom, top, zNear, zFar);
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform () 
{
  // Matriu de view de model
  //glm::mat4 View = glm::lookAt(OBS,VRP,UP);
	glm::mat4 View = glm::mat4(1.0f);
	View=glm::translate(View,glm::vec3(0.f,0.f,-distancia));
	View = glm::rotate(View,RotaY, glm::vec3(1,0,0));
	View = glm::rotate(View,RotaX, glm::vec3(0,1,0));
	View = glm::translate(View,-VRP);

  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}

//inicialitza càmera inicial OBS, VRP, up, FOV, zN, zF, raw
void MyGLWidget::init_camera () 
{
  CalculCapsaEscena();

  // Inicialitzar parametres de la camera
	distancia = radi_escena*2;
	//FOV = M_PI/3.0; // 60 graus
	FOV = 2*asin(radi_escena/distancia);
	FOV_a = FOV;
    raw = 1.0f;
    raw_actual = raw;

    zNear = distancia - radi_escena;
    zFar = distancia + radi_escena;

    VRP = centre_escena;
    OBS = glm::vec3(0.f,2.f,distancia*1.f);
	UP = glm::vec3(0,1,0);
	

  left = -radi_escena;
  right = radi_escena;
  bottom = -radi_escena;
  top = radi_escena;

	viewTransform();
	projectTransform();
}



// Em se les dimensions de la escena
void MyGLWidget::CalculCapsaEscena() {
  xmax = 2.5;
  ymax = 1.0;
  zmax = 2.5;
  xmin = -2.5;
  ymin = 0.0;
  zmin = -2.5;

  calculaCentreRadiEscena();
} 

void MyGLWidget::calculaCentreRadiEscena() 
{

  centre_escena = glm::vec3((xmax+xmin)/2,(ymax+ymin)/2,(zmax+zmin)/2);
  
  float modulX = pow(xmax - centre_escena.x, 2);
  float modulY = pow(ymax - centre_escena.y, 2);
  float modulZ = pow(zmax - centre_escena.z, 2);
  radi_escena = sqrt(modulX + modulY + modulZ);
}

void MyGLWidget::CalculCapsaPatricio() {
	xmax = xmin = patricio.vertices()[0];
	ymax = ymin = patricio.vertices()[1]; 
	zmax = zmin = patricio.vertices()[2];

	for (unsigned int i = 3; i < patricio.vertices().size(); i+=3) {
		if(patricio.vertices()[i] > xmax) xmax = patricio.vertices()[i]; 
		if(patricio.vertices()[i] < xmin) xmin = patricio.vertices()[i];
		if(patricio.vertices()[i+1] > ymax) ymax = patricio.vertices()[i+1];
		if(patricio.vertices()[i+1] < ymin) ymin = patricio.vertices()[i+1]; 
		if(patricio.vertices()[i+2] > zmax) zmax = patricio.vertices()[i+2];
		if(patricio.vertices()[i+2] < zmin) zmin = patricio.vertices()[i+2];
	}
	calculaCentreRadiModel();
} 
//inicialitza el radi i centre esfera a partir de capsa escena
void MyGLWidget::calculaCentreRadiModel() 
{

  centre_model = glm::vec3((xmax+xmin)/2,(ymax+ymin)/2,(zmax+zmin)/2);
  

  float modulX = pow(xmax - centre_model.x, 2);
  float modulY = pow(ymax - centre_model.y, 2);
  float modulZ = pow(zmax - centre_model.z, 2);
  radi_model = sqrt(modulX + modulY + modulZ);
}






void MyGLWidget::resizeGL (int w, int h) 
{
  glViewport(0, 0, w, h);
  if(not ortogonal) { //Perspectiva
    float rav = float(w)/float(h);
    raw_actual = rav;
    FOV_a = FOV;
    if(rav <1.f) {
  	 FOV_a = 2*atan(tan(FOV/2.f)/rav);
    }
  }
  else { // Ortogonal
    float rav = float(w)/float(h); // Viewport

    float aw = right-left; // Amplada window
    float hw = top-bottom;  // Altura window
    float raw = aw/hw;

    raw_actual = rav;
    if(rav > raw) { // Incrementar aw
      left = -radi_escena*rav; // left = left*rav
      right = radi_escena*rav; // right = right*rav
      bottom = -radi_escena; // bottom = bottom
      top = radi_escena; // top = top
    }
    else if(rav < raw) { // Incrementar hw
      left = -radi_escena; // left = left
      right = radi_escena; // right = right
      bottom = -radi_escena/rav; // bottom = bottom/rav
      top = radi_escena/rav; // top = top/rav
    }
  }
  projectTransform();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: { // escalar a més gran
      scale += 0.05;
      scale2 += 0.05;
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      scale -= 0.05;
      scale2 -= 0.05;
      break;
    }
    case Qt::Key_R: { // escalar a més petit
      angle -= float(M_PI/4);
      break;
    }
    case Qt::Key_Z: { // Zoom IN
      FOV -= (float)M_PI/180;
      if(FOV < M_PI/18) FOV = M_PI/18; // FOV Limitat a 10 graus
      FOV_a = FOV;
      projectTransform();
      break;
    }
    case Qt::Key_X: { // Zoom OUT
      FOV += (float)M_PI/180;
      if(FOV > 5*M_PI/6) FOV = 5*M_PI/6; // FOV Limitat a 150 graus
      FOV_a = FOV;
      projectTransform();
      break;
    }
    case Qt::Key_O: { // Zoom OUT
      ortogonal = not ortogonal;
      projectTransform();
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::mouseMoveEvent (QMouseEvent *e) {
	makeCurrent();
  if(DoingInteractive == ROTATE) {
  	float novaX = e->x();
  	float novaY = e->y();

  	float x = novaX - antigaX;
  	float y = novaY - antigaY;

    antigaX = novaX;
    antigaY = novaY;

  	RotaX += (x/width())*10;
  	RotaY += (y/height())*10;

  	;

  	viewTransform();
  	update();
  }
  else if (DoingInteractive == ZOOM) {
    float novaX = e->x();
    float novaY = e->y();

    float x = novaX - antigaX;
    float y = novaY - antigaY;

    antigaX = novaX;
    antigaY = novaY;

    FOV -= (float)x*M_PI/(180*10);
    FOV += (float)y*M_PI/(180*10);
    
    if(FOV > 5*M_PI/6) FOV = 5*M_PI/6; // FOV Limitat a 150 graus
    if(FOV < M_PI/18) FOV = M_PI/18; // FOV Limitat a 10 graus
    FOV_a = FOV;
    
    projectTransform();
    update();
  }
}


void MyGLWidget::mousePressEvent (QMouseEvent *e) {
  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier))) {
    DoingInteractive = ROTATE; // Boto ESQUERRA premut, Euler(moure camera)


    // EXPLICACIO: La mateixa que abaix
    antigaX = e->x();
    antigaY = e->y();

  }
  if (e->button() & Qt::RightButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier))) {
    DoingInteractive = ZOOM; // Boto DRET premut, ZOOM

    // EXPLICACIO(perque no m'en recordare)
    // Per tal de poder fer correctament el ZOOM, vull que el ZOOM es faci segons el punt on cliko
    // Es a dir, que no faci aquest "efecte de reinici"
    // Si no ho pillo, comenta les seguent linies de codi i comprova
    antigaX = e->x();
    antigaY = e->y();
  }
}
    
void MyGLWidget::mouseReleaseEvent(QMouseEvent *) {
  DoingInteractive = NONE; // Quan deixi de premer, no ha de modificar res
}

void MyGLWidget::createBuffers () 
{
  carregaPatricio();
  CalculCapsaPatricio(); // Dona valor al radi
  glBindVertexArray (0);

  glm::vec3 vertexs[6] = {
	glm::vec3(-2.5, 0.0, -2.5),
	glm::vec3(-2.5, 0.0, 2.5),
	glm::vec3(2.5, 0.0, 2.5),

	glm::vec3(-2.5, 0.0, -2.5),
	glm::vec3(2.5, 0.0, -2.5),
	glm::vec3(2.5, 0.0, 2.5),

  }; 
  glm::vec3 colors[6] = {
	glm::vec3(0.58,0.29,0),
	glm::vec3(0.58,0.29,0),
	glm::vec3(0.58,0.29,0),
	glm::vec3(0.58,0.29,0),
	glm::vec3(0.58,0.29,0),
	glm::vec3(0.58,0.29,0),
}; 



  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  // Dos VBOs, un amb posició i l'altre amb color
  GLuint VBO_Vertex;
  glGenBuffers(1, &VBO_Vertex);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Vertex);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs),vertexs, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  GLuint VBO_Color;
  glGenBuffers(1, &VBO_Color);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Color);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors),colors, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  glBindVertexArray(0);


}
void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/fragshad.frag");
  vs.compileSourceFile("shaders/vertshad.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “color” del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");
  // Uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");

	projLoc = glGetUniformLocation(program->programId(), "proj");

	viewLoc = glGetUniformLocation(program->programId(), "view");

}


void MyGLWidget::carregaPatricio () 
{
  patricio.load("../models/Patricio.obj");
  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_Patricio);
  glBindVertexArray(VAO_Patricio);

  // Dos VBOs, un amb posició i l'altre amb color
  GLuint VBO_Color;
  GLuint VBO_Vertex;
  glGenBuffers(1, &VBO_Vertex);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Vertex);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patricio.faces().size()*9
  	, patricio.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_Color);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Color);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patricio.faces().size()*9
  	, patricio.VBO_matdiff(), GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

}
