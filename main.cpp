#include "MainWindow.h"
#include <QApplication>

#ifdef RUN_TEST
#include "CRCTestClass.h"
#endif


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
