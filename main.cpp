#include "Calculator.h"
#include <QtWidgets/QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication *a = new QApplication(argc, argv);
	QFile file("style.qss");
	file.open(QIODevice::ReadOnly);
	QString style = QLatin1String(file.readAll());
	a->setStyleSheet(style);
    Calculator calc;
    calc.show();
    return a->exec();
}
