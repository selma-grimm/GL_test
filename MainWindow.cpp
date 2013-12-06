#include <QTreeView>
#include <QFileSystemModel>
#include <QLayout>
#include <QMenu>
#include <QtConcurrentRun>

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_model.setRootPath(QDir::currentPath());
	m_model.setFilter(QDir::AllEntries);

	QTreeView* view = ui->tree;
	view->setModel(&m_model);
	view->setSelectionBehavior(QAbstractItemView::SelectRows);
	view->setSelectionMode(QAbstractItemView::MultiSelection);
	view->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(view,
			SIGNAL(customContextMenuRequested(QPoint)),
			this,
			SLOT(showContextMenu(QPoint)));

	connect(&m_futureWatcher,
			SIGNAL(finished()),
			SLOT(finishedCounting()));

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::showContextMenu(const QPoint &p)
{
	QPoint gp(mapToGlobal(p));
	QMenu cont_menu;
	cont_menu.addAction("Calculate the Sum");

	QAction* selected_item = cont_menu.exec(gp);
	if (selected_item && (selected_item->text() == "Calculate the Sum"))
		calculateAndLogSum();
}

void MainWindow::finishedCounting()
{

}

void MainWindow::calculateAndLogSum()
{
//TODO: add check for empty selection or only folders
	QModelIndexList allIndexes = ui->tree->selectedIndexes();
	QStringList fileNamesList;
	foreach (const QModelIndex& index, allIndexes)
	{
		if (index.column() == 0)
		{
			QFileInfo fi = m_model.fileInfo(index);
			if (!fi.isDir())
				fileNamesList << fi.absoluteFilePath();
		}
	}

	QFileInfo fi = m_model.fileInfo(allIndexes.first());
	QString logName = fi.absolutePath() + QDir::separator() + "test_log.txt";

	m_logFile.setFileName(logName);
	if (!m_logFile.open(QIODevice::WriteOnly | QIODevice::Append))
//TODO: do something nice here
		return;

	m_future = QtConcurrent::map(fileNamesList, calculateOneFile);

	m_futureWatcher.setFuture(m_future);
}

void MainWindow::calculateOneFile(const QString &path)
{
	m_logFile.close();
}














