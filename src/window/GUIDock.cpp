#include <GUIDock.h>
#include <ui_GUIDock.h>

GUIDock::GUIDock(const QString &title, PluginManager &manager, MainWindow &caption_widget)
        : QDockWidget(title), plugin_manager(manager), main_caption_widget(caption_widget) {
    setupUi(this);
    setWindowTitle(title);

    setFloating(true);
}