#ifndef GUIDOCK_H
#define GUIDOCK_H

#include <QDockWidget>
#include <QString>
#include "ui_GUIDock.h"

class GUIDock : public QDockWidget {
Q_OBJECT
public:
    explicit GUIDock(QWidget *parent = nullptr);
    ~GUIDock();
private:
    Ui::GUIDock *ui;
};


#endif //GUIDOCK_H
