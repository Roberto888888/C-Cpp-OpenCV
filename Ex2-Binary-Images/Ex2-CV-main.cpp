/****************************************************************************************
*                                    EXERCISE 2                                         *
*                                   ------------                                        *
*                                  BINARY IMAGES                                        *
****************************************************************************************/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define LINES 'L'
#define RECTS 'R'
#define PI 3.14159265358979323846

using namespace std;
using namespace cv;

int main(int argc, char ** argv) {

    /***********************************************************************
    *                                PART 1                                *
    *                               --------                               *
    *                             Thresholding                             *
    ***********************************************************************/
    /// Uploading and converting to grey scale
    IplImage * img = cvLoadImage(argv[1] , 1);
    IplImage * G   = cvCreateImage(cvGetSize(img) , IPL_DEPTH_8U , 1);
    cvCvtColor(img , G , CV_RGB2GRAY);

    /// Uploading directly in grey scale
    //IplImage * G = cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

    ///__________________________Histogram__________________________________
    ///                DRAW A PRELIMINARY SIMPLE HISTOGRAM

    IplImage * his = getHistogram(G , LINES);

    cvNamedWindow("Grey scale" , 0);
    cvShowImage("Grey scale" , G);

    cvNamedWindow("histogram" , 0);
    cvShowImage("histogram" , his);

    /// First threshold selection
    printf("\nTake a look at the grey scale image and histogram then press any key.\n");
    cvWaitKey(0);

    ///_____________________Binary Image Formation_______________________________


    // These are needed in PART 2 to calculate the coordinates of the centre of mass.
    // Declared now because they can be calculated in parallel with the thresholding.
    int tot = 0, xcm = 0, ycm = 0;

    IplImage * bin;

    ///_______________INTERACTIVE THRESHOLDING - A little unstable...
    // Grey scale histogram converted to 3-channel image first.
    // It will be useful later to draw the threshold on the histogram.
    IplImage * Chis = cvCreateImage(cvGetSize(his) , IPL_DEPTH_8U , 3);
    cvCvtColor(his , Chis , COLOR_GRAY2RGB);
    cvFlip(Chis , Chis , 0);

    bin = interactiveThresholding(G , Chis , his , &tot , &xcm , &ycm);

    cvSaveImage("histogram.png" , Chis);

    ///______________________SIMPLE THRESHOLDING
    /// Observe grey scale and histogram, select a threshold once, and hope!
    //bin = simpleThresholding(G, his, &tot, &xcm, &ycm);


    /***********************************************************************
    *                                PART 2                                *
    *                               --------                               *
    *                            Centre of Mass                            *
    ***********************************************************************/
    /// Coordinates
    ycm /= tot;
    xcm /= tot;

    printf("\nCenter of mass: (%d , %d)\n", xcm, ycm);

    /// Draw a cross in the picture
    IplImage * I = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
    cvCvtColor(bin, I, COLOR_GRAY2RGB);          // To draw in the binary image
    //cvCvtColor(G, I, COLOR_GRAY2RGB);          // To draw in the grey scale image

    /// Draw a cross in the center of mass
    unsigned short A = 10;
    cvLine(I, cvPoint((xcm - A),(ycm - A))  , cvPoint((xcm + A),(ycm + A)) , cvScalar(0,0,255) , 2, 4);
    cvLine(I, cvPoint((xcm - A), (ycm + A)) , cvPoint((xcm + A),(ycm - A)) , cvScalar(0,0,255) , 2, 4);
     
    /// Just a different style of cross
    //cvLine(I, cvPoint(xcm,(ycm - A)), cvPoint(xcm,(ycm + A)), cvScalar(255,0,255), 2, 4);
    //cvLine(I, cvPoint((xcm - A), ycm), cvPoint((xcm + A), ycm), cvScalar(255,0,255), 2, 4);


    /***********************************************************************
    *                                PART 3                                *
    *                               --------                               *
    *                            Image Moments                             *
    ***********************************************************************/
    double M_20 = 0, M_02 = 0, M_11 = 0;
    reducedCentralMoments(bin, xcm, ycm, &M_20, &M_02, &M_11);

    /// Inclination of Principal Axis
    double th = 0.5 * atan2((2 * M_11), (M_20 - M_02));
    double th_deg = (th / PI) * 180;
    printf("\nInclination of main axis (deg): %.2f\n", th_deg);
    double m = tan(th);
    printf("\nAngular Coefficient: %.2f\n", m);

    /// Draw a line at an angle theta, through the centre of mass
    cvLine(I, cvPoint((xcm - 30),(ycm - 30*m)), cvPoint((xcm + 30),(ycm + 30*m)), cvScalar(255,0,0), 2, 4);

    cvNamedWindow("Center of Mass", 0);
    cvShowImage("Center of Mass", I);

    cvSaveImage("Binary.png", bin);
    cvSaveImage("Center_of_Mass.png", I);

    /***********************************************************************
    *                                PART 4                                *
    *                               --------                               *
    *                          Moment Invanriances                         *
    ***********************************************************************/

    //______________________________________________________________IN CLASS


    ///*********************************************************************
    ///                              Clean-Up                              *
    ///*********************************************************************

    cvWaitKey(0);

    cvReleaseImage(&img);
    cvReleaseImage(&his);
    cvReleaseImage(&G);
    cvReleaseImage(&bin);
    cvReleaseImage(&I);
    cvReleaseImage(&Chis);  // Comment this out if you're doing simple thresholding

    cvDestroyAllWindows();

    return 0;
}