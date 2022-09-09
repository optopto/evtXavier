#include "mainxavierevt.h"
#include "ui_mainxavierevt.h"
void mainXavierEvt::setExposure(int value){
    exposure = value;
    configureCamFunc();
}
void mainXavierEvt::setGain(int value){
    gain = value;
    configureCamFunc();
}
void mainXavierEvt::setRoi(int value){
    qDebug() << value;
}

void mainXavierEvt::setTime(int time){
    time_ = time;
    ui->label->setText(QString::number(time_));
}
void mainXavierEvt::setFrameRate(int frameRate){
    frame_rate =  frameRate;
    ui->labelTest->setText(QString::number(frame_rate));
}

void mainXavierEvt::configureCamFunc(){
    QMessageBox alerta_param_i;
    if(cameraOn == true){
        if(config == true){
            configure_user(&camera[0], gain, 2, exposure, 0, global_roi_w_1, global_roi_h_1,global_roi_x_1,global_roi_y_1);
        }
        if(config_ == true){
            configure_user(&camera[1], gain, 2, exposure, 1, global_roi_w_2, global_roi_h_2,global_roi_x_2,global_roi_y_2);
        }
        if(config == false && config_ == false){
            alerta_param_i.setText("Seleccione un parametro valido");
            alerta_param_i.exec();
        }
    }
}
