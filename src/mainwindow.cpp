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

    for(int i=0; i<_presets->rowCount(); i++){
        QString defaultCelString = QString("Preset #%1").arg(i+1);
        QTableWidgetItem *celContent = new QTableWidgetItem(defaultCelString);
        _presets->setItem(i, 0, celContent);
    }

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
