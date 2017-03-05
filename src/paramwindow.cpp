#include "paramwindow.h"
#include <QHBoxLayout>

paramWindow::paramWindow(QWidget *parent) :
    QMainWindow(parent),
    _paramNumsTable(new QTableWidget)
{
    QWidget *mainWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(mainWidget);

    _paramNumsTable->setRowCount(8);
    _paramNumsTable->setColumnCount(16);
    _paramNumsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _paramNumsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _paramNumsTable->horizontalHeader()->setHidden(true);
    _paramNumsTable->horizontalHeader()->setDefaultSectionSize(40);
    _paramNumsTable->verticalHeader()->setHidden(true);
    _paramNumsTable->verticalHeader()->setDefaultSectionSize(30);

    int i=0;
    for(int r=0; r<_paramNumsTable->rowCount(); r++){
        for(int c=0; c<_paramNumsTable->columnCount(); c++){
            QString val = QString::number(i);
            _paramNumsTable->setItem(r,c,new QTableWidgetItem(val));
            i++;
        }
    }

    mainLayout->addWidget(_paramNumsTable);
    mainWidget->setFixedSize(670,270);
    mainWidget->setLayout(mainLayout);
    this->setCentralWidget(mainWidget);

    connect(_paramNumsTable,SIGNAL(cellDoubleClicked(int,int)),parent,SLOT(updateParamCell(int,int)));

}

paramWindow::~paramWindow()
{
}
