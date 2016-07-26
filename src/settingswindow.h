#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QComboBox>
#include <QGroupBox>
#include <QCheckBox>
#include <QSpinBox>
#include <bitset>

#include "qmidimessage.h"
#include "qmidiin.h"
#include "qmidiout.h"

class settingsWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit settingsWindow(QWidget *parent = 0);

    ~settingsWindow();

    QMidiMessage* _midiMessage;

    QComboBox *_inPortComboBox;
    QComboBox *_outPortComboBox;

    QCheckBox *_channelThruCheckBox;
    QCheckBox *_sysexThruCheckBox;
    QCheckBox *_realtimeThruCheckBox;
    std::bitset<3> *_thruBitset;
    QSpinBox *_masterChannelSpinBox;

private:
    QMidiIn *_midiIn;
    QMidiOut *_midiOut;

signals:

public slots:
    void onThruCheckboxChange();
};

#endif // SETTINGSWINDOW_H
