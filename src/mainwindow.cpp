#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <vector>
#include <QDebug>
#include <QThread>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

mainWindow::mainWindow(QWidget *parent) :
    QMainWindow(parent),
    _eventWindow(new eventWindow(this)),
    _paramWindow(new paramWindow(this)),
    _settingsWindow(new settingsWindow(this)),
    _presetsList(new QListWidget),
    _presetSettingsTable(new QTableWidget),
    _menuBar(new QMenuBar),
    _midiMessage(new QMidiMessage),
    _midiIn(new QMidiIn(this)),
    _midiOut(new QMidiOut(this))
{
    for(int i=0; i<8; i++){
        _presetSlider.append(new QSlider());
        if(i==7) _presetSlider.at(7)->setDisabled(true);
        connect(_presetSlider.at(i),SIGNAL(valueChanged(int)),this,SLOT(updateSelectedPreset()));

    }

    this->setFixedSize(1024,768);
    _prefixPocketC =  {0xF0,0x00,0x20,0x20,0x14,0x00};
    _midiIn->setIgnoreTypes(false, false, false);

    // Temporary default starting values
    for(int i=0; i<128; i++){
        for(int j=0; j<3; j++){
            for(int k=0; k<16; k++){
                _preset[i][j][k] = ((j == 1) ? 127 : 0);
            }
        }
    }

    // Layout

    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    QSplitter *presetSplitter = new QSplitter(mainWidget);

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

    QHBoxLayout *toolBar = new QHBoxLayout;

    QPushButton *getPresetButton = new QPushButton;
    getPresetButton->setText("Get preset");

    connect(getPresetButton, SIGNAL(clicked(bool)),this,SLOT(sendSingleDumpRequest()));

    QHBoxLayout *presetPins = new QHBoxLayout;

    for(int i=0; i<8; i++)
    {
        _presetSlider.at(i)->setMaximum(1);
        _presetSlider.at(i)->setFixedHeight(50);
        _presetSlider.at(i)->setFixedWidth(20);
        _presetSlider.at(i)->setInvertedAppearance(true);
        presetPins->addWidget(_presetSlider.at(i));
    }

    toolBar->addWidget(getPresetButton);
    toolBar->addLayout(presetPins);

    mainLayout->addLayout(toolBar);

    // Table
    for(int i=0; i<128; i++){
        QString defaultCelString = QString("Preset #%1").arg(i+1);
        _presetsList->addItem(defaultCelString);
    }

    //connect(_presetsList,SIGNAL(itemSelectionChanged()),this,SLOT(updateTable()));
    connect(_presetsList,SIGNAL(itemSelectionChanged()),this,SLOT(updateSliders()));

    QStringList settingsList;
    settingsList << "Channel" << "Type" << "Parameter" << "Description" ;
    _presetSettingsTable->setRowCount(16);
    _presetSettingsTable->setColumnCount(4);
    _presetSettingsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _presetSettingsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _presetSettingsTable->setHorizontalHeaderLabels(settingsList);

    presetSplitter->addWidget(_presetsList);
    presetSplitter->addWidget(_presetSettingsTable);
    presetSplitter->setStretchFactor(1,1);
    mainLayout->addWidget(presetSplitter);

    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);

    connect(_midiIn, SIGNAL(midiMessageReceived(QMidiMessage*)), this, SLOT(onMidiMessageReceive(QMidiMessage*)));
    connect(_presetSettingsTable, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(presetCellClicked(int,int)));
    connect(settings, SIGNAL(triggered(bool)), this, SLOT(openSettingsWindow()));

    _presetsList->setCurrentRow(0);
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
}

void mainWindow::presetCellClicked(int row, int column)
{
    _paramWindow->setWindowModality(Qt::ApplicationModal);
    _paramWindow->setWindowTitle("Parameter");

    _eventWindow->setWindowModality(Qt::ApplicationModal);
    _eventWindow->setWindowTitle("Event");
    if(column == 1)
    {
        int presetNum = _presetsList->currentRow();
        int eventNum = _preset[presetNum][1][row];

        int r = eventNum % 32;
        int c = (eventNum - r) / 32;

        QModelIndex currentEvent = _eventWindow->_eventsTable->model()->index(r,c);
        _eventWindow->_eventsTable->setCurrentIndex(currentEvent);
        _eventWindow->_eventsTable->setFocus();
        _eventWindow->show();
    }

    if(column == 2)
    {
        // TODO : CREATE A PARAM WINDOW
         _paramWindow->show();
    }
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
    for(int i=0; i<3; i++){
        for(int j=0; j<16; j++){
            _preset[presetNum][i][j] = message->getRawMessage().at(9+(i*16+j));
        }
    }
    updateTable();
}

void mainWindow::updateTable()
{

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<16; j++)
        {
            QString value = QString::number(_preset[_presetsList->currentRow()][i][j]);
            if(i==1) value = _eventWindow->_eventsList.at(_preset[_presetsList->currentRow()][i][j]);
            _presetSettingsTable->setItem(j,i,new QTableWidgetItem(value));
        }
    }
}

void mainWindow::updateEventCell(int r, int c)
{
    int eventNum;
    QString eventName;

    eventNum = 32*c + r;
    eventName = _eventWindow->_eventsList.at(eventNum);
    _preset[_presetsList->currentRow()][1][_presetSettingsTable->currentRow()] = eventNum;
    _presetSettingsTable->setItem(_presetSettingsTable->currentRow(),1,new QTableWidgetItem(eventName));
   _eventWindow->close();
}

void mainWindow::updateSelectedPreset()
{
    std::bitset<8> presetNum;
    for(int i=0; i<8; i++)
    {
        presetNum.set(i,_presetSlider.at(i)->value());
    }
    _presetsList->setCurrentRow(presetNum.to_ulong());
    updateTable();
}

void mainWindow::updateSliders()
{
    std::bitset<8> presetNum (_presetsList->currentRow());
    for(int i=0; i<8; i++)
    {
        _presetSlider.at(i)->setValue(presetNum.test(i));
    }
    updateTable();
}

void mainWindow::openMidiPorts()
{
    _midiIn->closePort();
    _midiOut->closePort();
    if(_settingsWindow->_inPortComboBox->currentText() != "None")
    {
        qDebug()<< "Selected MIDI IN : " <<_settingsWindow->_inPortComboBox->currentText();
        _midiIn->openPort(_settingsWindow->_inPortComboBox->currentIndex());
    }
    if(_settingsWindow->_outPortComboBox->currentText() != "None")
    {
        qDebug()<< "Selected MIDI OUT : " <<_settingsWindow->_outPortComboBox->currentText();
        _midiOut->openPort(_settingsWindow->_outPortComboBox->currentIndex());
    }
}

void mainWindow::sendThruMasterChnRequest()
{
    std::vector<unsigned char> rawRequest;
    rawRequest = _prefixPocketC;
    rawRequest.push_back(0x56);
    rawRequest.push_back(0x00);
    rawRequest.push_back(0x00);
    rawRequest.push_back(0xF7);
    if(_midiOut->isPortOpen()) _midiOut->sendRawMessage(rawRequest);
}

void mainWindow::sendSingleDumpRequest()
{
    std::vector<unsigned char> rawRequest;
    rawRequest = _prefixPocketC;
    rawRequest.push_back(0x26);
    rawRequest.push_back(_presetsList->currentRow());
    rawRequest.push_back(0x00);
    rawRequest.push_back(0xF7);
    if(_midiOut->isPortOpen()) _midiOut->sendRawMessage(rawRequest);
}

void mainWindow::sendAllDumpRequest()
{
    _presetsList->setCurrentRow(0);
    for(int i=0; i<_presetsList->count(); i++){
        _presetsList->setCurrentRow(i);
        sendSingleDumpRequest();
        QThread::msleep(200);
    }

}

void mainWindow::sendSingleDump()
{
    std::vector<unsigned char> rawRequest;
    rawRequest = _prefixPocketC;
    rawRequest.push_back(0x20);
    rawRequest.push_back(_presetsList->currentRow());
    rawRequest.push_back(0x00);

    for(int i=0; i<3; i++){
        for(int j=0; j<16; j++){
            int value = _preset[_presetsList->currentRow()][i][j];
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
