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

struct CalculatorFunctor;

class FileInfo: public QFileInfo
{
public:
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
    void cleanup();

private:
	void calculateAndLogSum();

    Ui::MainWindow *ui;
	QFuture<void> m_future;
	QFutureWatcher<void> m_futureWatcher;
	QFile* m_pLogFile;
    std::set<FileInfo> m_fiSet;
    CalculatorFunctor* m_cf;

	QFileSystemModel m_model;
};

struct CalculatorFunctor: public std::unary_function<FileInfo, void>
{
	CalculatorFunctor(QFile* pFile);

    void operator()(const FileInfo& fileInfo);

private:
    QString makeHumanReadable(qint64 iSize);
    QString checksum(const FileInfo& fi);

    const std::shared_ptr<QFile> m_pLogFile;
    const std::shared_ptr<QMutex> m_pMutex;
};

#endif // MAINWINDOW_H
