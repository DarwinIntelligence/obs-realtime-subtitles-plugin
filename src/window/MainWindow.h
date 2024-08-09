#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <obs.h>
#include <obs.hpp>
#include <obs-module.h>
#include <obs-frontend-api.h>

#include <PluginSupport.h>
#include <UIConstants.h>
#include <PluginSettings.h>
#include <ui_MainWindow.h>

class MainWindow : public QDialog {
	Q_OBJECT
public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	void show_self();

private:
	Ui::MainWindow *ui;
	PluginSettings *plugin_settings_data;

	void setupUIText();
	void ConnectUISignalHandlers();

private slots:

	void HandleAudioSourceChange(QString new_text);
	void HandleSourceLanguageChange(QString new_text);
	void HandleTranslateLanguageChange(QString new_text);
	void SaveButtonClicked();
};

#endif // MAINWINDOW_H
