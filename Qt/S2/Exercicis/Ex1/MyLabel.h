#include <QLabel>
class MyLabel: public QLabel
{
Q_OBJECT
public:
MyLabel (QWidget *parent);

public slots:
void tractaSlider (int slid);
void Trunca(int slid,QString text);

signals:
void enviaText (int slid,QString text);

};