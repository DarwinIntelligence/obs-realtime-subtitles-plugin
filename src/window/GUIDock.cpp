#include "GUIDock.h"
#include <ui_GUIDock.h>

GUIDock::GUIDock() : ui(new Ui::GUIDock)
{
    ui->setupUi(this);
    setWindowTitle("title");
    setFloating(true);
}

GUIDock::~GUIDock()
{

}
