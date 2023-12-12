#include "ImageView.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageView w;
    w.show();
    return a.exec();
}
