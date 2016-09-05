#include "settingswindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QGroupBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QMessageBox.h>
#include <QLabel>
#include <bitset>
settingsWindow::settingsWindow(QWidget *parent) :
    QMainWindow(parent),
    _midiIn(new QMidiIn),
    _midiOut(new QMidiOut),
    _midiMessage(new QMidiMessage),
    _inPortComboBox(new QComboBox(this)),
    _outPortComboBox(new QComboBox(this)),
    _channelThruCheckBox(new QCheckBox(this)),
    _sysexThruCheckBox(new QCheckBox(this)),
    _realtimeThruCheckBox(new QCheckBox(this)),
    _masterChannelSpinBox(new QSpinBox(this)),
    _thruBitset(new std::bitset<3> )
{
    this->setWindowTitle("Configuration");

    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);

    qDebug()<<"inputs"<<_midiIn->getPorts();
    qDebug()<<"outputs"<<_midiOut->getPorts();

    _inPortComboBox->addItems(_midiIn->getPorts());
    _inPortComboBox->addItem("None");
    _outPortComboBox->addItems(_midiOut->getPorts());
    _outPortComboBox->addItem("None");

    emit _inPortComboBox->setCurrentIndex(_inPortComboBox->count()-1);
    emit _outPortComboBox->setCurrentIndex(_outPortComboBox->count()-1);


    QPushButton *applyBouton = new QPushButton();
    applyBouton->setText("Apply");
    connect(applyBouton, SIGNAL(clicked(bool)), parent, SLOT(openMidiPorts()));


    // THRU MASTER CHANNEL
    QGroupBox *thruChnBox = new QGroupBox("Thru Settings");
    _channelThruCheckBox->setText("Channel Events Thru (BIT0)");
    _sysexThruCheckBox->setText("Sysex Events Thru (BIT1)");
    _realtimeThruCheckBox->setText("Realtim Events Thru (BIT2)");
    _masterChannelSpinBox->setMinimum(1);
    _masterChannelSpinBox->setMaximum(16);
    _masterChannelSpinBox->setValue(1);

    QPushButton *getConfigButton = new QPushButton;
    getConfigButton->setText("Get configuration");
    QPushButton *setConfigButton = new QPushButton;
    setConfigButton->setText("Set configuration");

    connect(getConfigButton, SIGNAL(clicked(bool)),parent,SLOT(sendThruMasterChnRequest()));
    connect(setConfigButton, SIGNAL(clicked(bool)),parent, SLOT(updateDeviceConfig()));

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(_channelThruCheckBox);
    vbox->addWidget(_sysexThruCheckBox);
    vbox->addWidget(_realtimeThruCheckBox);
    vbox->addWidget(_masterChannelSpinBox);
    vbox->addWidget(getConfigButton);
    vbox->addWidget(setConfigButton);
    thruChnBox->setLayout(vbox);
    // END THRU MASTER CHANNEL

    mainLayout->addWidget(new QLabel("Midi IN"));
    mainLayout->addWidget(_inPortComboBox);
    mainLayout->addWidget(new QLabel("Midi OUT"));
    mainLayout->addWidget(_outPortComboBox);
    mainLayout->addWidget(applyBouton);
    mainLayout->addWidget(thruChnBox);

    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);

    connect(_channelThruCheckBox,SIGNAL(toggled(bool)),this,SLOT(onThruCheckboxChange()));
    connect(_sysexThruCheckBox,SIGNAL(toggled(bool)),this,SLOT(onThruCheckboxChange()));
    connect(_realtimeThruCheckBox,SIGNAL(toggled(bool)),this,SLOT(onThruCheckboxChange()));


}

settingsWindow::~settingsWindow()
{
}

void settingsWindow::onThruCheckboxChange(){
    _thruBitset->set(0, _channelThruCheckBox->isChecked());
    _thruBitset->set(1, _sysexThruCheckBox->isChecked());
    _thruBitset->set(2,_realtimeThruCheckBox->isChecked());
}

void settingsWindow::updateConfig(QMidiMessage* message){
    int mc  = message->getRawMessage().at(8)+1;
    _masterChannelSpinBox->setValue(mc);

    std::bitset<3> thruVal (message->getRawMessage().at(7));
    _channelThruCheckBox->setChecked(thruVal.test(0));
    _sysexThruCheckBox->setChecked(thruVal.test(1));
    _realtimeThruCheckBox->setChecked(thruVal.test(2));
}
