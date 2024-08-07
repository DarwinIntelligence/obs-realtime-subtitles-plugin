#ifndef GUIDOCK_H
#define GUIDOCK_H

#include <QDockWidget>
#include <QString>
#include <ui_GUIDock.h>

class GUIDock : public QDockWidget, Ui_GUIDock {
Q_OBJECT
public:
    explicit GUIDock();
    ~GUIDock() override;
};


#endif //GUIDOCK_H