/*****************************************************************************************
*                                    EXERCISE 3                                          *
*                                   ------------                                         *
*                                    FILTERING                                           *
*****************************************************************************************/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>

using namespace cv;
using namespace std;


void func(char *, char *, int, short);
void lowPass(IplImage *, IplImage *);
void fractileFilter(IplImage *, IplImage *, int);
void xGradientFilter(IplImage *, IplImage *);
void yGradientFilter(IplImage *, IplImage *);
int doubleStream(CvMat*);