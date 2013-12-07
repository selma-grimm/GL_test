#include "MainWindow.h"
#include "CRCTestClass.h"


void CRCTestClass::testCRC()
{
    QFETCH(QString, fileName);
    QFETCH(QString, correct_crc);

    QString spr(QDir::separator());
    fileName.prepend(/*".." + spr +*/
                     ".." + spr +
                     "GL_test" + spr +
                     "files_for_test" + spr
                     );
    qDebug() << fileName;

    QString calculated_crc(CalculatorFunctor::checksum_test(fileName));

    QCOMPARE(calculated_crc, correct_crc);
}

void CRCTestClass::testCRC_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QString>("correct_crc");
    //supposed we know exact sums for this files
    QTest::newRow("1") << "1.jpg" << "ff96cc01";
    QTest::newRow("2") << "2.jpg" << "89f3cc05";
    QTest::newRow("3") << "3.jpg" << "9db44119";
}
