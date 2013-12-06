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
	void calculateOneFile(const QString& path);

	QFuture<void> m_future;
	QFutureWatcher<void> m_futureWatcher;
	QFile m_logFile;
	Ui::MainWindow *ui;

	QFileSystemModel m_model;
};

#endif // MAINWINDOW_H
