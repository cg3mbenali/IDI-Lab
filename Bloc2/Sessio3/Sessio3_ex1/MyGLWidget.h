#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <model.h>
#include <QMouseEvent>

#include <cmath>
#include <iostream>
#include <algorithm> // per calcular mínims.

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);
    // mousePressEvent - Es cridat quan es prem el ratoli
    virtual void mouseMoveEvent (QMouseEvent *e);

  private:
    void createBuffers ();
    void carregaShaders ();
    void modelTransformPatricio ();
    void modelTransformTerra ();
	void projectTransform ();
	void viewTransform ();
	void init_camera();
	void calculaCentreRadi();
    void carregaPatricio();
    void CalculCapsaPatricio();

    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint transLoc;
	GLuint projLoc;
	GLuint viewLoc;
    // VAO i VBO names
    GLuint VAO_Patricio;
    GLuint VAO_Terra;
    Model patricio;
    // Program
    QOpenGLShaderProgram *program;
    // Internal vars
    float scale = 1.0f;
    float scale2 = 0.8f;
    float angle = 0.f;

    float RotaX = 0;
    float RotaY = 0;
    float antigaX = 0;
	float antigaY = 0;


    //init_esfera();
    glm::vec3 centre_escena;
    float radi_escena;
    float distancia;// Distancia del OBS al VRP

    //init_camera() 
    glm::vec3 OBS, VRP, UP;
    float FOV, FOV_a, zNear, zFar, raw_actual,raw;

    //CalculaCapsaPatricio();
    float xmax, ymax, zmax, xmin, ymin, zmin;
    glm::vec3 centre_capsa;


    typedef enum {ROTATE, NONE, ZOOM} InteractiveAction;
	InteractiveAction DoingInteractive;


};

