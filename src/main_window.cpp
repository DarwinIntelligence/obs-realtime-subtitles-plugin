#include "main_window.h"

MainWidget::MainWidget(): QWidget(), ui(new Ui::RealTimeSubtitles)
{
	ui->setupUi(this);

	setVisible(false);
}

void MainWidget::show_self() {
    show();
    raise();
}

MainWidget::~MainWidget()
{

}