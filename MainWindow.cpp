#include <QTreeView>
#include <QFileSystemModel>
#include <QLayout>
#include <QMenu>

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	model.setRootPath(QDir::currentPath());
	model.setFilter(QDir::AllEntries);

	QTreeView* view = ui->tree;
	view->setModel(&model);
	view->setSelectionBehavior(QAbstractItemView::SelectRows);
	view->setSelectionMode(QAbstractItemView::MultiSelection);
	view->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(view,
			SIGNAL(customContextMenuRequested(QPoint)),
			this,
			SLOT(showContextMenu(QPoint)));

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

void MainWindow::calculateAndLogSum()
{

}
