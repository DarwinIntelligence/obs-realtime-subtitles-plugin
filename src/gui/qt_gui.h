// gui/qt_gui.h
#ifndef QT_GUI_H
#define QT_GUI_H

#include <QWidget>
#include "ui_main_window.h" // Include the generated header

class SimpleQtApp : public QWidget
{
    Q_OBJECT

public:
    SimpleQtApp(QWidget *parent = 0);

private:
    Ui::MainWindow ui; // Use the generated Ui class
};

// Function to run the Qt application
void run_qt_app();

#endif // QT_GUI_H
