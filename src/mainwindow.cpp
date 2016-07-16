#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <vector>
#include <QDebug>

mainWindow::mainWindow(QWidget *parent) :
    QMainWindow(parent),
    _presetsTable(new QTableWidget),
    _presetSettingsTable(new QTableWidget),
    _menuBar(new QMenuBar),
    _midiIn(new QMidiIn),
    _midiOut(new QMidiOut),
    _midiMessage(new QMidiMessage)
{
    //_midiIn->openPort(NULL);
    //_midiOut->openPort(NULL);


    // Layout

    QWidget *mainWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(mainWidget);


    // Menu Bar

    QMenu* menu1 = new QMenu("File", _menuBar);
    menu1->addAction("Settings");
    menu1->addSeparator();
    menu1->addMenu("Send");
    menu1->addMenu("Receive");

    _menuBar->addMenu(menu1);
    mainLayout->addWidget(_menuBar);

    // Table

    _presetsTable->setRowCount(128);
    _presetsTable->setColumnCount(1);

    for(int i=0; i<_presetsTable->rowCount(); i++){
        QString defaultCelString = QString("Preset #%1").arg(i+1);
        QTableWidgetItem *celContent = new QTableWidgetItem(defaultCelString);
        _presetsTable->setItem(i, 0, celContent);
    }

    _presetsTable->horizontalHeader()->setVisible(false);

    QStringList settingsList;
    settingsList << "Channel" << "Description"<< "Type" << "Parameter";
    _presetSettingsTable->setRowCount(16);
    _presetSettingsTable->setColumnCount(4);
    _presetSettingsTable->setHorizontalHeaderLabels(settingsList);


    mainLayout->addWidget(_presetsTable);
    mainLayout->addWidget(_presetSettingsTable);

    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);


    connect(_midiIn, SIGNAL(midiMessageReceived(QMidiMessage*)), this, SLOT(onMidiMessageReceive(QMidiMessage*)));

}

mainWindow::~mainWindow()
{

}

void mainWindow::onMidiMessageReceive(QMidiMessage *message)
{
    std::vector<unsigned char> rawMessage = message->getRawMessage();
    // MAKE SURE THE RECEIVED MESSAGE IS COMING FROM A POCKETC (F0 00 20 20 14 00) AND THAT message.size() > 7 BYTES
    if(rawMessage.size() > 7)
    {
        // PROCESS THE MESSAGE ACCORDING TO THE BYTE #7
        switch(rawMessage.at(6))
        {
            case THRU_MC_DUMP:
            {
                break;
            }

            case SINGLE_DUMP:
            {
                break;
            }
            case VERSION_ANS:
            {
                break;
            }
        }
    }

    //qDebug() << "BYTE #7 : " << rawMessage.at(6) ; // DEBUG
}
