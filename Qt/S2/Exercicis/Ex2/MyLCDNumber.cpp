#include "MyLCDNumber.h"
// constructor
MyLCDNumber::MyLCDNumber(QWidget *parent):QLCDNumber(parent) {
	//connect(this,SIGNAL(returnPressed()),this,SLOT(tractaReturn()));
// Inicialització d'atributs si cal
}


// implementació slots
void MyLCDNumber::actualitza(int slid) 	{
	QPalette *p = new QPalette;
	if(slid == 0) p->setColor(QPalette::WindowText, QColor(0, 255, 0));
	else if(slid % 2 == 0) p->setColor(QPalette::WindowText, QColor(0, 0, 255));
	else p->setColor(QPalette::WindowText, QColor(255, 0, 0));
	setPalette(*p);
    setSegmentStyle(Filled); 

	display(slid);
}

void MyLCDNumber::reset() 	{
	display(0);
}
