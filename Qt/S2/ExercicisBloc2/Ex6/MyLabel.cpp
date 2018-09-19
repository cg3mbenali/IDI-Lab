#include "MyLabel.h"
// constructor
MyLabel::MyLabel(QWidget *parent):QLabel(parent) {
	//connect(this,SIGNAL(returnPressed()),this,SLOT(tractaReturn()));
	red = 0;
	green = 0;
	blue = 0;
}


void MyLabel::agafaRed (int valor) {
	red = valor;
	emit enviaColor (red,green,blue);
}
void MyLabel::agafaGreen (int valor) {
	green = valor;
	emit enviaColor (red,green,blue);
}
void MyLabel::agafaBlue (int valor) {
	blue = valor;
	emit enviaColor (red,green,blue);
}

