#ifndef GUIDOCK_H
#define GUIDOCK_H

#include <QDockWidget>
#include "PluginManager.h"
#include "MainWindow.h"
#include <ui_GUIDock.h>

class GUIDock : public QDockWidget, Ui_GUIDock {
Q_OBJECT
private:
    PluginManager &plugin_manager;
    MainWindow &main_caption_widget;
    string last_output_line;

public:
    explicit GUIDock(const QString &title, PluginManager &plugin_manager, MainWindow &main_caption_widget);
};


#endif //GUIDOCK_H