#include "GUIDock.h"

GUIDock::GUIDock() : ui(new Ui::GUIDock)
{
    ui->setupUi(this);
	setVisible(false);
	setFloating(true);
}

GUIDock::~GUIDock()
{

}
