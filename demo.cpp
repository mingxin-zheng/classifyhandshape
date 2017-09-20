#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace std;
using namespace cv;
// Classify the webcam image by template matching
int findSet(Mat& templ);

// The skin detecting function
void mySkinDetect(Mat& src, Mat& dst);

// Find the maximum & minimum number among three numbers
int myMax(int a, int b, int c);

int myMin(int a, int b, int c);

int main( int argc, char** argv )
{
    Mat templ;
    VideoCapture cap(0);

    // if not successful, exit program
    if (!cap.isOpened())
    {
        cout << "Cannot open the video cam" << endl;
        return -1;
    }

    String trainingSet[5];
    trainingSet[0] = "ok sign";
    trainingSet[1] = "fist";
    trainingSet[2] = "open palm";
    trainingSet[3] = "gun sign";
    trainingSet[4] = "thumb up";

    while (1) {
        // read a new frame from video
        Mat frame;
        bool bSuccess = cap.read(frame);
        //if not successful, break loop
        if (!bSuccess) {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }

        // destination frame
        Mat frameSkin;
        frameSkin = Mat::zeros(frame.rows, frame.cols,
                               CV_8UC1); //Returns a zero array of same size as src mat, and of type CV_8UC1
        //----------------
        //Skin color detection
        //----------------
        mySkinDetect(frame, frameSkin);
        // copy the image in rectange ROI into a template Mat container
        Mat templ;
        frameSkin(Rect(0, frame.rows / 5, frame.cols / 3, frame.rows / 5 * 3)).copyTo(templ);
        resize(templ,templ,Size(160,160));

        // Classify the template image
        int set = findSet(templ);
        //cout<<" set: " <<trainingSet[set] <<endl;


        //----------------
        //	Display
        //----------------
        Mat frameDisplay = frame.clone();

        // Resize and put text on the image display
        rectangle(frameDisplay, Point(0, frame.rows / 5), Point(frame.cols / 3, frame.rows / 5 * 4), Scalar(128,128,128));
        rectangle(frameSkin, Point(0, frame.rows / 5), Point(frame.cols / 3, frame.rows / 5 * 4), 128);

        putText(frameDisplay,trainingSet[set], Point(frame.cols / 8, frame.rows / 8), FONT_HERSHEY_SIMPLEX, 2, 128, 4);
        putText(frameSkin,trainingSet[set], Point(frame.cols / 8, frame.rows / 8), FONT_HERSHEY_SIMPLEX, 2, 128, 4);

        Mat frameSkinS,frameS;

        resize(frameDisplay,frameS,Size(frameSkin.cols/2,frameSkin.rows/2));
        resize(frameSkin,frameSkinS,Size(frameSkin.cols/2,frameSkin.rows/2));

        imshow("Camera", frameS);
        imshow("Skin", frameSkinS);


        //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop

        if (waitKey(30) == 27) {
            cout << "esc key is pressed by user" << endl;
            imwrite("../screenshot.png",frameDisplay);
            imwrite("../screenshot_skin.png", frameSkin);
            break;
        }

    }

    return 0;
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
    /*
    Mat img_display;
    img.copyTo( img_display );
    rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(255), 2, 8, 0 );
    */
    int set = (matchLoc.y+templ.rows/2)/(160*4);
    return set;
}

void mySkinDetect(Mat& src, Mat& dst)
{
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