#include "mainwindow.h"
#include <QHBoxLayout>

mainWindow::mainWindow(QWidget *parent) :
    QMainWindow(parent),
    _presets(new QTableWidget),
    _presetSettings(new QTableWidget)
{
    QWidget *mainWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(mainWidget);

    _presets->setRowCount(128);
    _presets->setColumnCount(1);
//    _presets->setHorizontalHeaderLabels(c_str('Preset'));
    _presets->horizontalHeader()->setVisible(false);

    QStringList settingsList;
    settingsList << "Channel" << "Description"<< "Type" << "Parameter";
    _presetSettings->setRowCount(16);
    _presetSettings->setColumnCount(4);
    _presetSettings->setHorizontalHeaderLabels(settingsList);


    mainLayout->addWidget(_presets);
    mainLayout->addWidget(_presetSettings);

    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}

mainWindow::~mainWindow()
{

}
