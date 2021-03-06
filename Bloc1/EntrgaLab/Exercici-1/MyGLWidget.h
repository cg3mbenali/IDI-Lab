#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
     // resize - Es cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);  
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);

  private:
    void createBuffersTriangle ();
    void carregaShaders ();
    void pintaTriangle ();
    
    void modelTransformT1 ();
    void modelTransformT2 ();

    // attribute locations
    GLuint vertexLoc, colorLoc, transLoc;

    float angle1 = 0.0;
    float angle2 = 0.0;

    
    // Program
    QOpenGLShaderProgram *program;

    GLuint VAO1;
};
