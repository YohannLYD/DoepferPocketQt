#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <vector>
#include <QDebug>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

mainWindow::mainWindow(QWidget *parent) :
    QMainWindow(parent),
    _settingsWindow(new settingsWindow(this)),
    _presetsList(new QListWidget),
    _presetSettingsTable(new QTableWidget),
    _menuBar(new QMenuBar),
    _midiMessage(new QMidiMessage),
    _midiIn(new QMidiIn(this)),
    _midiOut(new QMidiOut(this))
{
    _prefixPocketC =  {0xF0,0x00,0x20,0x20,0x14,0x00};
    _midiIn->setIgnoreTypes(false, false, false);

    // Temporary default starting values (all parameters set to zero)
    for(int i=0; i<128; i++){
        for(int j=0; j<48; j++){
            _preset[i][j] = 0;
        }
    }

    // Layout

    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    QHBoxLayout *tableLayout = new QHBoxLayout;

    // Menu Bar

    QMenu* mainMenu = new QMenu("File", _menuBar);
    QAction* settings = mainMenu->addAction("Settings");
    mainMenu->addSeparator();

    QMenu* get = mainMenu->addMenu("Get");
    QAction* getSingle = get->addAction("Get this preset");
    connect(getSingle,SIGNAL(triggered(bool)),this,SLOT(sendSingleDumpRequest()));

    QAction* getAll = get->addAction("Get all presets");
    connect(getAll,SIGNAL(triggered(bool)),this,SLOT(sendAllDumpRequest()));

    QMenu* send = mainMenu->addMenu("Send");
    QAction* sendSingle = send->addAction("Send this preset");
    connect(sendSingle,SIGNAL(triggered(bool)),this,SLOT(sendSingleDump()));

    QAction* sendAll = send->addAction("Send all presets");

    _menuBar->addMenu(mainMenu);
    mainLayout->addWidget(_menuBar);

    QPushButton *getPresetButton = new QPushButton;
    getPresetButton->setText("Get preset");
    mainLayout->addWidget(getPresetButton);
    connect(getPresetButton, SIGNAL(clicked(bool)),this,SLOT(sendSingleDumpRequest()));

    // Table
    for(int i=0; i<128; i++){
        QString defaultCelString = QString("Preset #%1").arg(i+1);
        _presetsList->addItem(defaultCelString);
    }

    connect(_presetsList,SIGNAL(itemSelectionChanged()),this,SLOT(updateTable()));

    QStringList settingsList;
    settingsList << "Channel" << "Type" << "Parameter" << "Description" ;
    _presetSettingsTable->setRowCount(16);
    _presetSettingsTable->setColumnCount(4);
    _presetSettingsTable->setHorizontalHeaderLabels(settingsList);


    tableLayout->addWidget(_presetsList);
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
                this->updatePreset(message);
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

void mainWindow::updateDeviceConfig()
{
    if(!_midiOut->isPortOpen()) return;

    std::bitset<3> *thru = _settingsWindow->_thruBitset;
    int mastChn = _settingsWindow->_masterChannelSpinBox->value() - 1;

    std::vector<unsigned char> rawRequest;
    rawRequest = _prefixPocketC;
    rawRequest.push_back(0x50);
    rawRequest.push_back(thru->to_ulong());
    rawRequest.push_back(mastChn);
    rawRequest.push_back(0xF7);

    _midiOut->sendRawMessage(rawRequest);

}

void mainWindow::updatePreset(QMidiMessage *message)
{
    int presetNum = message->getRawMessage().at(7);
    for(int i=0; i<48; i++){
        _preset[presetNum][i] = message->getRawMessage().at(9+i);
    }
}

void mainWindow::updateTable()
{
    // ROWS
    for(int i=0; i<16; i++){
        //COLS
        for(int j=0; j<3; j++){
            QString value = QString::number(_preset[_presetsList->currentRow()][i+16*j]);
            _presetSettingsTable->setItem(i,j,new QTableWidgetItem(value));
        }
    }
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

void mainWindow::sendSingleDumpRequest(){
    std::vector<unsigned char> rawRequest;
    rawRequest = _prefixPocketC;
    rawRequest.push_back(0x26);
    rawRequest.push_back(_presetsList->currentRow());
    rawRequest.push_back(0x00);
    rawRequest.push_back(0xF7);
    if(_midiOut->isPortOpen()) _midiOut->sendRawMessage(rawRequest);
}

void mainWindow::sendAllDumpRequest(){
    _presetsList->setCurrentRow(0);
    for(int i=0; i<_presetsList->count(); i++){
        _presetsList->setCurrentRow(i);
        sendSingleDumpRequest();
    }

}

void mainWindow::sendSingleDump(){
    std::vector<unsigned char> rawRequest;
    rawRequest = _prefixPocketC;
    rawRequest.push_back(0x20);
    rawRequest.push_back(_presetsList->currentRow());
    rawRequest.push_back(0x00);

    for(int i=0; i<3; i++){
        for(int j=0; j<16; j++){
            int value = _presetSettingsTable->item(j,i)->text().toInt();
            rawRequest.push_back(value);
        }
    }

    rawRequest.push_back(0xF7);
    if(_midiOut->isPortOpen()) _midiOut->sendRawMessage(rawRequest);

    std::vector<unsigned char> storeRequest;
    storeRequest = _prefixPocketC;
    storeRequest.push_back(0x30);
    storeRequest.push_back(_presetsList->currentRow());
    storeRequest.push_back(0x00);
    storeRequest.push_back(0xF7);
    if(_midiOut->isPortOpen()) _midiOut->sendRawMessage(storeRequest);
}
