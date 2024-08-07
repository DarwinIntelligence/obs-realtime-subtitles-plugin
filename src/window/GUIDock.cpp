#include "GUIDock.h"
#include <QLabel>
#include <QLineEdit>

GUIDock::GUIDock(const QString &title, CaptionPluginManager &plugin_manager, MainWindow &main_caption_widget)
        : QDockWidget(title), plugin_manager(plugin_manager), main_caption_widget(main_caption_widget) {
    setupUi(this);
    setWindowTitle(title);

    setFloating(true);
}