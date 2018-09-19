#include <QApplication>
#include "MyForm.h"

int main (int argc, char **argv) 
{
  QApplication a( argc, argv);

  MyForm form;
  form.show();

  return a.exec ();
}

