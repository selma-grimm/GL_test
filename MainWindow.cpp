#include <QTreeView>
#include <QFileSystemModel>
#include <QLayout>
#include <QMenu>
#include <QtConcurrentMap>
#include <QFile>
#include <QDateTime>

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
	, ui(new Ui::MainWindow)
    , m_pLogFile(nullptr)
    , m_cf(nullptr)
{
	ui->setupUi(this);

	m_model.setRootPath(QDir::currentPath());
	m_model.setFilter(QDir::AllEntries);

	QTreeView* view = ui->tree;
	view->setModel(&m_model);
	view->setSelectionBehavior(QAbstractItemView::SelectRows);
	view->setSelectionMode(QAbstractItemView::MultiSelection);
	view->setContextMenuPolicy(Qt::CustomContextMenu);
    view->setColumnWidth(0, 300);

	connect(view,
			SIGNAL(customContextMenuRequested(QPoint)),
			this,
			SLOT(showContextMenu(QPoint)));

	connect(&m_futureWatcher,
			SIGNAL(finished()),
            SLOT(cleanup()));

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

void MainWindow::cleanup()
{
	m_pLogFile->close();
}

void MainWindow::calculateAndLogSum()
{
    const QModelIndexList allIndexes = ui->tree->selectionModel()->selectedRows(0);
    m_fiSet.clear();
	foreach (const QModelIndex& index, allIndexes)
	{	
        const QFileInfo& qfi = m_model.fileInfo(index);
        if (!qfi.isDir())
            m_fiSet.insert(FileInfo(qfi));
	}

    if (m_fiSet.empty())
		return;

    QString logName = m_fiSet.begin()->absolutePath() + QDir::separator() + "test_log.txt";

    m_pLogFile = new QFile(logName);
	if (!m_pLogFile->open(QIODevice::WriteOnly | QIODevice::Append))
		return;

    m_cf = new CalculatorFunctor(m_pLogFile);
    m_future = QtConcurrent::map(m_fiSet, *m_cf);
	m_futureWatcher.setFuture(m_future);
}

CalculatorFunctor::CalculatorFunctor(QFile *pFile):
	std::unary_function<QFileInfo, void>()
  , m_pLogFile(std::shared_ptr<QFile>(pFile))
  , m_pMutex(std::shared_ptr<QMutex>(new QMutex(QMutex::NonRecursive)))
{ }

void CalculatorFunctor::operator()(const QFileInfo &fileInfo)
{    
    QString toWrite(fileInfo.completeBaseName() + " - ");
    toWrite.append(" SIZE: " + makeHumanRedable(fileInfo.size()));
    toWrite.append(" CREATED: " + fileInfo.created().toString("dd.MM.yyyy"));
    toWrite.append(" MODIFIED: " + fileInfo.lastModified().toString("dd.MM.yyyy"));
    toWrite.append("\n");

    QMutexLocker locker(m_pMutex.get());    
    m_pLogFile->write(toWrite.toUtf8());
}

QString CalculatorFunctor::makeHumanRedable(qint64 iSize)
{
    QString result;
    int bytesPerKB = 1024;
    int bytesPerMB = bytesPerKB * 1024;
    int bytesPerGB = bytesPerMB * 1024;

    if (iSize > bytesPerGB)
    {
       int r = iSize / bytesPerGB;
       result = QString::number(r) + " ";
       r = iSize % bytesPerGB;
       result += QString::number(r) + " GB";
    }
    else if (iSize > bytesPerMB)
    {
        int r = iSize / bytesPerMB;
        result = QString::number(r) + " MB";
    }
    else if (iSize > bytesPerKB)
    {
        int r = iSize / bytesPerKB;
        result =  QString::number(r) + " KB";
    }
    else
    {
        result = QString::number(iSize) + " B";
    }

    return result;
}



