#include "camerawindow.h"
#include "ui_camerawindow.h"
#include <cv.h>
#include <highgui.h>
#include <QSettings>
#include <QDebug>
#include <QtCore>
#include <QFuture>
#include <QtConcurrent>
#include <iostream>

#define M_PI 3.14159265358979323846

camerawindow::camerawindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::camerawindow)
{
    ui->setupUi(this);
}

camerawindow::~camerawindow()
{
    delete ui;
}



void camerawindow::on_pushButton_clicked()
{
    QSettings settings("JoshCrawleySoft", "3D_Scanner");
    std::string name1 = "Video1";  // We pass this to the thread creator to see what's what
    QString a = (settings.value("Video/vDevice1" , -1)).toString();  //Get variable from file
    int vidDevice1 = a.right(1).toInt();  //Convert QString to integer to feed to VideoCapture
    int x1 = settings.value("Video/vXAxis1").toInt(); //Obvious X is X
    int y1 = settings.value("Video/vYAxis1").toInt(); //Obvious Y is Y
    
    if( ui->calib1->isChecked() == true){

        cvNamedWindow("Calibrated VidWindow 1");
        QString iFile = settings.value("Calibration/fileStorage").toString() + "/Intrinsics1.xml";
        QString dFile = settings.value("Calibration/fileStorage").toString() + "/Distortion1.xml";
        QByteArray iname = iFile.toLocal8Bit();
        QByteArray dname = dFile.toLocal8Bit();
        const char *intrinsicName = iname.data();
        const char *distortionName = dname.data();
        CvMat *intrinsic = (CvMat*)cvLoad( intrinsicName );
        CvMat *distortion = (CvMat*)cvLoad( distortionName ); //Above sets up standard variables for this section

        CvCapture* capture = cvCreateCameraCapture( vidDevice1 );
        assert( capture != NULL );
        cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, x1 );
        cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, y1 );

        IplImage *frame1 = cvQueryFrame( capture );

        IplImage* mapx = cvCreateImage( cvGetSize( frame1 ), IPL_DEPTH_32F, 1 ); //Calculate transients X
        IplImage* mapy = cvCreateImage( cvGetSize( frame1 ), IPL_DEPTH_32F, 1 ); //Calculate transients Y
        cvInitUndistortMap( intrinsic, distortion, mapx, mapy ); //precomputes calib matrix

        while ( frame1 ) {
            IplImage *temp = cvCloneImage( frame1 );
            cvRemap( temp, frame1, mapx, mapy );
            cvReleaseImage( &temp );

            cv::Mat old;

            cv::Mat channel[3];
            cv::split(frame1, channel);
            if(ui->cam1B->isChecked() == true) {
                channel[0]=cv::Mat::zeros(frame1->height, frame1->width, CV_8UC1); //BLUE!!!!!!
            }
            if(ui->cam1G->isChecked() == true) {
                channel[1]=cv::Mat::zeros(frame1->height, frame1->width, CV_8UC1); //GREEN!!!!!!
            }
            if(ui->cam1R->isChecked() == true) {
                channel[2]=cv::Mat::zeros(frame1->height, frame1->width, CV_8UC1); //RED!!!!!!
            }
            cv::merge(channel, 3, old);
            IplImage newer = old;
            IplImage* new_image = &newer;

            cvShowImage( "Calibrated VidWindow 1", new_image );
            int c = cvWaitKey( 15 );
            if( c == 'p' ){
                        c = 0;
                        while( c != 'p' && c != 27 ){
                            c = cvWaitKey( 250 );
                        }
                    }
                    if( c == 27 ){
                        cvDestroyWindow("Calibrated VidWindow 1");
                        cvReleaseCapture(&capture);
                        break;}
            frame1 = cvQueryFrame( capture );

        }
    }
    else {
        cv::namedWindow(name1);
        cv::VideoCapture cap1 = cv::VideoCapture(vidDevice1);
        cap1.set(CV_CAP_PROP_FRAME_WIDTH, x1 );
        cap1.set(CV_CAP_PROP_FRAME_HEIGHT, y1 );
        cv::Mat frame1;
        cv::Mat channel[3];
        do{
            cap1 >> frame1;
            cv::split(frame1, channel);
            if(ui->cam1B->isChecked() == true) {
                channel[0]=cv::Mat::zeros(frame1.rows, frame1.cols, CV_8UC1); //BLUE!!!!!!
            }
            if(ui->cam1G->isChecked() == true) {
                channel[1]=cv::Mat::zeros(frame1.rows, frame1.cols, CV_8UC1); //GREEN!!!!!!
            }
            if(ui->cam1R->isChecked() == true) {
                channel[2]=cv::Mat::zeros(frame1.rows, frame1.cols, CV_8UC1); //RED!!!!!!
            }
            cv::merge(channel, 3, frame1);
            imshow(name1 ,frame1);
            //cap1.release();
        }while(cv::waitKey(30)<0);
        cv::destroyWindow(name1);
        }
}


void camerawindow::on_pushButton_2_clicked()
{
    QSettings settings("JoshCrawleySoft", "3D_Scanner");
    std::string name2 = "Video2";  // We pass this to the thread creator to see what's what
    QString a = (settings.value("Video/vDevice2" , -1)).toString();  //Get variable from file
    int vidDevice2 = a.right(1).toInt();  //Convert QString to integer to feed to VideoCapture
    int x2 = settings.value("Video/vXAxis2").toInt(); //Obvious X is X
    int y2 = settings.value("Video/vYAxis2").toInt(); //Obvious Y is Y

    if( ui->calib2->isChecked() == true){

        cvNamedWindow("Calibrated VidWindow 2");
        QString iFile = settings.value("Calibration/fileStorage").toString() + "/Intrinsics2.xml";
        QString dFile = settings.value("Calibration/fileStorage").toString() + "/Distortion2.xml";
        QByteArray iname = iFile.toLocal8Bit();
        QByteArray dname = dFile.toLocal8Bit();
        const char *intrinsicName = iname.data();
        const char *distortionName = dname.data();
        CvMat *intrinsic = (CvMat*)cvLoad( intrinsicName );
        CvMat *distortion = (CvMat*)cvLoad( distortionName ); //Above sets up standard variables for this section

        CvCapture* capture = cvCreateCameraCapture( vidDevice2 );
        assert( capture != NULL );
        cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, x2 );
        cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, y2 );

        IplImage *frame2 = cvQueryFrame( capture );

        IplImage* mapx = cvCreateImage( cvGetSize( frame2 ), IPL_DEPTH_32F, 1 ); //Calculate transients X
        IplImage* mapy = cvCreateImage( cvGetSize( frame2 ), IPL_DEPTH_32F, 1 ); //Calculate transients Y
        cvInitUndistortMap( intrinsic, distortion, mapx, mapy ); //precomputes calib matrix

        while ( frame2 ) {
            IplImage *temp = cvCloneImage( frame2 );
            cvRemap( temp, frame2, mapx, mapy );
            cvReleaseImage( &temp );

            cv::Mat old;

            cv::Mat channel[3];
            cv::split(frame2, channel);
            if(ui->cam2B->isChecked() == true) {
                channel[0]=cv::Mat::zeros(frame2->height, frame2->width, CV_8UC1); //BLUE!!!!!!
            }
            if(ui->cam2G->isChecked() == true) {
                channel[1]=cv::Mat::zeros(frame2->height, frame2->width, CV_8UC1); //GREEN!!!!!!
            }
            if(ui->cam2R->isChecked() == true) {
                channel[2]=cv::Mat::zeros(frame2->height, frame2->width, CV_8UC1); //RED!!!!!!
            }
            cv::merge(channel, 3, old);
            IplImage newer = old;
            IplImage* new_image = &newer;

            cvShowImage( "Calibrated VidWindow 2", new_image );
            int c = cvWaitKey( 15 );
            if( c == 'p' ){
                        c = 0;
                        while( c != 'p' && c != 27 ){
                            c = cvWaitKey( 250 );
                        }
                    }
                    if( c == 27 ){
                        cvDestroyWindow("Calibrated VidWindow 2");
                        cvReleaseCapture(&capture);
                        break;}
            frame2 = cvQueryFrame( capture );

        }
    }
    else {
        cv::namedWindow(name2);
        cv::VideoCapture cap2 = cv::VideoCapture(vidDevice2);
        cap2.set(CV_CAP_PROP_FRAME_WIDTH, x2 );
        cap2.set(CV_CAP_PROP_FRAME_HEIGHT, y2 );
        cv::Mat frame2;
        cv::Mat channel[3];
        do{
            cap2 >> frame2;
            cv::split(frame2, channel);
            if(ui->cam2B->isChecked() == true) {
                channel[0]=cv::Mat::zeros(frame2.rows, frame2.cols, CV_8UC1); //BLUE!!!!!!
            }
            if(ui->cam2G->isChecked() == true) {
                channel[1]=cv::Mat::zeros(frame2.rows, frame2.cols, CV_8UC1); //GREEN!!!!!!
            }
            if(ui->cam2R->isChecked() == true) {
                channel[2]=cv::Mat::zeros(frame2.rows, frame2.cols, CV_8UC1); //RED!!!!!!
            }
            cv::merge(channel, 3, frame2);
            imshow(name2 ,frame2);
            //cap1.release();
        }while(cv::waitKey(30)<0);
        cv::destroyWindow(name2);
        }
}
void camerawindow::on_pushButton_3_clicked()
{
    QSettings settings("JoshCrawleySoft", "3D_Scanner");
    std::string name3 = "Video3";  // We pass this to the thread creator to see what's what
    QString a = (settings.value("Video/vDevice3" , -1)).toString();  //Get variable from file
    int vidDevice3 = a.right(1).toInt();  //Convert QString to integer to feed to VideoCapture
    int x3 = settings.value("Video/vXAxis3").toInt(); //Obvious X is X
    int y3 = settings.value("Video/vYAxis3").toInt(); //Obvious Y is Y
    if( ui->calib3->isChecked() == true){

        cvNamedWindow("Calibrated VidWindow 3");
        QString iFile = settings.value("Calibration/fileStorage").toString() + "/Intrinsics3.xml";
        QString dFile = settings.value("Calibration/fileStorage").toString() + "/Distortion3.xml";
        QByteArray iname = iFile.toLocal8Bit();
        QByteArray dname = dFile.toLocal8Bit();
        const char *intrinsicName = iname.data();
        const char *distortionName = dname.data();
        CvMat *intrinsic = (CvMat*)cvLoad( intrinsicName );
        CvMat *distortion = (CvMat*)cvLoad( distortionName ); //Above sets up standard variables for this section

        CvCapture* capture = cvCreateCameraCapture( vidDevice3 );
        assert( capture != NULL );
        cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, x3 );
        cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, y3 );

        IplImage *frame3 = cvQueryFrame( capture );

        IplImage* mapx = cvCreateImage( cvGetSize( frame3 ), IPL_DEPTH_32F, 1 ); //Calculate transients X
        IplImage* mapy = cvCreateImage( cvGetSize( frame3 ), IPL_DEPTH_32F, 1 ); //Calculate transients Y
        cvInitUndistortMap( intrinsic, distortion, mapx, mapy ); //precomputes calib matrix

        while ( frame3 ) {
            IplImage *temp = cvCloneImage( frame3 );
            cvRemap( temp, frame3, mapx, mapy );
            cvReleaseImage( &temp );

            cv::Mat old;

            cv::Mat channel[3];
            cv::split(frame3, channel);
            if(ui->cam3B->isChecked() == true) {
                channel[0]=cv::Mat::zeros(frame3->height, frame3->width, CV_8UC1); //BLUE!!!!!!
            }
            if(ui->cam3G->isChecked() == true) {
                channel[1]=cv::Mat::zeros(frame3->height, frame3->width, CV_8UC1); //GREEN!!!!!!
            }
            if(ui->cam3R->isChecked() == true) {
                channel[2]=cv::Mat::zeros(frame3->height, frame3->width, CV_8UC1); //RED!!!!!!
            }
            cv::merge(channel, 3, old);
            IplImage newer = old;
            IplImage* new_image = &newer;

            cvShowImage( "Calibrated VidWindow 3", new_image );
            int c = cvWaitKey( 15 );
            if( c == 'p' ){
                        c = 0;
                        while( c != 'p' && c != 27 ){
                            c = cvWaitKey( 250 );
                        }
                    }
                    if( c == 27 ){
                        cvDestroyWindow("Calibrated VidWindow 3");
                        cvReleaseCapture(&capture);
                        break;}
            frame3 = cvQueryFrame( capture );

        }
    }
    else {
        cv::namedWindow(name3);
        cv::VideoCapture cap3 = cv::VideoCapture(vidDevice3);
        cap3.set(CV_CAP_PROP_FRAME_WIDTH, x3 );
        cap3.set(CV_CAP_PROP_FRAME_HEIGHT, y3 );
        cv::Mat frame3;
        cv::Mat channel[3];
        do{
            cap3 >> frame3;
            cv::split(frame3, channel);
            if(ui->cam3B->isChecked() == true) {
                channel[0]=cv::Mat::zeros(frame3.rows, frame3.cols, CV_8UC1); //BLUE!!!!!!
            }
            if(ui->cam3G->isChecked() == true) {
                channel[1]=cv::Mat::zeros(frame3.rows, frame3.cols, CV_8UC1); //GREEN!!!!!!
            }
            if(ui->cam3R->isChecked() == true) {
                channel[2]=cv::Mat::zeros(frame3.rows, frame3.cols, CV_8UC1); //RED!!!!!!
            }
            cv::merge(channel, 3, frame3);
            imshow(name3 ,frame3);
            //cap1.release();
        }while(cv::waitKey(30)<0);
        cv::destroyWindow(name3);
        }
}

void camerawindow::on_pushButton_4_clicked()
{
    QSettings settings("JoshCrawleySoft", "3D_Scanner");
    std::string name4 = "Video4";  // We pass this to the thread creator to see what's what
    QString a = (settings.value("Video/vDevice4" , -1)).toString();  //Get variable from file
    int vidDevice4 = a.right(1).toInt();  //Convert QString to integer to feed to VideoCapture
    int x4 = settings.value("Video/vXAxis4").toInt(); //Obvious X is X
    int y4 = settings.value("Video/vYAxis4").toInt(); //Obvious Y is Y
    if( ui->calib4->isChecked() == true){

        cvNamedWindow("Calibrated VidWindow 4");
        QString iFile = settings.value("Calibration/fileStorage").toString() + "/Intrinsics4.xml";
        QString dFile = settings.value("Calibration/fileStorage").toString() + "/Distortion4.xml";
        QByteArray iname = iFile.toLocal8Bit();
        QByteArray dname = dFile.toLocal8Bit();
        const char *intrinsicName = iname.data();
        const char *distortionName = dname.data();
        CvMat *intrinsic = (CvMat*)cvLoad( intrinsicName );
        CvMat *distortion = (CvMat*)cvLoad( distortionName ); //Above sets up standard variables for this section

        CvCapture* capture = cvCreateCameraCapture( vidDevice4 );
        assert( capture != NULL );
        cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, x4 );
        cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, y4 );

        IplImage *frame4 = cvQueryFrame( capture );

        IplImage* mapx = cvCreateImage( cvGetSize( frame4 ), IPL_DEPTH_32F, 1 ); //Calculate transients X
        IplImage* mapy = cvCreateImage( cvGetSize( frame4 ), IPL_DEPTH_32F, 1 ); //Calculate transients Y
        cvInitUndistortMap( intrinsic, distortion, mapx, mapy ); //precomputes calib matrix

        while ( frame4 ) {
            IplImage *temp = cvCloneImage( frame4 );
            cvRemap( temp, frame4, mapx, mapy );
            cvReleaseImage( &temp );

            cv::Mat old;

            cv::Mat channel[3];
            cv::split(frame4, channel);
            if(ui->cam4B->isChecked() == true) {
                channel[0]=cv::Mat::zeros(frame4->height, frame4->width, CV_8UC1); //BLUE!!!!!!
            }
            if(ui->cam4G->isChecked() == true) {
                channel[1]=cv::Mat::zeros(frame4->height, frame4->width, CV_8UC1); //GREEN!!!!!!
            }
            if(ui->cam4R->isChecked() == true) {
                channel[2]=cv::Mat::zeros(frame4->height, frame4->width, CV_8UC1); //RED!!!!!!
            }
            cv::merge(channel, 3, old);
            IplImage newer = old;
            IplImage* new_image = &newer;

            cvShowImage( "Calibrated VidWindow 4", new_image );
            int c = cvWaitKey( 15 );
            if( c == 'p' ){
                        c = 0;
                        while( c != 'p' && c != 27 ){
                            c = cvWaitKey( 250 );
                        }
                    }
                    if( c == 27 ){
                        cvDestroyWindow("Calibrated VidWindow 4");
                        cvReleaseCapture(&capture);
                        break;}
            frame4 = cvQueryFrame( capture );

        }
    }
    else {
        cv::namedWindow(name4);
        cv::VideoCapture cap4 = cv::VideoCapture(vidDevice4);
        cap4.set(CV_CAP_PROP_FRAME_WIDTH, x4 );
        cap4.set(CV_CAP_PROP_FRAME_HEIGHT, y4 );
        cv::Mat frame4;
        cv::Mat channel[3];
        do{
            cap4 >> frame4;
            cv::split(frame4, channel);
            if(ui->cam4B->isChecked() == true) {
                channel[0]=cv::Mat::zeros(frame4.rows, frame4.cols, CV_8UC1); //BLUE!!!!!!
            }
            if(ui->cam4G->isChecked() == true) {
                channel[1]=cv::Mat::zeros(frame4.rows, frame4.cols, CV_8UC1); //GREEN!!!!!!
            }
            if(ui->cam4R->isChecked() == true) {
                channel[2]=cv::Mat::zeros(frame4.rows, frame4.cols, CV_8UC1); //RED!!!!!!
            }
            cv::merge(channel, 3, frame4);
            imshow(name4 ,frame4);
            //cap1.release();
        }while(cv::waitKey(30)<0);
        cv::destroyWindow(name4);
        }
}

void camerawindow::on_pushButton_5_clicked()
{
    std::string name5 = "Video5";
    cv::namedWindow(name5);
    QSettings settings("JoshCrawleySoft", "3D_Scanner");
    QString videoStreamAddress = settings.value("Video/vURL").toString();
    std::string vSA = videoStreamAddress.toStdString();
    int x5 = settings.value("Video/vXAxis5").toInt(); //Obvious X is X
    int y5 = settings.value("Video/vYAxis5").toInt(); //Obvious Y is Y
    if( ui->calib5->isChecked() == true){

        cvNamedWindow("Calibrated VidWindow 5");
        QString iFile = settings.value("Calibration/fileStorage").toString() + "/Intrinsics5.xml";
        QString dFile = settings.value("Calibration/fileStorage").toString() + "/Distortion5.xml";
        QByteArray iname = iFile.toLocal8Bit();
        QByteArray dname = dFile.toLocal8Bit();
        const char *intrinsicName = iname.data();
        const char *distortionName = dname.data();
        CvMat *intrinsic = (CvMat*)cvLoad( intrinsicName );
        CvMat *distortion = (CvMat*)cvLoad( distortionName ); //Above sets up standard variables for this section

        CvCapture* capture = cvCaptureFromFile( videoStreamAddress.toStdString().c_str() );
        cv::waitKey(30);
        //assert( capture != NULL );
        cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, x5 );
        cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, y5 );

        IplImage *frame5 = cvQueryFrame( capture );

        IplImage* mapx = cvCreateImage( cvGetSize( frame5 ), IPL_DEPTH_32F, 1 ); //Calculate transients X
        IplImage* mapy = cvCreateImage( cvGetSize( frame5 ), IPL_DEPTH_32F, 1 ); //Calculate transients Y
        cvInitUndistortMap( intrinsic, distortion, mapx, mapy ); //precomputes calib matrix

        while ( frame5 ) {
            IplImage *temp = cvCloneImage( frame5 );
            cvRemap( temp, frame5, mapx, mapy );
            cvReleaseImage( &temp );

            cv::Mat old;

            cv::Mat channel[3];
            cv::split(frame5, channel);
            if(ui->cam5B->isChecked() == true) {
                channel[0]=cv::Mat::zeros(frame5->height, frame5->width, CV_8UC1); //BLUE!!!!!!
            }
            if(ui->cam5G->isChecked() == true) {
                channel[1]=cv::Mat::zeros(frame5->height, frame5->width, CV_8UC1); //GREEN!!!!!!
            }
            if(ui->cam5R->isChecked() == true) {
                channel[2]=cv::Mat::zeros(frame5->height, frame5->width, CV_8UC1); //RED!!!!!!
            }
            cv::merge(channel, 3, old);
            IplImage newer = old;
            IplImage* new_image = &newer;

            cvShowImage( "Calibrated VidWindow 5", new_image );
            int c = cvWaitKey( 15 );
            if( c == 'p' ){
                        c = 0;
                        while( c != 'p' && c != 27 ){
                            c = cvWaitKey( 250 );
                        }
                    }
                    if( c == 27 ){
                        cvDestroyWindow("Calibrated VidWindow 5");
                        cvReleaseCapture(&capture);
                        break;}
            frame5 = cvQueryFrame( capture );

        }
    }
    else {
        cv::namedWindow(name5);
        cv::VideoCapture cap5 = cv::VideoCapture(vSA);
        cap5.set(CV_CAP_PROP_FRAME_WIDTH, x5 );
        cap5.set(CV_CAP_PROP_FRAME_HEIGHT, y5 );
        cv::Mat frame5;
        cv::Mat channel[3];
        do{
            cap5 >> frame5;
            cv::split(frame5, channel);
            if(ui->cam5B->isChecked() == true) {
                channel[0]=cv::Mat::zeros(frame5.rows, frame5.cols, CV_8UC1); //BLUE!!!!!!
            }
            if(ui->cam5G->isChecked() == true) {
                channel[1]=cv::Mat::zeros(frame5.rows, frame5.cols, CV_8UC1); //GREEN!!!!!!
            }
            if(ui->cam5R->isChecked() == true) {
                channel[2]=cv::Mat::zeros(frame5.rows, frame5.cols, CV_8UC1); //RED!!!!!!
            }
            cv::merge(channel, 3, frame5);
            imshow(name5 ,frame5);
            //cap1.release();
        }while(cv::waitKey(30)<0);
        cv::destroyWindow(name5);
        }

}

