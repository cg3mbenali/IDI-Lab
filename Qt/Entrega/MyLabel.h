#include <QLabel>
#include <QColor>
#include <QPalette>
class MyLabel: public QLabel
{
Q_OBJECT
public:
MyLabel (QWidget *parent);

int hora, minut;
bool ON;


public slots:
void reset();
void on();
void off();
void updateHora(int h);
void updateMinut(int m);

signals:
void reseteja (int valor);

};