#include "configure.h"
#include "ui_configure.h"
int getMaxGain(CEmergentCamera* camera);
int getMaxExposure(CEmergentCamera* camera);
configure::configure(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::configure)
{
    ui->setupUi(this);
    setWindowTitle("Configure");
    setGeometry(1200,400,234,326);
    unsigned int listcam_buf_size = MAX_CAMERAS;
    unsigned int count;
    EVT_ListDevices(configure_info, &listcam_buf_size, &count);
    if(count > 0){
        EVT_CameraOpen(&configure_cam[0], &configure_info[0]);
        ui->exposureSlider->setMaximum(getMaxExposure(&configure_cam[0]));
        ui->gainSlider->setMaximum(getMaxGain(&configure_cam[0]));}
    else if(count == 0){
        ui->exposureSlider->setMaximum(30000);
        ui->gainSlider->setMaximum(8000);
        std::cout << "NO HAY CAMARAS CONECTADAS" << std::endl;
    }
    ui->exposureSlider->setValue(1000);
    ui->gainSlider->setValue(300);
    EVT_CameraClose(&configure_cam[0]);
    connect(ui->exposureSlider,&QSlider::sliderMoved,this,&configure::sliderExposure);
    connect(ui->gainSlider,&QSlider::sliderMoved,this,&configure::sliderGain);
    connect(ui->roiSlider,&QSlider::sliderMoved,this,&configure::sliderRoi);
    connect(ui->leExposure,&QLineEdit::returnPressed,this,&configure::leExposure);
    connect(ui->leGain,&QLineEdit::returnPressed,this,&configure::leGain);
    connect(ui->leRoi,&QLineEdit::returnPressed,this,&configure::leRoi);
    connect(ui->sendParam,&QPushButton::clicked,this,&configure::sendParam);
}

configure::~configure()
{
    delete ui;
}

void configure::sliderExposure(){
    int exposure = ui->exposureSlider->value();
    emit exposureSignal(exposure);
    ui->leExposure->setText(QString::number(exposure));
}
void configure::sliderGain(){
    int gain = ui->gainSlider->value();
    emit gainSignal(gain);
    ui->leGain->setText(QString::number(gain));
}
void configure::sliderRoi(){
    int roi = ui->roiSlider->value();
    emit roiSignal(roi);
    ui->leRoi->setText(QString::number(roi));
}

void configure::leExposure(){
    int exposure = ui->leExposure->text().toInt();
    emit exposureSignal(exposure);
    ui->exposureSlider->setValue(exposure);
}

void configure::leGain(){
    int gain = ui->leGain->text().toInt();
    emit gainSignal(gain);
    ui->gainSlider->setValue(gain);
}

void configure::leRoi(){
    int roi = ui->leRoi->text().toInt();
    emit roiSignal(roi);
    ui->roiSlider->setValue(roi);
}
void configure::sendParam(){
    int time =ui->leTime->text().toInt();
    int frameR =ui->leFrameR->text().toInt();
    emit timeSignal(time);
    emit frameRateSignal(frameR);
}


