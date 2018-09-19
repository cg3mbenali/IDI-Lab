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


    //**************************************************************************//
    //**************************************************************************//
    //**************************************************************************//
    //**************************************************************************//
    // SIGNALS I SLOTS
    public slots:
    void actualitzaFOV(int fov);
 	void canviaModel(bool boto);
 	void canviaOptica(bool boto);
 	void canviaEscala(int scale);
 	void canviaEulerX(int angle);
 	void canviaEulerY(int angle);
 	void actualitzarColorTerra(int red, int blue, int green);


 	signals:
	void enviaEscala (int scale);
	void enviaEulerX (int angle);
	void enviaEulerY (int angle);



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

    // mousePressEvent - Es cridat quan es mou el ratoli
    virtual void mouseMoveEvent (QMouseEvent *e);

    // mousePressEvent -  Es cridat quan es prem un boto el ratoli
    virtual void mousePressEvent (QMouseEvent *e);
    
    // mouseReleaseEvent - Es cridat quan es prem un boto el ratoli
    virtual void mouseReleaseEvent(QMouseEvent *);


  private:
    void createBuffers ();
    void carregaShaders ();

    void modelTransformPatricio1 ();
    void modelTransformPatricio2 ();
    void modelTransformPatricio3 ();
    void modelTransformLegoman1 ();
    void modelTransformLegoman2 ();
    void modelTransformLegoman3 ();
    void modelTransformTerra ();
	void projectTransform ();
	void viewTransform ();

	void init_camera();
    void CalculCapsaEscena();
    void calculaCentreRadiEscena();

	void calculaCentreRadiModel();

    void carregaPatricio();
    void carregaLegoman();
    void CalculCapsaModel(const Model &model);

    void pintaPatricio1();
    void pintaPatricio2();
    void pintaPatricio3();
    void pintaLegoman1();
    void pintaLegoman2();
    void pintaLegoman3();
    void pintaTerra();


    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint transLoc;
	GLuint projLoc;
	GLuint viewLoc;
    // VAO i VBO names
    GLuint VAO_Patricio;
    GLuint VAO_Legoman;
    GLuint VAO_Terra;
    Model patricio;
    Model legoman;
    // Program
    QOpenGLShaderProgram *program;
    // Internal vars
    double scale = 1.0;
    double scale2 = 1.0;
    float angle = 0.f;

    // Rotacio amb ratoli
    float RotaX = 0;
    float RotaY = 0;
    float antigaX = 0;	// Pixels
	float antigaY = 0;	// Pixels

	//	Rotcio amb slots i signals
	int antigAngleX = 0;	// Comença a 0, graus
	int antigAngleY = 0; 	// Comença a 0, graus



    //init_esfera();
    glm::vec3 centre_escena, centre_model;
    float radi_escena, radi_model;
    float distancia;// Distancia del OBS al VRP

    //init_camera() 
    glm::vec3 OBS, VRP, UP;
    float FOV, FOV_a, zNear, zFar, raw_actual,raw;

    
    float left;
    float right;
    float bottom;
    float top;

    bool ortogonal = false;
    bool pintaPatricio = true;

    //CalculaCapsaModel();
    float xmax, ymax, zmax, xmin, ymin, zmin;
    glm::vec3 centre_capsa;


    typedef enum {ROTATE, NONE, ZOOM} InteractiveAction;
	InteractiveAction DoingInteractive;









};

