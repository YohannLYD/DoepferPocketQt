#ifndef PARAMWINDOW_H
#define PARAMWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QTableWidget>

class paramWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit paramWindow(QWidget *parent = 0);
    ~paramWindow();

    QTableWidget* _paramNumsTable;

signals:

public slots:
};

#endif // PARAMWINDOW_H
