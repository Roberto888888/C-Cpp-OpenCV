/****************************************************************************************
*                                    EXERCISE 4                                         *
*                                   ------------                                        *
*                                     CONTOURS                                          *
****************************************************************************************/

#include <opencv2/opencv.hpp>
#include "Ex4-CV-header.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#define LINES 'L'
#define RECTS 'R'

using namespace std;
using namespace cv;



void gradSearchContour(IplImage * img , IplImage * contour) {
    for(int i = 0 ; i < img->height - 2 ; i++) {

        char * p_img = img->imageData + (i + 1) * img->widthStep + 1;
        char * p_contour = contour->imageData + i * contour->widthStep;

        for(int j = 0 ; j < img->width - 2 ; j++) {
            if ( gradCheck(p_img , p_img - 1 - img->widthStep , img->widthStep) )
                    *p_contour = 255;
            p_img++;
            p_contour++;
        }
    }
}




bool gradCheck (char * c , char * p , int step) {
    for(int i = 0 ; i < 3 ; i++) {
        char * lp = p + i * step;
        for(int j = 0 ; j < 3 ; j++) {
            if (i == 1 && j == 1) {
                lp++;
                continue;
            }
            float diff = (float)((uchar)(*lp) - (uchar)(*c));
            if(diff > 23)
                return true;
            lp++;
        }
    }
    return false;
}




void searchContour(IplImage * bin , IplImage * contour) {
    for(int i = 0 ; i < bin->height - 2 ; i++) {

        char * p_bin = bin->imageData + (i + 1) * bin->widthStep + 1;
        char * p_contour = contour->imageData + i * contour->widthStep;

        for(int j = 0 ; j < bin->width - 2 ; j++) {
            if ( ((uchar)*p_bin == 0) && check(p_bin - 1 - bin->widthStep , bin->widthStep))
                    *p_contour = 255;
            p_bin++;
            p_contour++;
        }
    }
}



bool check (char * p , int step) {
    for(int i = 0 ; i < 3 ; i++) {

        char * lp = p + i * step;

        for(int j = 0 ; j < 3 ; j++) {
            if (i == 1 && j == 1) {
                lp++;
                continue;
            }
            if((uchar)(*lp) == 255)
                return true;
            lp++;
        }
    }
    return false;
}



/****************************************************************************************
* This function allows the user to select a threshold and draws a new histogram with a  *
* visual indication of the threshold. At the same time computes the necessary numbers   *
* for the centre of mass.                                                               *
*                                                                                       *
* The function needs:                                                                   *
*    - A pointer to a GREYSCALE image to apply the threshold to.                        *
*    - A pointer to a preliminary histogram image (starting point to draw the new       *
*      histogram with the threshold).                                                   *
*    - 3 pointers to int to compute the center of mass coordinates.                     *
****************************************************************************************/

IplImage * simpleThresholding(IplImage * G, IplImage * his) {
    unsigned int t = 0;             // Threshold
    char st = 4;                    // To place correctly the threshold line in the image
    IplImage * bin;                 // To contain the binary image

    printf("\nInsert Threshold: ");
    scanf("%u", &t);

    IplImage * Chis = cvCreateImage(cvGetSize(his) , IPL_DEPTH_8U , 3);

    // Represent a greyscale histogram in a 3-channel image.
    // It will be useful later to draw the threshold on the histogram.

    cvCvtColor(his, Chis, COLOR_GRAY2RGB);
    cvFlip(Chis, Chis, 0);

    cvLine(Chis, cvPoint(t*st, 0), cvPoint(t*st, 600), cvScalar(0,0,255), 2, 4);
    cvShowImage("histogram", Chis);
    cvSaveImage("histogram.png", Chis);
    cvReleaseImage(&Chis);

    bin = getBinImagePlus(G, t);
    cvNamedWindow("Binary Image", 0);
    cvShowImage("Binary Image", bin);

    return bin;
}




/****************************************************************************************
* From the pointer to a greyscale image, draws a histogram image and returns a pointer  *
* to it. Two possible styles can be selected:                                           *
* LINES (a line for each bin).                                                          *
* RECTS (a rectangle for each bin).                                                     *
****************************************************************************************/

IplImage * getHistogram(IplImage * gray, char style) {
    int dep = 256;
    int hist[dep];             // Histogram for gray scale image
    int st = 4;                // To separate lines in the histograms, for better look ;)

    /// Initialize histograms
    for (int i = 0 ; i < dep ; i++)
        hist[i] = 0;

    /// Calculate histogram
    for(int i = 0 ; i < gray->height ; i++) {

        char * ptr = gray->imageData + i*gray->widthStep;

        for(int j = 0 ; j < gray->width ; j++) {
                if(*ptr < 0) {
                    char c = *ptr;
                    uchar x = (uchar) c;
                    hist[x] += 1;
                }
                else
                    hist[(unsigned)(*ptr)] += 1;
                ptr++;
        }
    }

    /// Create image for the histograms
    IplImage * his = cvCreateImage(cvSize(st*dep,600), IPL_DEPTH_8U, 1);
    cvSet(his, 0);                    // Initialize image (all black)
    his->origin = IPL_ORIGIN_BL;      // Set the origin in the bottom left corner

    /// Draw a line/Rectangle for each bin
    if (style == LINES)
        for (int i = 0 ; i < dep ; i++)
            if (hist[i] != 0)
                cvLine(his, cvPoint(i*st, 0), cvPoint(i*st, hist[i]/10), 150, 1, 4);
    else if (style == RECTS)
        for (int i = 0 ; i < dep ; i++)
            if (hist[i] != 0)
                cvRectangle(his, cvPoint(i*st,hist[i]/10), cvPoint((i+1)*st,0), 150, -1, 4);
    else {
        printf("\nError. No style selected.\n");
        cvReleaseImage(&his);
        return NULL;
    }

    return his;
}




/****************************************************************************************
* Input: a pointer to a GREY SCALE image, a threshold, 3 pointers to integer for the    *
* necessary computation to determine the center of mass.                                *
* Forms a binary image, counts the number of pixels set to 0, sums the values of 2      *
* coordinates for the pixels set to 0 (used later to calculate the center of mass       *
* coordinates).                                                                         *
*                                                                                       *
* PS. It is possible to calculate the coordinates even in this function but it's done   *
* later to respect the structure of the exercise                                        *
****************************************************************************************/

IplImage * getBinImagePlus(IplImage * img, unsigned t) {
    IplImage * I = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U , 1);
    for(int i = 0 ; i < img->height ; i++) {

        char * ptr = img->imageData + i * img->widthStep;
        char * p = I->imageData + i * I->widthStep;

        for(int j = 0 ; j < img->width ; j++) {
                if ((uchar)(*ptr) >= (uchar)t)
                    *p = 255;
                else {
                    // In this case we are on the object
                    *p = 0;
                }
            ptr++;
            p++;
        }
    }
    return I;
}
