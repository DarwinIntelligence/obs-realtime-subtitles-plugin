#include "GUIDock.h"

GUIDock::GUIDock(QWidget *parent)
	: QDockWidget(parent), ui(new Ui::GUIDock)
{
    ui->setupUi(this);
	setVisible(false);
	setFloating(true);
}

GUIDock::~GUIDock()
{

}
