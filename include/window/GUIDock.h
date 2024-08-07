#ifndef GUIDOCK_H
#define GUIDOCK_H

#include <QDockWidget>
#include "SourceCaptioner.h"
#include "PluginManager.h"
#include "MainWindow.h"
#include "ui_GUIDock.h"

class GUIDock : public QDockWidget, Ui_GUIDock {
Q_OBJECT
private:
    CaptionPluginManager &plugin_manager;
    MainWindow &main_caption_widget;
    string last_output_line;

    void handle_caption_data_cb(
            shared_ptr<OutputCaptionResult> caption_result,
            bool cleared,
            string recent_caption_text
    );

private slots:

    void on_settingsToolButton_clicked();

public:
    GUIDock(const QString &title, CaptionPluginManager &plugin_manager, MainWindow &main_caption_widget);

};


#endif //GUIDOCK_H