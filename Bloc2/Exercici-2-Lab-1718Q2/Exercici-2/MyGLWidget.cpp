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
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  createBuffersModel();
  createBuffersTerra();

  init_camera(); // inicialitza camera

  projectTransform ();
  viewTransform ();
}

void MyGLWidget::paintGL () 
{
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Activem el VAO per a pintar el terra 
  glBindVertexArray (VAO_Terra);

  modelTransformTerra ();

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, 6);

  // Activem el VAO per a pintar el Patricio
  glBindVertexArray (VAO_Patr);

  modelTransformPatricio ();

  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);

  glBindVertexArray (VAO_Patr);

  modelTransformPatricio2 ();

  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  

  glBindVertexArray (VAO_Patr);

  modelTransformPatricio3 ();

  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  
  
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h) 
{
  glViewport(0, 0, w, h);

  if(not camera_per_defecte) {
    float rav = float(w)/float(h);
    raw_actual = rav;
    FOV_a = FOV;
    if(rav <1.f) {
     FOV_a = 2*atan(tan(FOV/2.f)/rav);
   }
   projectTransform();
  }
  else {
    float rav = float(w)/float(h);
    raw_actual_defecte = rav;
    FOV_a_defecte = FOV_defecte;
    if(rav <1.f) {
     FOV_a_defecte = 2*atan(tan(FOV_defecte/2.f)/rav);
   }
   projectTransform();
  }

}

void MyGLWidget::modelTransformPatricio ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(-2.f,0.f,-2.f));
  TG = glm::rotate(TG,float(M_PI), glm::vec3(0.f,1.f,0.f));
  TG = glm::scale(TG, glm::vec3(escala, escala, escala));
  TG = glm::translate(TG, -centreBasePatr);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio2 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::rotate(TG,float(M_PI/2), glm::vec3(0.f,1.f,0.f));
  TG = glm::scale(TG, glm::vec3(escala, escala, escala));
  TG = glm::translate(TG, -centreBasePatr);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio3 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(2.f,0.f,2.f));
  TG = glm::scale(TG, glm::vec3(escala, escala, escala));
  TG = glm::translate(TG, -centreBasePatr);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTerra ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj;  // Matriu de projecció
  if(camera_per_defecte) Proj = glm::perspective(FOV_a_defecte,raw_actual_defecte , 4.0f, 12.0f); // FOV, raw_acutal, Znear, Zfar
  else Proj = glm::perspective(FOV_a, raw_actual, zNear, zFar);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform ()
{
  glm::mat4 View;  // Matriu de posició i orientació
  if(camera_per_defecte) View = glm::lookAt(glm::vec3(0, 0.5, 8),glm::vec3(0, 0.5, 0),glm::vec3(0, 1, 0));
  else View = glm::lookAt(OBS,VRP,UP);

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}




//inicialitza càmera inicial OBS, VRP, up, FOV, zN, zF, raw
void MyGLWidget::init_camera () 
{
  // Inicialitzar parametres de la camera
  FOV = M_PI/2; // 90 graus
  FOV_a = FOV;
  raw = 1.0f;
  raw_actual = raw;

  zNear = 0.1f;
  zFar = 30.f;

  VRP = glm::vec3(0.f,1.f,0.f);;
  OBS = glm::vec3(2.f,1.f,2.f);
  UP = glm::vec3(0,1,0);




  FOV_defecte = float(M_PI/3.0);
  FOV_a_defecte = FOV_defecte;
  raw_actual_defecte = 1.f;

  camera_per_defecte = true;
}











void MyGLWidget::createBuffersModel ()
{
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  patr.load("./models/Patricio.obj");

  // Calculem la capsa contenidora del model
  calculaCapsaModel ();
  
  // Creació del Vertex Array Object del Patricio
  glGenVertexArrays(1, &VAO_Patr);
  glBindVertexArray(VAO_Patr);

  // Creació dels buffers del model patr
  GLuint VBO_Patr[2];
  // Buffer de posicions
  glGenBuffers(2, VBO_Patr);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Patr[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Buffer de component difusa usada com a color
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Patr[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray(0);
}

void MyGLWidget::createBuffersTerra ()
{
  // Dades del terra
  // VBO amb la posició dels vèrtexs
  glm::vec3 posterra[6] = {
	glm::vec3(-2.5, 0.0, 2.5),
	glm::vec3(2.5, 0.0, 2.5),
	glm::vec3(-2.5, 0.0, -2.5),
	glm::vec3(-2.5, 0.0, -2.5),
	glm::vec3(2.5, 0.0, 2.5),
	glm::vec3(2.5, 0.0, -2.5),
  }; 

  glm::vec3 color(0.8,0,0.8);

  // Fem que aquest material afecti a tots els vèrtexs per igual
  glm::vec3 colterra[6] = {
	color, color, color, color, color, color
  };

// Creació del Vertex Array Object del terra
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  GLuint VBO_Terra[2];
  glGenBuffers(2, VBO_Terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colterra), colterra, GL_STATIC_DRAW);

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

  // Demanem identificadors per als uniforms del vertex shader
  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "proj");
  viewLoc = glGetUniformLocation (program->programId(), "view");
}

void MyGLWidget::calculaCapsaModel ()
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = patr.vertices()[0];
  miny = maxy = patr.vertices()[1];
  minz = maxz = patr.vertices()[2];
  for (unsigned int i = 3; i < patr.vertices().size(); i+=3)
  {
    if (patr.vertices()[i+0] < minx)
      minx = patr.vertices()[i+0];
    if (patr.vertices()[i+0] > maxx)
      maxx = patr.vertices()[i+0];
    if (patr.vertices()[i+1] < miny)
      miny = patr.vertices()[i+1];
    if (patr.vertices()[i+1] > maxy)
      maxy = patr.vertices()[i+1];
    if (patr.vertices()[i+2] < minz)
      minz = patr.vertices()[i+2];
    if (patr.vertices()[i+2] > maxz)
      maxz = patr.vertices()[i+2];
  }
  escala = 1.0/(maxy-miny);
  centreBasePatr[0] = (minx+maxx)/2.0; centreBasePatr[1] = miny; centreBasePatr[2] = (minz+maxz)/2.0;
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_C: { 
      // aquesta tecla ha de canviar de càmera entre la que hi ha per defecte i la que es demana (1ª persona)
      camera_per_defecte = not camera_per_defecte;
      viewTransform();
      projectTransform();
      break;
    }
    case Qt::Key_Left: {
      // aquesta tecla ha de fer que la càmera giri a la seva esquerra
      float x = 0.28f;
      float z = 0.28f;
      if(not camera_per_defecte) {
        if(VRP.x < OBS.x) VRP += glm::vec3(0.0f,0.f,z);
        if(VRP.x >= OBS.x) VRP += glm::vec3(0.0f,0.f,-z);
        if(VRP.z < OBS.z) VRP += glm::vec3(-x,0.f,0.0f);
        if(VRP.z >= OBS.z) VRP += glm::vec3(x,0.f,0.0f);
        viewTransform();
      }
      break;
    }
    case Qt::Key_Right: { 
      // aquesta tecla ha de fer que la càmera giri a la seva dreta
      float x = 0.28f;
      float z = 0.28f;
      if(not camera_per_defecte) {
        if(VRP.x < OBS.x) VRP += glm::vec3(0.0f,0.f,-z);
        if(VRP.x >= OBS.x) VRP += glm::vec3(0.0f,0.f,z);
        if(VRP.z < OBS.z) VRP += glm::vec3(x,0.f,0.0f);
        if(VRP.z >= OBS.z) VRP += glm::vec3(-x,0.f,0.0f);
        viewTransform();
      }
      break;
    }
    case Qt::Key_Up: { 
      // aquesta tecla ha de fer que la càmera es mogui endavant
      if(not camera_per_defecte) {
        float x = 0.04f;
        float zero = 0.f;
        if(VRP.x < OBS.x) {
          VRP +=glm::vec3(-x,zero,zero);
          OBS +=glm::vec3(-x,zero,zero);
        }
        if(VRP.x >= OBS.x) {
          VRP +=glm::vec3(x,zero,zero);
          OBS +=glm::vec3(x,zero,zero);
        }
        if(VRP.z < OBS.z) {
          VRP +=glm::vec3(zero,zero,-x);
          OBS +=glm::vec3(zero,zero,-x);
        }
        if(VRP.z >= OBS.z) {
          VRP +=glm::vec3(zero,zero,x);
          OBS +=glm::vec3(zero,zero,x);
        }
        viewTransform();
        }       
      break;
    }
    case Qt::Key_Down: { 
      // aquesta tecla ha de fer que la càmera es mogui endarrera
      if(not camera_per_defecte) {
        float x = 0.04f;
        float zero = 0.f;
        if(VRP.x < OBS.x) {
          VRP +=glm::vec3(x,zero,zero);
          OBS +=glm::vec3(x,zero,zero);
        }
        if(VRP.x >= OBS.x) {
          VRP +=glm::vec3(-x,zero,zero);
          OBS +=glm::vec3(-x,zero,zero);
        }
        if(VRP.z < OBS.z) {
          VRP +=glm::vec3(zero,zero,x);
          OBS +=glm::vec3(zero,zero,x);
        }
        if(VRP.z >= OBS.z) {
          VRP +=glm::vec3(zero,zero,-x);
          OBS +=glm::vec3(zero,zero,-x);
        }
        viewTransform();
        }   
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

