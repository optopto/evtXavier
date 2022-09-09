#include "mainxavierevt.h"
#include "ui_mainxavierevt.h"
#include "globalvar.h"
void configure_defaults(CEmergentCamera* camera);
int getH(CEmergentCamera* camera);
int getW(CEmergentCamera* camera);
int getMaxGain(CEmergentCamera* camera);
int getMaxExposure(CEmergentCamera* camera);
void CallBackFunc(int event, int x, int y, int flags, void* userdata);
int aproxGetRoiX(int roiX);
int aproxGetRoiY(int roiY);
int aproxRoiSlider(int roiSlider);
extern "C" int test(int value);
extern "C" uchar getFrameCam(uchar *Frame,uchar *Frame_2,int buffer);

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
    zc1 = checked;

}

void mainXavierEvt::on_checkBox2_clicked(bool checked)
{
    config_ = checked;
    zc2 = checked;
}

void mainXavierEvt::on_actionOpen_triggered()
{
    roix = roi;
    QMessageBox alertas;
    labelsCam[0] =  ui->cam1Label;
    labelsCam[1] =  ui->cam2Label;
    checkCam[0] = ui->checkBox;
    checkCam[1] = ui->checkBox2;
    uchar mask_1[616*800];
    for(int var = 0; var < 50; var++){widhtOffset[var] = var*16;}
    for(int var_ = 0; var_ < 154; var_++){heightOffset[var_] = var_*4;}
    for(int i = 0; i < 616*800; i++){
        if(i<200000)
        mask_1[i] = 1;
        else
        mask_1[i] = 0;
    }
    int ReturnVal = SUCCESS;
    unsigned int listcam_buf_size = MAX_CAMERAS;
    unsigned int count;
    EVT_ERROR err = EVT_SUCCESS;
    EVT_ListDevices(deviceInfo, &listcam_buf_size, &count);
    CAMERAS = count;
    for (int cam = 0; cam < CAMERAS; cam++){
        ReturnVal = EVT_CameraOpen(&camera[cam], &deviceInfo[cam]);
        configure_defaults(&camera[cam]);
        configure_user(&camera[cam], gain, 2, exposure, cam, global_roi_w_1, global_roi_h_1,global_roi_x_1,global_roi_y_1);
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
    int frames_recd = 0;
    while(cameraOn)
    {
        int buffer = 800*616;
        uchar *imgAux;
        uchar output[buffer];
        Mat cropedImage[2];
        Mat cI[2];
        QString name_dirfps;
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

// Se puede extraer el frame como uchar
        if(frames_recd == 1){
            imgAux = evtFrame[0].imagePtr;
        }
        else{
            output[buffer] = getFrameCam(evtFrame[0].imagePtr,mask_1, buffer);
            imgAux = evtFrame[0].imagePtr;
        }

       // Mat mates = cv::Mat(616, 800, CV_8UC1, output);
       // imwrite("/home/nvxv/Pictures/out/sumatest.bmp",mates);

        imagenGPU.load("/home/nvxv/Pictures/out/sumaImagenes.bmp");
        imagenGPU = imagenGPU.scaled(ui->outLabel->size(),Qt::KeepAspectRatio);
        ui->outLabel->setAlignment(Qt::AlignHCenter);
        ui->outLabel->setPixmap(QPixmap::fromImage(imagenGPU));

        for (int cam = 0; cam < CAMERAS; cam++){
            sprintf(path_, "/home/nvxv/Documents/Proyecto/stream/imagen_%d.bmp",cam+1);
            EVT_FrameSave(&evtFrame[cam], path_, EVT_FILETYPE_BMP, EVT_ALIGN_NONE);
            fullImage[cam] =  imread(path_);
            fullI[cam] = imread(path_);
        }

        if(config){
                    namedWindow("Cam 1 Zoom", 1);
                    setMouseCallback("Cam 1 Zoom",CallBackFunc, NULL);
                    if(x_-roi <= 0 || x_-roi <= 0 || yy_1+2*roi >= 616 || xx_1+2*roi >= 800){ int nothing = 0;
                       }else{rectangle(fullImage[0], r , Scalar(0,0,255),3,8,0);}
                    imshow("Cam 1 Zoom", fullImage[0]);
                    waitKey(30);
        }else if(config_){
                    namedWindow("Cam 2 Zoom", 1);
                    setMouseCallback("Cam 2 Zoom",CallBackFunc, NULL);
                    rectangle(fullImage[1], r_2 , Scalar(0,255,0),3,8,0);
                    imshow("Cam 2 Zoom", fullImage[1]);
                    waitKey(30);
        }else{
            destroyAllWindows();
        }

       // if(x_-roi <= 0 || x_-roi <= 0 || yy_1+2*roi >= 616 || xx_1+2*roi >= 800){
         //          int nn = 0;
           //    }else{ cropedImage[0] = fullI[0](r);}
        cropedImage[0] = fullI[0](r);
        //cropedImage[1] = fullI[1](r_2);

        if(config){
                 if (cropedImage[0].empty()){cout << "No change size" << endl;}else
                 {cI[0] = cropedImage[0];
                     imwrite("/home/nvxv/Documents/Proyecto/stream/imagen_1.bmp", cI[0]);}
        }else if(config_){
                 if (cropedImage[1].empty()){cout << " No change size" << endl;}else
                 {cI[1] = cropedImage[1];
                     imwrite("/home/nvxv/Documents/Proyecto/stream/imagen_2.bmp", cI[1]);}
        }



        for (int cam = 0; cam < CAMERAS; cam++){
           // sprintf(path_, "/home/nvxv/Documents/Proyecto/stream/imagen_%d.bmp",cam+1);
           // EVT_FrameSave(&evtFrame[cam], path_, EVT_FILETYPE_BMP, EVT_ALIGN_NONE);
            path.sprintf("/home/nvxv/Documents/Proyecto/stream/imagen_%d.bmp",cam+1);
            images[cam].load(path);
            images[cam] = images[cam].scaled(labelsCam[cam]->size(),Qt::KeepAspectRatio);
            labelsCam[cam]->setAlignment(Qt::AlignHCenter);
            labelsCam[cam]->setPixmap(QPixmap::fromImage(images[cam]));
        }



        if(instant == true){
            char nameFinal[100];
            int month = QDate::currentDate().month();
            int day = QDate::currentDate().day();
            int hour  = QTime::currentTime().hour();
            int minutes = QTime::currentTime().minute();
            int sec = QTime::currentTime().second();
            name_dirfps = QString("/home/nvxv/Pictures/instantaneas/D%1M%2H%3M%4S%5").arg(day).arg(month).arg(hour).arg(minutes).arg(sec);
            std::ifstream src(path_,std::ios::binary);
            string name_ = name_dirfps.toStdString();
            strcpy(nameFinal,name_.c_str());
            std::ofstream dst(nameFinal, std::ios::binary);
            dst << src.rdbuf();
            instant = false;
        }

        QTime dieTime = QTime::currentTime().addMSecs(1000/30);
        while( QTime::currentTime() < dieTime ){QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );}

        if(optRoi == true){
                    xx_1 = aproxGetRoiX(xx_1);
                    yy_1 = aproxGetRoiY(yy_1);
                    r =  Rect(0,0,0,0);
                    configure_user(&camera[0], gain, 30, exposure, 0, 2*roix, 2*roix,xx_1,yy_1);
                    global_roi_x_1 = xx_1;
                    global_roi_y_1 = yy_1;
                    global_roi_h_1 = 2*roi;
                    global_roi_w_1 = 2*roi;
                    optRoi = false;
                    zoom_cam1 = false;
                    zc1 = false;
                }
        if(optRoi_2 == true){
                    xx_2 = aproxGetRoiX(xx_2);
                    yy_2 = aproxGetRoiY(yy_2);
                    r_2 =  Rect(0,0,0,0);
                    configure_user(&camera[1], gain, 30, exposure, 0, 2*roi_2, 2*roi_2,xx_2,yy_2);
                    global_roi_x_2 = xx_2;
                    global_roi_y_2 = yy_2;
                    global_roi_h_2 = 2*roi_2;
                    global_roi_w_2 = 2*roi_2;
                    optRoi_2 = false;
                    zoom_cam2 = false;
                    zc2 = false;
        }
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


void mainXavierEvt::on_pushButton_clicked()
{
    instant = true;
}


void CallBackFunc(int event, int x, int y, int flags, void* userdata){
    if(event == EVENT_LBUTTONDOWN){
        cout << "ROI (" << x << ", " << y << ")" << endl;
        if(zc1 == true){
            if(x-roix <= 0 || y-roix <= 0 || y+2*roix >= 616 || x+2*roix >= 800){
            int n = 0;
            }else{
                x_ = x;
                y_ = y;
                xx_1 = x_-roix;
                yy_1 = y_-roix;
                r = Rect(xx_1,yy_1,2*roix,2*roix);
            }
        }else if(zc2 == true){
            if(x-roi_2 <= 0 || y-roi_2 <= 0 || y+2*roi_2 >= 616 || x+2*roi_2 >= 800){
                int n_2 = 0;
            }else{
                x_2 = x;
                y_2 = y;
                xx_2 = x_2-roi_2;
                yy_2 = y_2-roi_2;
                r_2 = Rect(xx_2,yy_2,2*roi_2,2*roi_2);
            }
        }
     }
    if(event == EVENT_LBUTTONDBLCLK){
        if(zc1 == true){optRoi = true;}else
            if(zc2 == true){optRoi_2 = true;}
    }
}


int aproxGetRoiX(int roiX){
    int x_4_[2];
    int comparar = 0;
    for(int var = 0; var < 50; var++){
        comparar = widhtOffset[var]-roiX;
        if(comparar > 0){
            x_4_[0] = qFabs(widhtOffset[var-1]-roiX);
            x_4_[1] = qFabs(widhtOffset[var]-roiX);
            if(x_4_[0] > x_4_[1]){roiX = widhtOffset[var];break;
            }else{roiX = widhtOffset[var-1];break;}
        }else if(comparar == 0){roiX = widhtOffset[var];break;}
    }
    return roiX;
}
int aproxGetRoiY(int roiY){
    int y_4_[2];
    int comparar = 0;
    for(int var_ = 0; var_ < 154; var_++){
        comparar = heightOffset[var_]-roiY;
        if(comparar > 0){
            y_4_[0] = qFabs(heightOffset[var_-1]-roiY);
            y_4_[1] = qFabs(heightOffset[var_]-roiY);
            if(y_4_[0] > y_4_[1]){roiY = heightOffset[var_];break;
            }else{roiY = heightOffset[var_-1];break;}
        }else if(comparar == 0){roiY = heightOffset[var_];break;}
    }
    return roiY;
}

int aproxRoiSlider(int roiSlider){
    int x_4_[2];
    int comparar = 0;
    for(int var = 0; var < 50; var++){
        comparar = widhtOffset[var]-roiSlider;
        if(comparar > 0){
            x_4_[0] = qFabs(widhtOffset[var-1]-roiSlider);
            x_4_[1] = qFabs(widhtOffset[var]-roiSlider);
            if(x_4_[0] > x_4_[1]){roiSlider = widhtOffset[var];break;
            }else{roiSlider = widhtOffset[var-1];break;}
        }else if(comparar == 0){roiSlider = widhtOffset[var];break;}
    }
    return roiSlider;
}


void mainXavierEvt::on_pushButton_4_clicked()
{
    int var = test(10);
    cout << var << endl;
}
