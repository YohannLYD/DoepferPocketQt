#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QComboBox>
#include <QGroupBox>;
#include <QCheckBox>
#include <QSpinBox>

#include "qmidimessage.h"
#include "qmidiin.h"
#include "qmidiout.h"

class settingsWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit settingsWindow(QWidget *parent = 0);

    QMidiMessage* _midiMessage;

    QComboBox *_inPortComboBox;
    QComboBox *_outPortComboBox;

    QCheckBox *_channelThruCheckBox;
    QCheckBox *_sysexThruCheckBox;
    QCheckBox *_realtimeThruCheckBox;
    QSpinBox *_masterChannelSpinBox;

private:
    QMidiIn *_midiIn;
    QMidiOut *_midiOut;

signals:

public slots:
};

#endif // SETTINGSWINDOW_H
