#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFuture>
#include <QFutureWatcher>

#include <QFileSystemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void showContextMenu(const QPoint& p);
	void finishedCounting();

private:
	void calculateAndLogSum();

	QFuture<void> m_future;
	QFutureWatcher<void> m_futureWatcher;
	QFile* m_pLogFile;
	QFileInfoList m_filesList;
	Ui::MainWindow *ui;

	QFileSystemModel m_model;
};

struct CalculatorFunctor: public std::unary_function<QFileInfo, void>
{
	CalculatorFunctor(QFile* pFile):
		std::unary_function<QFileInfo, void>()
	  , m_pLogFile(pFile)
	{ }

	void operator()(const QFileInfo& fileInfo);

private:
	const QFile* m_pLogFile;
	QMutex m_mutex;
};

#endif // MAINWINDOW_H
