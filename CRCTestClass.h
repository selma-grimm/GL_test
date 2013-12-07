#ifndef CRCTESTCLASS_H
#define CRCTESTCLASS_H

#include <QObject>
#include <QTest>

class CRCTestClass : public QObject
{
    Q_OBJECT
    
private slots:
    void testCRC();
    void testCRC_data();
    
};

#endif // CRCTESTCLASS_H
