#include "splot.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SPlot plot;
    plot.resize(640, 480);
    plot.show();

    return a.exec();
}
