#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QMainWindow>
#include <QtWidgets>
#include <QTableWidget>
#include <vector>
#include <bitset>

#include "settingswindow.h"
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

    settingsWindow* _settingsWindow;
    QListWidget* _presetsList;
    QTableWidget* _presetSettingsTable;
    QMenuBar* _menuBar;

    ~mainWindow();

    std::vector<unsigned char> _prefixPocketC;
    QMidiMessage* _midiMessage;
    deviceCommandByte _deviceCommandByte;

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
    void openSettingsWindow();
    void updateDeviceConfig();
    //void updatePresetSettings(std::vector<unsigned int>* presetSettings);
private slots:
    void openMidiPorts();
    void sendThruMasterChnRequest();
    void sendSingleDumpRequest();
    void sendAllDumpRequest();

};

#endif // MAINWINDOW_H
