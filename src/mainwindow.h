#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QMainWindow>
#include <QtWidgets>
#include <QTableWidget>
#include <vector>
#include <bitset>

#include "qmidimessage.h"
#include "qmidiin.h"
#include "qmidiout.h"

enum deviceCommandByte {

    SINGLE_DUMP            = 0x20,
    SINGLE_STORE           = 0x30,
    SINGLE_STORE_REQ       = 0x26,
    THRU_MC_DUMP           = 0x50,
    THRU_MC_DUMP_REQ       = 0x56,
    VERSION_REQ            = 0x06,
    VERSION_ANS            = 0x00,
};


class mainWindow : public QMainWindow
{
    Q_OBJECT
public:
    mainWindow(QWidget *parent = 0);
    QTableWidget* _presetsTable;
    QTableWidget* _presetSettingsTable;

    ~mainWindow();

    QMidiMessage* _midiMessage;
    deviceCommandByte _deviceCommandByte;

    int _currentPreset;
    int _masterChannel;
    std::vector<unsigned int> _presetSettings;

private:
    QMidiIn *_midiIn;
    QMidiOut *_midiOut;


signals:
    void singleDumpReceived(QMidiMessage* message);
    void thruMasterChnDumpReceived(QMidiMessage* message);
    void versionAnswerReceived(QMidiMessage* message);

public slots:
    void onMidiMessageReceive(QMidiMessage* message);
    //void updateDeviceConfig(std::bitset<3> thru, int mastChn);
    //void updatePresetSettings(std::vector<unsigned int>* presetSettings);

};

#endif // MAINWINDOW_H
