#include <obs-module.h>
#include <plugin-support.h>
#include <obs-source.h>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>

OBS_DECLARE_MODULE();
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US");

OBS_MODULE_AUTHOR("DarwinIntelligence");

void register_gui()
{
    /* UI registration from
     * https://github.com/Palakis/obs-websocket/
     */
    const auto menu_action = static_cast<QAction*>(obs_frontend_add_tools_menu_qaction(T_MENU_TUNA));
    obs_frontend_push_ui_translation(obs_module_get_string);
    const auto main_window = static_cast<QMainWindow*>(obs_frontend_get_main_window());
    tuna_dialog = new tuna_gui(main_window);
    obs_frontend_pop_ui_translation();

    const auto menu_cb = [] { tuna_dialog->toggleShowHide(); };
    QAction::connect(menu_action, &QAction::triggered, menu_cb);
}

bool obs_module_load(void)
{
	obs_log(LOG_INFO, "[OBS RealTime Subtitles] plugin loaded successfully (version %s)",
			PLUGIN_VERSION);
	return true;
}

void obs_module_unload(void)
{
	obs_log(LOG_INFO, "plugin unloaded");
}
