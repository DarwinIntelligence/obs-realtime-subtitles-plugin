#include "MainWindow.h"


MainWindow::MainWindow(): QWidget(), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	setVisible(false);
    setWindowTitle(QString::fromUtf8(T_WINDOW_TITLE));
    ui->audio_source_label->setText()
}

void MainWindow::show_self()
{
    show();
    raise();
}

MainWindow::~MainWindow()
{

}
