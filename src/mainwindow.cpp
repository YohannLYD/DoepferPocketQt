#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <vector>
#include <QDebug>

mainWindow::mainWindow(QWidget *parent) :
    QMainWindow(parent),
    _settingsWindow(new settingsWindow(this)),
    _presetsTable(new QTableWidget),
    _presetSettingsTable(new QTableWidget),
    _menuBar(new QMenuBar),
    _midiMessage(new QMidiMessage),
    _midiIn(new QMidiIn(this)),
    _midiOut(new QMidiOut(this))
{
    _prefixPocketC =  {0xF0,0x00,0x20,0x20,0x14,0x00};
    _midiIn->setIgnoreTypes(false, false, false);
    // Layout

    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    QHBoxLayout *tableLayout = new QHBoxLayout;

    // Menu Bar

    QMenu* mainMenu = new QMenu("File", _menuBar);
    QAction* settings = mainMenu->addAction("Settings");
    mainMenu->addSeparator();
    QMenu* send = mainMenu->addMenu("Send");
    QAction* sendSingle = send->addAction("Send this preset");
    QAction* sendAll = send->addAction("Send all presets");

    _menuBar->addMenu(mainMenu);
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


    tableLayout->addWidget(_presetsTable);
    tableLayout->addWidget(_presetSettingsTable);

    mainLayout->addLayout(tableLayout);

    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);


    connect(_midiIn, SIGNAL(midiMessageReceived(QMidiMessage*)), this, SLOT(onMidiMessageReceive(QMidiMessage*)));
    connect(settings, SIGNAL(triggered(bool)), this, SLOT(openSettingsWindow()));

}

mainWindow::~mainWindow()
{

}

// SLOTS

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
                _settingsWindow->updateConfig(message);
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

void mainWindow::openSettingsWindow()
{
    _settingsWindow->setWindowModality(Qt::ApplicationModal);
    _settingsWindow->show();
}

void mainWindow::openMidiPorts(){
    _midiIn->closePort();
    _midiOut->closePort();
    if(_settingsWindow->_inPortComboBox->currentText() != "None"){
        qDebug()<< "Selected MIDI IN : " <<_settingsWindow->_inPortComboBox->currentText();
        _midiIn->openPort(_settingsWindow->_inPortComboBox->currentIndex());
    }
    if(_settingsWindow->_outPortComboBox->currentText() != "None"){
        qDebug()<< "Selected MIDI OUT : " <<_settingsWindow->_outPortComboBox->currentText();
        _midiOut->openPort(_settingsWindow->_outPortComboBox->currentIndex());
    }
}

void mainWindow::sendThruMasterChnRequest(){
    std::vector<unsigned char> rawRequest;
    rawRequest = _prefixPocketC;
    rawRequest.push_back(0x56);
    rawRequest.push_back(0x00);
    rawRequest.push_back(0x00);
    rawRequest.push_back(0xF7);
    if(_midiOut->isPortOpen()) _midiOut->sendRawMessage(rawRequest);
}
