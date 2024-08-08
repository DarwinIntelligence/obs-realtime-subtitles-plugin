#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <obs.h>
#include <obs.hpp>
#include <obs-module.h>
#include <obs-frontend-api.h>

#include <UIConstants.h>
#include <ui_MainWindow.h>

class MainWindow : public QWidget {
	Q_OBJECT
public:
	explicit MainWindow();
	~MainWindow();

	void show_self();

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
