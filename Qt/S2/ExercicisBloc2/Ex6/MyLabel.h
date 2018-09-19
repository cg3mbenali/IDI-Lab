#include <QLabel>
class MyLabel: public QLabel
{
Q_OBJECT
public:

int red, green, blue;

MyLabel (QWidget *parent);

public slots:
void agafaRed (int valor);  // valor en RGB
void agafaGreen (int valor);  // valor en RGB
void agafaBlue (int valor);  // valor en RGB


signals:
void enviaColor (int red,int green, int blue);

private:
	
};