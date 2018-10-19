#include "MyGLWidget.h"

#include <iostream>

GLuint VAO2; // Crea VAO2

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
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
}

void MyGLWidget::paintGL ()
{
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  // Activem l'Array a pintar 
  glBindVertexArray(VAO1);
 
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, 3);
  
  // Desactivem el VAO
  glBindVertexArray(0);

	glBindVertexArray(VAO2);
 
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, 6);
  
  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
  glViewport (0, 0, w, h);
}

void MyGLWidget::createBuffers ()
{
  glm::vec3 Vertices1[3];  // Tres vèrtexs amb X, Y i Z    Triangle
  Vertices1[0] = glm::vec3(0.0, 0.0, 0.0);
  Vertices1[1] = glm::vec3(1.0, 0.0, 0.0);
  Vertices1[2] = glm::vec3(0.5, 1.0, 0.0);
	
	glm::vec3 Vertices2[6]; // SIS vèrtexs amb X, Y i Z      Quadrat
	Vertices2[0] = glm::vec3(-1.0, -1.0, 0.0);
  Vertices2[1] = glm::vec3(-1.0, 0.0, 0.0);
  Vertices2[2] = glm::vec3(0.0, -1.0, 0.0);
	Vertices2[3] = glm::vec3(-1.0, 0.0, 0.0);
  Vertices2[4] = glm::vec3(0.0, 0.0, 0.0);
  Vertices2[5] = glm::vec3(0.0, -1.0, 0.0);

  
  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO1); // VAO Triangle
  glBindVertexArray(VAO1);

	glGenVertexArrays(1, &VAO2); // VAO Quadrat

  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO1; // VBO1 Triangle
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices1), Vertices1, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Desactivem el VAO
  glBindVertexArray(0);

	glBindVertexArray(VAO2); // Activa VAO2 ( Quadrat)

	// Creació del buffer amb les dades dels vèrtexs
  GLuint VBO2; // VBO2 Quadrat
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2), Vertices2, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Desactivem el VAO
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
}
