#include "mainxavierevt.h"
#include "ui_mainxavierevt.h"

void configure_defaults(CEmergentCamera* camera);
int getH(CEmergentCamera* camera);
int getW(CEmergentCamera* camera);
int getMaxGain(CEmergentCamera* camera);
int getMaxExposure(CEmergentCamera* camera);

mainXavierEvt::mainXavierEvt(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mainXavierEvt)
{
    ui->setupUi(this);
    ui->checkBox->setDisabled(true);
    ui->checkBox2->setDisabled(true);
    setGeometry(700,200,750,650);
    connect(confWin,SIGNAL(exposureSignal(int)),this,SLOT(setExposure(int)));
    connect(confWin,SIGNAL(gainSignal(int)),this,SLOT(setGain(int)));
    connect(confWin,SIGNAL(roiSignal(int)),this,SLOT(setRoi(int)));
    connect(confWin,SIGNAL(timeSignal(int)),this,SLOT(setTime(int)));
    connect(confWin,SIGNAL(frameRateSignal(int)),this,SLOT(setFrameRate(int)));
    connect(ui->actionConfiguration,&QAction::triggered,this,&mainXavierEvt::on_actionConfiguration_triggered);

}

mainXavierEvt::~mainXavierEvt()
{
    delete ui;
}


void mainXavierEvt::on_checkBox_clicked(bool checked)
{
    config = checked;
}

void mainXavierEvt::on_checkBox2_clicked(bool checked)
{
    config_ = checked;
}

void mainXavierEvt::on_actionOpen_triggered()
{
    QMessageBox alertas;
    labelsCam[0] =  ui->cam1Label;
    labelsCam[1] =  ui->cam2Label;
    checkCam[0] = ui->checkBox;
    checkCam[1] = ui->checkBox2;
    int ReturnVal = SUCCESS;
    unsigned int listcam_buf_size = MAX_CAMERAS;
    unsigned int count;
    EVT_ERROR err = EVT_SUCCESS;
    EVT_ListDevices(deviceInfo, &listcam_buf_size, &count);
    CAMERAS = count;
    for (int cam = 0; cam < CAMERAS; cam++){
        EVT_CameraOpen(&camera[cam], &deviceInfo[cam]);
        configure_defaults(&camera[cam]);
        configure_user(&camera[cam], gain, 30, exposure, cam, global_roi_w_1, global_roi_h_1,global_roi_x_1,global_roi_y_1);
        EVT_CameraOpenStream(&camera[cam]);
        checkCam[cam]->setDisabled(false);
        if (ReturnVal != SUCCESS)
        {   alertas.setText("CameraOpenStream: Error\n");
            alertas.exec();
            return; }
        evtFrame[cam].size_x = getW(&camera[cam]);
        evtFrame[cam].size_y = getH(&camera[cam]);
        evtFrame[cam].pixel_type = GVSP_PIX_BAYGB8;
        EVT_AllocateFrameBuffer(&camera[cam], &evtFrame[cam], EVT_FRAME_BUFFER_ZERO_COPY);
        EVT_CameraQueueFrame(&camera[cam], &evtFrame[cam]);
    }

    int frames_recd;
    while(cameraOn)
    {
        char path_[200];
        for(int cam = 0; cam < CAMERAS; cam++){
            EVT_CameraExecuteCommand(&camera[cam], "AcquisitionStart");
        }
        for (int cam = 0; cam < CAMERAS; cam++)
        {
            err = EVT_CameraQueueFrame(&camera[cam], &evtFrame[cam]);
            if (err) {
                printf("EVT_CameraQueueFrame Error!\n");
                break;
            }
            err = EVT_CameraGetFrame(&camera[cam], &evtFrame[cam], EVT_INFINITE);
            if (err) {
                printf("EVT_CameraGetFrame Error!\n");
                break;
            }
            else{frames_recd++;}
            EVT_CameraExecuteCommand(&camera[cam], "AcquisitionStop");
        }
        for (int cam = 0; cam < CAMERAS; cam++){
            sprintf(path_, "/home/nvxv/Documents/Proyecto/stream/imagen_%d.bmp",cam+1);
            EVT_FrameSave(&evtFrame[cam], path_, EVT_FILETYPE_BMP, EVT_ALIGN_NONE);
            path.sprintf("/home/nvxv/Documents/Proyecto/stream/imagen_%d.bmp",cam+1);
            images[cam].load(path);
            images[cam] = images[cam].scaled(labelsCam[cam]->size(),Qt::KeepAspectRatio);
            labelsCam[cam]->setAlignment(Qt::AlignHCenter);
            labelsCam[cam]->setPixmap(QPixmap::fromImage(images[cam]));
        }
        QTime dieTime = QTime::currentTime().addMSecs(1000/60);
        while( QTime::currentTime() < dieTime ){QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );}
    }
    cameraOn = true;
}

void mainXavierEvt::on_actionClose_triggered()
{
    cameraOn = false;
}

void mainXavierEvt::on_actionConfiguration_triggered()
{
    confWin->show();
}


