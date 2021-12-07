#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <QWidget>
#include "emergent.h"

#define MAX_CAMERAS 2

namespace Ui {
class configure;
}

class configure : public QWidget
{
    Q_OBJECT

public:
    explicit configure(QWidget *parent = 0);
    ~configure();
    void sliderExposure();
    void sliderGain();
    void sliderRoi();
    void leExposure();
    void leGain();
    void leRoi();
    void sendParam();
signals:
    void exposureSignal(int);
    void gainSignal(int);
    void roiSignal(int);
    void timeSignal(int);
    void frameRateSignal(int);
private:
    Ui::configure *ui;
    CEmergentCamera configure_cam[MAX_CAMERAS];
    struct GigEVisionDeviceInfo configure_info[MAX_CAMERAS];

};

#endif // CONFIGURE_H
