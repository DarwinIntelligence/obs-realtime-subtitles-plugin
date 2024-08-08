#include "GUIDock.h"

GUIDock::GUIDock() : ui(new Ui::GUIDock)
{
    ui->setupUi(this);
    setFloating(true);
}

GUIDock::~GUIDock()
{

}
