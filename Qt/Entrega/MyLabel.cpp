#include "MyLabel.h"
// constructor
MyLabel::MyLabel(QWidget *parent):QLabel(parent) {
	//connect(this,SIGNAL(returnPressed()),this,SLOT(tractaReturn()));
	hora = 0;
	minut = 0;
	ON = false;
}

// Signal per posar els valors a 0 i posar OFF en el button
void MyLabel::reset() {
	emit reseteja(0);
}



void MyLabel::off() {
	this->setStyleSheet("QLabel { background-color : red; color : white; }"); // Color

	setText("Alarma desactivada");
	ON = false;
}

void MyLabel::on() {
	this->setStyleSheet("QLabel { background-color : green; color : white; }");

	QString h = QString::number(hora);
    QString m = QString::number(minut);
    QString result = "Alarma -- "+ h + "::" + m;
	setText(result);

	ON = true;
}

// Actualitzar la hora
void MyLabel::updateHora(int h) {
	hora = h;
	if(ON) on();
}

// Actualitza minut
void MyLabel::updateMinut(int m) {
	minut = m;
	if(ON) on();
}
