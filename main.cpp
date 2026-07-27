#include "echoterminalwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
     EchoTerminalWindow w;
    w.show();
    return a.exec();
}
