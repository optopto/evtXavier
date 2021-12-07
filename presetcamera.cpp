#include "mainxavierevt.h"
#include "ui_mainxavierevt.h"

char* next_token;
void configure_defaults(CEmergentCamera* camera)
{
    unsigned int param_val_max;
    unsigned int width_max, height_max;
    const unsigned long enumBufferSize = 1000;
    unsigned long enumBufferSizeReturn = 0;
    char enumBuffer[enumBufferSize];
    EVT_CameraGetEnumParamRange(camera, "PixelFormat", enumBuffer, enumBufferSize, &enumBufferSizeReturn);
    char* enumMember = strtok_s(enumBuffer, ",", &next_token);
    EVT_CameraSetEnumParam(camera, "PixelFormat", enumMember);
    EVT_CameraSetUInt32Param(camera, "FrameRate", 30);
    EVT_CameraSetUInt32Param(camera, "OffsetX", 0);
    EVT_CameraSetUInt32Param(camera, "OffsetY", 0);
    EVT_CameraGetUInt32ParamMax(camera, "Width", &width_max);
    EVT_CameraSetUInt32Param(camera, "Width", width_max);
    EVT_CameraGetUInt32ParamMax(camera, "Height", &height_max);
    EVT_CameraSetUInt32Param(camera, "Height", height_max);
    EVT_CameraSetEnumParam(camera, "AcquisitionMode", "MultiFrame");
    EVT_CameraSetUInt32Param(camera, "AcquisitionFrameCount", 200);
    EVT_CameraSetEnumParam(camera, "TriggerSelector", "AcquisitionStart");
    EVT_CameraSetEnumParam(camera, "TriggerMode", "Off");
    EVT_CameraSetEnumParam(camera, "TriggerSource", "Software");
    EVT_CameraSetEnumParam(camera, "BufferMode", "Off");
    EVT_CameraSetUInt32Param(camera, "BufferNum", 0);
    EVT_CameraGetUInt32ParamMax(camera, "GevSCPSPacketSize", &param_val_max);
    EVT_CameraSetUInt32Param(camera, "GevSCPSPacketSize", param_val_max);
    EVT_CameraSetUInt32Param(camera, "Gain", 256);
    EVT_CameraSetUInt32Param(camera, "Offset", 0);
    EVT_CameraSetBoolParam(camera, "LUTEnable", false);
    EVT_CameraSetBoolParam(camera, "AutoGain",false);
}

void mainXavierEvt::configure_user(CEmergentCamera *camera,int gain, int frameRate, int exposure, int staticI,int we, int he, int offX, int offY)
{
    const unsigned long StringSize = 256;
       unsigned long StringSizeReturn = 0;
       char StringBuffer[StringSize];
       char ip[16];
       char friendName[128];
       QString info("Status Camera: ");
       QString info_particular;
       unsigned int frame_rate_max, frame_rate_status;
       unsigned int width_max, height_max;
       int temp;
       info_particular.sprintf("%d: \n", staticI + 1);
       info.append(info_particular);
       EVT_CameraGetInt32Param(camera, "SensTemp", &temp);
       info_particular.sprintf("Temperature: %d C \n", temp);
       info.append(info_particular);
       EVT_CameraGetStringParam(camera, "DeviceVendorName", StringBuffer, StringSize, &StringSizeReturn);
       info_particular.sprintf("Device Name: %s\n", StringBuffer);
       info.append(info_particular);
       EVT_CameraGetStringParam(camera, "DeviceModelName", StringBuffer, StringSize, &StringSizeReturn);
       info_particular.sprintf("Device Model: %s\n", StringBuffer);
       info.append(info_particular);
       EVT_CameraGetStringParam(camera, "DeviceVersion", StringBuffer, StringSize, &StringSizeReturn);
       info_particular.sprintf("Version: %s\n", StringBuffer);
       info.append(info_particular);
       EVT_CameraGetStringParam(camera, "DeviceSerialNumber", StringBuffer, StringSize, &StringSizeReturn);
       info_particular.sprintf("Serial Number: %s\n", StringBuffer);
       info.append(info_particular);
       EVT_CameraGetStringParam(camera, "DeviceFirmwareVersion", StringBuffer, StringSize, &StringSizeReturn);
       info_particular.sprintf("Firmware Version: %s\n", StringBuffer);
       info.append(info_particular);
       EVT_CameraGetUInt32ParamMax(camera, "Height", &height_max);
       EVT_CameraGetUInt32ParamMax(camera, "Width", &width_max);
       info_particular.sprintf("Max Res: %d x %d \n", height_max, width_max);
       info.append(info_particular);
       EVT_CameraSetUInt32Param(camera, "Height", he);
       EVT_CameraSetUInt32Param(camera, "Width", we);
       info_particular.sprintf("Set Res: %d x %d \n", he, we);
       info.append(info_particular);
       EVT_CameraSetUInt32Param(camera, "FrameRate", frameRate);
       EVT_CameraGetUInt32Param(camera, "FrameRate", &frame_rate_status);
       EVT_CameraGetUInt32ParamMax(camera, "FrameRate", &frame_rate_max);
       EVT_CameraSetUInt32Param(camera, "Gain", gain);
       EVT_CameraSetUInt32Param(camera, "Exposure", exposure);
       EVT_CameraSetUInt32Param(camera, "OffsetX", offX);
       EVT_CameraSetUInt32Param(camera, "OffsetY", offY);
       strcpy_s(friendName,deviceInfo[staticI].nic.friendlyName);
       info_particular.sprintf("Network Name: %s\n",friendName);
       info.append(info_particular);
       strcpy_s(ip,deviceInfo[staticI].nic.ip4Address);
       info_particular.sprintf("Ip: %s\n",ip);
       info.append(info_particular);
      // QString max_fps_str =  QString("%1").arg(frame_rate_max);
      // ui->label_fps_max->setText(max_fps_str);
       if (staticI == 0){ui->info1->setText(info);}
       else if (staticI == 1){ui->info2->setText(info);}
   }

int getW(CEmergentCamera* camera) {
    unsigned int width;
    EVT_CameraGetUInt32Param(camera, "Width", &width);
    return width;
}

int getH(CEmergentCamera* camera) {
    unsigned int heigth;
    EVT_CameraGetUInt32Param(camera, "Height", &heigth);
    return heigth;
}

int getMaxGain(CEmergentCamera* camera){
    unsigned int param_val_max;
    EVT_CameraGetUInt32ParamMax(camera, "Gain", &param_val_max);
    return param_val_max;
}

int getMaxExposure(CEmergentCamera* camera){
    unsigned int param_val_max;
    EVT_CameraGetUInt32ParamMax(camera, "Exposure", &param_val_max);
    return param_val_max;
}
