//opencv libraries
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//C++ standard libraries
#include <iostream>
#include <chrono>
#include <vector>

using namespace cv;
using namespace std;

//function declarations

/**
Function that returns the maximum of 3 integers
@param a first integer
@param b second integer
@param c third integer
*/
int myMax(int a, int b, int c);

/**
Function that returns the minimum of 3 integers
@param a first integer
@param b second integer
@param c third integer
*/
int myMin(int a, int b, int c);

/**
Function that detects whether a pixel belongs to the skin based on RGB values
@param src The source color image
@param dst The destination grayscale image where skin pixels are colored white and the rest are colored black
*/
void mySkinDetect(Mat& src, Mat& dst);

/**
Function that does frame differencing between the current frame and the previous frame
@param src The current color image
@param prev The previous color image
@param dst The destination grayscale image where pixels are colored white if the corresponding pixel intensities in the current
and previous image are not the same
*/

void testImg(vector<Mat> mh, int i);
/**
@param mh a container of all the hand templates
@param imgMat destination image
@param  # of the set (fist, open plam ...)
*/

int findSet(Mat& templ);

int main()
{
    String trainingSet[5];
    trainingSet[0] = "ok sign";
    trainingSet[1] = "fist";
    trainingSet[2] = "open palm";
    trainingSet[3] = "gun sign";
    trainingSet[4] = "thumb up";

    vector<Mat> testImgSet[5];

    string promptMsg[2];
    promptMsg[0] = "Get Ready to put your hand in rectangle";
    promptMsg[1] = "Starting in 1 second";

    int FRAME_SIZE = 100;
    //----------------
    //a) Reading a stream of images from a webcamera, and displaying the video
    //----------------
    // For more information on reading and writing video: http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html
    // open the video camera no. 0
    VideoCapture cap(0);

    // if not successful, exit program
    if (!cap.isOpened())
    {
        cout << "Cannot open the video cam" << endl;
        return -1;
    }

    //create a window called "MyVideoFrame0"
    //namedWindow("MyVideo0", WINDOW_AUTOSIZE);
    Mat frame0;

    // read a new frame from video
    bool bSuccess0 = cap.read(frame0);

    //if not successful, break loop
    if (!bSuccess0)
    {
        cout << "Cannot read a frame from videoFRAME_SIZE stream" << endl;
    }

    //create a window called "Camera"
    namedWindow("Camera", WINDOW_AUTOSIZE);

    for (size_t i = 0; i< 5; i ++) {
        Mat info = Mat::zeros(frame0.rows/2, frame0.cols/2, CV_8UC1);
        putText(info,trainingSet[i],Point(frame0.rows/8,frame0.cols/8),FONT_HERSHEY_SIMPLEX,4,128);
        imshow("Camera", info);
        waitKey(0);

        int frameCount = 0;
        chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
        while (frameCount<FRAME_SIZE) {
            // read a new frame from video
            Mat frame;
            bool bSuccess = cap.read(frame);
            //if not successful, break loop
            if (!bSuccess) {
                cout << "Cannot read a frame from video stream" << endl;
                break;
            }

            Mat frameDisplay = frame.clone();
            rectangle(frameDisplay, Point(0, frame.rows / 5), Point(frame.cols / 3, frame.rows / 5 * 4), 128);
            Mat frameS;
            resize(frameDisplay,frameS,Size(frameDisplay.cols/2,frameDisplay.rows/2));
            imshow("Camera", frameS);

            // Time the elapse seconds
            chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
            chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>(t2 - t1);

            if (time_used.count() < 3) {
                putText(frameS, promptMsg[0], Point(frameS.cols / 8, frameS.rows / 8),
                        FONT_HERSHEY_SIMPLEX, 2, 128, 4);
            } else if (time_used.count() < 4) {
                putText(frameS, promptMsg[1], Point(frameS.cols / 8, frameS.rows / 8),
                        FONT_HERSHEY_SIMPLEX, 2, 128, 4);
            } else {
                // Put the image into template training set
                testImgSet[i].push_back(frame);

                frameCount++;
            }
            frame0 = frame;

            //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
            if (waitKey(50) == 27) {
                cout << "esc key is pressed by user" << endl;
                break;
            }

        }
    }
    cap.release();

    // Merge all image into a downsampled big image
    for (int i = 0; i< 5; i ++)
    {
        testImg(testImgSet[i], i);
    }

    return 0;
}

//Function that returns the maximum of 3 integers
int myMax(int a, int b, int c) {
    int m = a;
    (void)((m < b) && (m = b));
    (void)((m < c) && (m = c));
    return m;
}

//Function that returns the minimum of 3 integers
int myMin(int a, int b, int c) {
    int m = a;
    (void)((m > b) && (m = b));
    (void)((m > c) && (m = c));
    return m;
}

//Function that detects whether a pixel belongs to the skin based on RGB values
void mySkinDetect(Mat& src, Mat& dst) {
    //Surveys of skin color modeling and detection techniques:
    //Vezhnevets, Vladimir, Vassili Sazonov, and Alla Andreeva. "A survey on pixel-based skin color detection techniques." Proc. Graphicon. Vol. 3. 2003.
    //Kakumanu, Praveen, Sokratis Makrogiannis, and Nikolaos Bourbakis. "A survey of skin-color modeling and detection methods." Pattern recognition 40.3 (2007): 1106-1122.
    for (size_t x=0; x<src.rows; x++)
    {
        for (size_t y=0; y<src.cols; y++)
        {
            Vec3b intensity = src.at<Vec3b>(x, y);
            // Question: the arrangment of color RGB or BGR
            int B = intensity[0], G = intensity[1], R = intensity[2];
            if ((R > 95 && G > 40 && B > 20) && (myMax(R, G, B) - myMin(R, G, B) > 15) && (abs(R - G) > 15) && (R > G) && (R > B)){
                dst.at<uchar>(x, y) = 255;
            }
        }
    }

}

//Merge all templates
void testImg(vector<Mat> mh, int i){
    int ConfusionMat[5];
    ConfusionMat[0] = 0;
    ConfusionMat[1] = 0;
    ConfusionMat[2] = 0;
    ConfusionMat[3] = 0;
    ConfusionMat[4] = 0;
    for (size_t j=0; j<100; j++){

        Mat frame = mh[j];
        Mat frameSkin;
        frameSkin = Mat::zeros(frame.rows, frame.cols,
                               CV_8UC1); //Returns a zero array of same size as src mat, and of type CV_8UC1
        //	b) Skin color detection
        mySkinDetect(frame, frameSkin);

        Mat templ;
        frameSkin(Rect(0, frame.rows / 5, frame.cols / 3, frame.rows / 5 * 3)).copyTo(templ);
        resize(templ,templ,Size(160,160));
        int set = findSet(templ);
        ConfusionMat[set]++;
    }
    cout << ConfusionMat[0] <<" "
         << ConfusionMat[1] <<" "
         << ConfusionMat[2] <<" "
         << ConfusionMat[3] <<" "
         << ConfusionMat[4] <<" "<<endl;
}

int findSet(Mat& templ)
{
    Mat img; Mat result;
    img = imread( "../template.png", CV_8UC1 );

    int result_cols =  img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;

    matchTemplate( img, templ, result, 0);

    double minVal; double maxVal; Point minLoc; Point maxLoc;
    Point matchLoc;
    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
    matchLoc = minLoc;

    int set = (matchLoc.y+templ.rows/2)/(160*4);
    return set;
}