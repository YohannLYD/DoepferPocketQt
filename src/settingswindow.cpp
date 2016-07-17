#include "settingswindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QLabel>
settingsWindow::settingsWindow(QWidget *parent) :
    QMainWindow(parent),
    _midiIn(new QMidiIn),
    _midiOut(new QMidiOut),
    _midiMessage(new QMidiMessage),
    _inPortComboBox(new QComboBox(this)),
    _outPortComboBox(new QComboBox(this))
{
    this->setWindowTitle("Configuration");

    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);

    qDebug()<<"inputs"<<_midiIn->getPorts();
    qDebug()<<"outputs"<<_midiOut->getPorts();

    _inPortComboBox->addItems(_midiIn->getPorts());
    _outPortComboBox->addItems(_midiOut->getPorts());


    QPushButton *bouton = new QPushButton();
    bouton->setText("OK");

    mainLayout->addWidget(new QLabel("Midi IN"));
    mainLayout->addWidget(_inPortComboBox);
    mainLayout->addWidget(new QLabel("Midi OUT"));
    mainLayout->addWidget(_outPortComboBox);
    mainLayout->addWidget(bouton);

    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);



}
