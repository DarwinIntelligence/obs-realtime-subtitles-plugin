#include "main_window.h"

CountdownDockWidget::CountdownDockWidget(): QWidget(), ui(new Ui::CountdownTimer)
{
	ui->setupUi(this);

	SetupCountdownWidgetUI(countdownTimerData);

	setVisible(false);
}

CountdownDockWidget::~CountdownDockWidget()
{

}