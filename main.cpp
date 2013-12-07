#include "MainWindow.h"
#include "CRCTestClass.h"
#include <QApplication>

//#define RUN_TEST

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
#ifndef RUN_TEST
	MainWindow w;
	w.show();
#else
    QTest::qExec(new CRCTestClass);
#endif

	return a.exec();
}
