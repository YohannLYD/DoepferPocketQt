#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QMainWindow>
#include <QtWidgets>
#include <QTableWidget>
class mainWindow : public QMainWindow
{
    Q_OBJECT
public:
    mainWindow(QWidget *parent = 0);
    QTableWidget* _presets;
    QTableWidget* _presetSettings;

    ~mainWindow();
signals:

public slots:
};

#endif // MAINWINDOW_H
