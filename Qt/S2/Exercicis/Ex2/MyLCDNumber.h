#include <QLCDNumber>
#include <QColor>
#include <QPalette>
class MyLCDNumber: public QLCDNumber
{
Q_OBJECT
public:
MyLCDNumber (QWidget *parent);

public slots:
void actualitza (int slid);
void reset();

//signals:

};