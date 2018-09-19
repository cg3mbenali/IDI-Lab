#include "MyLabel.h"
// constructor
MyLabel::MyLabel(QWidget *parent):QLabel(parent) {
	//connect(this,SIGNAL(returnPressed()),this,SLOT(tractaReturn()));
// Inicialització d'atributs si cal
}


// implementació slots
void MyLabel::tractaSlider(int slid) {
	// Implementació de tractaSlider
	emit enviaText (slid,text());
	
}

void MyLabel::Trunca(int slid,QString text) {
	text.truncate(slid);
	setText(text);
}
