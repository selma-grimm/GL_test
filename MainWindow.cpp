#include <QTreeView>
#include <QFileSystemModel>
#include <QLayout>
#include <QMenu>
#include <QtConcurrentMap>
#include <QFile>

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, m_pLogFile(new QFile())
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
	m_pLogFile->close();
	delete m_pLogFile;
	m_pLogFile = nullptr;
}

void MainWindow::calculateAndLogSum()
{
	QModelIndexList allIndexes = ui->tree->selectionModel()->selectedIndexes();
	m_filesList.clear();
	foreach (const QModelIndex& index, allIndexes)
	{
		if (index.column() == 0)
		{
			QFileInfo fi = m_model.fileInfo(index);
			if (!fi.isDir())
				m_filesList << fi;
		}
	}

	if (m_filesList.isEmpty())
		return;

	QString logName = m_filesList.first().absolutePath() + QDir::separator() + "test_log.txt";

	m_pLogFile->setFileName(logName);
	if (!m_pLogFile->open(QIODevice::WriteOnly | QIODevice::Append))
//TODO: do something nice here
		return;

	m_future = QtConcurrent::map(m_filesList, CalculatorFunctor(m_pLogFile));
	m_futureWatcher.setFuture(m_future);
}

CalculatorFunctor::CalculatorFunctor(QFile *pFile):
	std::unary_function<QFileInfo, void>()
  , m_pLogFile(pFile)
  , m_mutex(QMutex(QMutex::NonRecursive))
{

}

void CalculatorFunctor::operator()(const QFileInfo &fileInfo)
{
	QMutexLocker locker(&m_mutex);

	QString toWrite(fileInfo.absoluteFilePath());
	m_pLogFile->write(toWrite.toStdString().c_str());

}


