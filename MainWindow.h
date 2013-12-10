#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFuture>
#include <QFutureWatcher>
#include <QFileSystemModel>
#include <set>
#include <memory>

namespace Ui {
class MainWindow;
}

struct Calculator;

class FileInfo: public QFileInfo
{
public:
    FileInfo():
        QFileInfo()
    { }

    FileInfo(const QFileInfo& qfi):
        QFileInfo(qfi)
    { }

    bool operator<(FileInfo other) const
    {
        return fileName() < other.fileName();
    }
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void showContextMenu(const QPoint& p);

private:
	void calculateAndLogSum();

    Ui::MainWindow *ui;
	QFileSystemModel m_model;
};

struct Calculator: public QRunnable
{
    Calculator(const FileInfo& fi, std::shared_ptr<QFile> pFile, std::shared_ptr<QMutex> pMutex);
    void run();    

    static QString checksum_test(const QString& fileName);

private:
    QString makeHumanReadable(qint64 iSize);
    static QString checksum(const FileInfo& fi);

    FileInfo m_fi;
    const std::shared_ptr<QFile> m_pLogFile;
    const std::shared_ptr<QMutex> m_pMutex;

    static const int BUFSIZE = 8192;
    static const quint32 m_crc32_tab[256];
};
#endif // MAINWINDOW_H
