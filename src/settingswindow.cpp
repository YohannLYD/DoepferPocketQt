#include "settingswindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QGroupBox>
#include <QCheckBox>
#include <QSpinBox>
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
    _masterChannelSpinBox(new QSpinBox(this))
{
    this->setWindowTitle("Configuration");

    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);

    qDebug()<<"inputs"<<_midiIn->getPorts();
    qDebug()<<"outputs"<<_midiOut->getPorts();

    _inPortComboBox->addItems(_midiIn->getPorts());
    _outPortComboBox->addItems(_midiOut->getPorts());


    QPushButton *bouton = new QPushButton();
    bouton->setText("Apply");

    // THRU MASTER CHANNEL
    QGroupBox *thruChnBox = new QGroupBox("Thru Settings");
    _channelThruCheckBox->setText("Channel Events Thru (BIT0)");
    _sysexThruCheckBox->setText("Sysex Events Thru (BIT1)");
    _realtimeThruCheckBox->setText("Realtim Events Thru (BIT2)");

    _masterChannelSpinBox->setValue(1);

    QPushButton *getConfigButton = new QPushButton;
    getConfigButton->setText("Get configuration");
    QPushButton *setConfigButton = new QPushButton;
    setConfigButton->setText("Set configuration");

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
    mainLayout->addWidget(bouton);
    mainLayout->addWidget(thruChnBox);

    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);




}
