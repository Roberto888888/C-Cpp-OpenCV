/****************************************************************************************
*                                   EXERCISE 2                                          *
*                                  ------------                                         *
*                                 BINARY IMAGES                                         *
****************************************************************************************/

#include <opencv2/opencv.hpp>

#define LINES 'L'
#define RECTS 'R'
#define PI 3.14159265358979323846

using namespace std;
using namespace cv;


IplImage * getHistogram(IplImage *, char);
IplImage * getBinImagePlus(IplImage *, unsigned, int *, int *, int *);
IplImage * interactiveThresholding(IplImage *, IplImage *, IplImage *, int *, int *, int *);
IplImage * simpleThresholding(IplImage *, IplImage *, int *, int *, int *);
void reducedCentralMoments(IplImage *, double, double, double *, double *, double *);