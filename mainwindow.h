#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "tripform.h"
#include "lureform.h"
#include "placeform.h"
#include "settingsform.h"
#include "statisticsform.h"
#include "messagedisplay.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow, public MessageDisplay
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual void showErrorMessage(const QString&);
    virtual bool showConfirmationMessage(const QString&);
    virtual EChoices showChoiceMessage(const QString&);
    virtual void showNotificationMessage(const QString&);
    virtual void showStatusMessage(const QString&, bool);
    virtual void closeEvent (QCloseEvent* event);

private:    
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
