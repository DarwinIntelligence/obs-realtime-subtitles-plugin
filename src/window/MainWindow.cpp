#include "MainWindow.h"

MainWindow::MainWindow(): QWidget(), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	setVisible(false);
}

void MainWindow::show_self() {
    show();
    raise();
}

MainWindow::~MainWindow()
{

}
