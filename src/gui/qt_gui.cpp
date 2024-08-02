// gui/qt_gui.cpp
#include "qt_gui.h"
#include <QApplication>

SimpleQtApp::SimpleQtApp(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this); // Set up the UI
}

void run_qt_app()
{
    int argc = 0;
    char *argv[] = {};
    QApplication app(argc, argv);

    SimpleQtApp window;
    window.show();

    app.exec();
}
