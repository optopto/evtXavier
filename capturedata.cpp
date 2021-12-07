#include "mainxavierevt.h"
#include "ui_mainxavierevt.h"

void configure_defaults(CEmergentCamera* camera);
int getH(CEmergentCamera* camera);
int getW(CEmergentCamera* camera);
QString dir_save_files;

void mainXavierEvt::worker_thread(CEmergentCamera *cam_p,int frame_to_recv,int cameraWork)
{   unsigned int we1 = 0;
    unsigned int he1 = 0;
    we1 = getW(cam_p);
    he1 = getH(cam_p);
    CEmergentAVIFile videoBuffer;
    videoBuffer.fps=60;
    videoBuffer.codec = EVT_CODEC_NONE;
    videoBuffer.width = we1;
    videoBuffer.height = he1;
    EVT_ERROR err = EVT_SUCCESS;
    bool buffer_used = false, buffer_recd = false;
    CEmergentFrame p_frms[FIXEDBUFFER];
    CEmergentFrame evtFrameRecv[FIXEDBUFFER];
    int frame_count = 0;
    int dropped_frames = 0;
    unsigned short id_prev= 0;
    bool  done_ = false;
    int save_frame = 0;
    QString file_number;
    QString file_number_;
    file_number = QString(dir_save_files + "%1/").arg(cameraWork+1);
    file_number_ = QString(file_number + "my_video_%1.avi").arg(cameraWork+1);
    string file_name = file_number_.toStdString();
    strcpy(videoBuffer.fileName,file_name.c_str());
    EVT_AVIOpen(&videoBuffer);
    int frames = 0;
    int rep = 0;
    for (frames = 0; frames < FIXEDBUFFER; frames++)
    {
        p_frms[frames].size_x = we1;
        p_frms[frames].size_y = he1;
        p_frms[frames].pixel_type = GVSP_PIX_MONO8;
        err = EVT_AllocateFrameBuffer(cam_p, &p_frms[frames], EVT_FRAME_BUFFER_ZERO_COPY);
        if (err){cout<<"Error"<<endl;}
        err = EVT_CameraQueueFrame(cam_p, &p_frms[frames]);
        if (err){cout<<"Error EVT: Camera Queue Frame error uno "<<endl;}
    }
    while ((d1 && d2) == false) {
        if (cameraWork == 0){d1 = true;}
        if (cameraWork == 1 || CAMERAS < 2) {d2 = true;}
    }
    sleep(1);
    err = EVT_CameraExecuteCommand(cam_p, "AcquisitionStart");
    if (err)
    {
        cout<<"Error EVT: Acquisition Start error dos"<<endl;
    }
    while(!done_){
        if (buffer_used){err = EVT_CameraQueueFrame(cam_p, &evtFrameRecv[save_frame-1]);
            if (err){cout<<"Error EVT: Camera Queue Frame error"<<endl;}
            buffer_used = false;}
        err = EVT_CameraGetFrame(cam_p, &evtFrameRecv[save_frame], EVT_INFINITE);
        if (!err){
            if (((evtFrameRecv[save_frame].frame_id) != id_prev + 1) && (frame_count != 0)){
                dropped_frames++;
                buffer_recd = false;
            }
            else{frame_count++; buffer_recd = true;}
        }
        else{dropped_frames++; buffer_recd = false;}
        if (evtFrameRecv[save_frame].frame_id == 65535){id_prev = 0;}
        else{id_prev = evtFrameRecv[save_frame].frame_id;}
        buffer_used = true;
        if (frame_count >= frame_to_recv){done_ = true;}
        if (!buffer_recd) continue;
        if (save_frame == 200-1){
            for (save_frame = 0; save_frame < 200; save_frame++) {
                // char filename[100];
                /* file_number_ = QString(file_number + "%1.tiff").arg((200*rep)+save_frame+1);
                string file_name = file_number_.toStdString();
                strcpy(filename,file_name.c_str());
                EVT_FrameSave(&evtFrameRecv[save_frame], filename, EVT_FILETYPE_TIF, EVT_ALIGN_NONE);*/
                EVT_AVIAppend(&videoBuffer,&evtFrameRecv[save_frame]);
                }
            rep++;
            save_frame = -1;
            buffer_used = false;
        }
        save_frame++;
    }
    EVT_AVIClose(&videoBuffer);
    for (frames = 0; frames < FIXEDBUFFER; frames++){EVT_ReleaseFrameBuffer(cam_p, &p_frms[frames]);}

    while ((d1 || d2 ) == true) {
        if (cameraWork == 0){d1 = false;}
        if (cameraWork == 1 || CAMERAS < 2) {d2 = false;}
    }

    //EVT_CameraClose(cam_p);
    return ;
}



void mainXavierEvt::on_actionCapture_triggered()
{
    int frame_to_recv = time_*frame_rate;
    createQtDir(frame_rate,CAMERAS);
    dir_save_files = exist;
    for(int cam=0 ;cam < CAMERAS; cam++){
        EVT_CameraSetUInt32Param(&camera[cam], "FrameRate", frame_rate);
    }

    int cam = 0;

    //for (int cam = 0; cam < CAMERAS; cam++){
        std::thread capture_(&mainXavierEvt::worker_thread,this,&camera[0],frame_to_recv,cam);
        std::thread capture_2(&mainXavierEvt::worker_thread,this,&camera[1],frame_to_recv,cam+1);
        capture_.join();
        capture_2.join();
    //}

    return;
}
