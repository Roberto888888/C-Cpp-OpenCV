/****************************************************************************************
*                                    EXERCISE 4                                         *
*                                   ------------                                        *
*                                     CONTOURS                                          *
****************************************************************************************/

#include <opencv2/opencv.hpp>
#include <iostream>

#define LINES 'L'
#define RECTS 'R'

using namespace std;
using namespace cv;

IplImage * simpleThresholding(IplImage * , IplImage *);
IplImage * getHistogram(IplImage * , char);
IplImage * getBinImagePlus(IplImage * , unsigned);
void searchContour(IplImage * , IplImage *);
void gradSearchContour(IplImage * , IplImage *);
bool gradCheck(char * , char * , int);
bool check(char * , int);