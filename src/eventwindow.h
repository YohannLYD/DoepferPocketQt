#ifndef EVENTWINDOW_H
#define EVENTWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QTableWidget>

class eventWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit eventWindow(QWidget *parent = 0);
    ~eventWindow();

    QTableWidget* _eventsTable;
    QStringList _eventsList;

signals:

public slots:
    void updatePreset(int row, int col);
};

#endif // EVENTWINDOW_H
