#include <GUIDock.h>
#include <ui_GUIDock.h>

GUIDock::GUIDock(const QString &title, PluginManager &plugin_manager, MainWindow &main_caption_widget)
        : QDockWidget(title), plugin_manager(plugin_manager), main_caption_widget(main_caption_widget) {
    setupUi(this);
    setWindowTitle(title);

    setFloating(true);
}