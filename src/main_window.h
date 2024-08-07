#ifndef COUNTDOWNWIDGET_H
#define COUNTDOWNWIDGET_H

#include <QMainWindow>
#include <QDockWidget>
#include <QEvent>
#include <QLabel>
#include <QPushButton>
#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QLCDNumber>
#include <QDateTimeEdit>
#include <QRegularExpression>
#include <QValidator>
#include <QTimer>
#include <QTime>
#include <QChar>
#include <QIcon>
#include <QGroupBox>
#include <QCheckBox>
#include <QTabWidget>
#include <Qt>

#include <string>
#include <iostream>
#include <chrono>
#include <list>
#include <util/base.h>
#include <util/platform.h>
#include <util/config-file.h>
#include <vector>
#include <obs.h>
#include <obs.hpp>
#include <obs-frontend-api.h>
#include <obs-module.h>
#include <obs-websocket-api.h>

#include "ui_RealTimeSubtitles.h"

#define CONFIG "config.json"

class MainWidget : public QWidget {
	Q_OBJECT
public:
	explicit MainWidget();
	~MainWidget();

	void show_self();

private:
	Ui::RealTimeSubtitles *ui;
};

#endif // COUNTDOWNWIDGET_H
