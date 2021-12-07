#ifndef MAINXAVIEREVT_H
#define MAINXAVIEREVT_H

#include <QImage>
#include <QString>
#include <QMessageBox>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QThread>
#include <QtMath>
#include <QtCore>

#include "unistd.h"
#include <thread>

#include "configure.h"


#define MAX_CAMERAS 2
#define BUFFPREVIEW 2
#define SUCCESS 0
#define FIXEDBUFFER 300
#define BUFFCAPTURE 200


namespace Ui {
class mainXavierEvt;
}

class mainXavierEvt : public QMainWindow
{
    Q_OBJECT

public:
    explicit mainXavierEvt(QWidget *parent = 0);
    ~mainXavierEvt();
    void on_actionConfiguration_triggered();
    void createQtDir(int frame_rate, int camM);
    void worker_thread(CEmergentCamera *cam_p,int frame_to_recv,int cameraWork);



public slots:
    void setExposure(int);
    void setGain(int);
    void setRoi(int);
    void setTime(int);
    void setFrameRate(int);

private slots:
    void on_checkBox_clicked(bool checked);
    void on_checkBox2_clicked(bool checked);
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void configure_user(CEmergentCamera *camera,int gain, int frameRate, int exposure, int staticI,int we, int he, int offX, int offY);
    void configureCamFunc();
    void on_actionSave_Directory_triggered();
    void on_actionCapture_triggered();

private:
    Ui::mainXavierEvt *ui;
    CEmergentFrame evtFrame[BUFFPREVIEW];
    struct GigEVisionDeviceInfo deviceInfo[MAX_CAMERAS];
    CEmergentCamera camera[MAX_CAMERAS];
    QString path;
    QImage images[2];
    QLabel *labelsCam[2];
    QCheckBox *checkCam[2];
    int CAMERAS = 0;
    bool cameraOn = true;
    configure *confWin = new configure;
    int gain = 200;
    int exposure = 500;
    int roi = 0;
    int global_roi_x_1 = 0;
    int global_roi_y_1 = 0;
    int global_roi_h_1 = 616;
    int global_roi_w_1 = 800;
    int roi_capture_2 = 0;
    int global_roi_x_2 = 0;
    int global_roi_y_2 = 0;
    int global_roi_h_2 = 616;
    int global_roi_w_2 = 800;
    bool config = false;
    bool config_ = false;
    int maxExposure = 0;
    QStringList save_directory = QStringList("/home/pc/");
    QString exist;
    bool d1 = false;
    bool d2 = false;
    QThread *camThread[2];
    int time_ = 0;
    int frame_rate = 0;

};

#endif // MAINXAVIEREVT_H
