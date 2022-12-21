#include <stdio.h> 
#include "cv.h" 
#include "cxcore.h"
#include "highgui.h"
#include <conio.h> 

int main(int argc, char* argv[]) {
	IplImage * img;
	img = cvLoadImage(argv[1], 1);
	const char* wName = "Hello world!"; // window name     
	cvNamedWindow(wName, 0); // create simple window 
	const char* gName = "Hello world! gray "; // window name     
	cvNamedWindow(gName, 0); // create simple window 
	CvCapture* capture = 0;

	double capProp = 0;

	IplImage *frame,*frame24; // pointers to images 
	
	const char* RName = "B"; // window name   
	const char* GName = "G"; // window name 
	const char* BName = "R"; // window name 
	
	cvNamedWindow(RName, 0); // create simple window 
	cvNamedWindow(GName, 0); // create simple window 
	cvNamedWindow(BName, 0); // create simple window d
	
	capture = cvCaptureFromCAM(0); // initialize capture device 
	frame = cvRetrieveFrame(capture);
	frame24 = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U,3 );
	printf("Frame settings:\n Width: %d\n Height: %d\n", frame->width, frame->height);                  
	cvResizeWindow(wName, frame->width, frame->height); 
	IplImage *gray = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 1 );
	cvCvtColor(frame, gray, CV_RGB2GRAY );
	cvShowImage(wName, frame);
	cvShowImage(gName, gray);
	///3 channel
	IplImage *R = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 1);
	IplImage *G = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 1);
	IplImage *B = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 1);
	
	cvSplit(img, B, G, R, NULL);
	cvShowImage(RName, B);
	cvShowImage(GName, G);
	cvShowImage(BName, R);
	///__________________________Histogram__________________________________

	int hist[256];
	int histB[256];
	int histG[256];
	int histR[256];
	int st = 4;
	// Initialize histogram
	for (int i = 0; i < 256; i++) {
		hist[i] = 0;
		histB[i] = 0;
		histG[i] = 0;
		histR[i] = 0;
	}
	// Calculate histogram

	for (int i = 0; i < img->height; i++) {
		char * ptrB = B->imageData + i * B->widthStep;
		char * ptrG = G->imageData + i * G->widthStep;
		char * ptrR = R->imageData + i * R->widthStep;
		for (int j = 0; j < img->width; j++) {
			/// PROBLEM: accessing data with a pointer to char is NOT ok! But no cast works!
			///I don't understand why... What follows (which is horrible) is the consequence
			if (*ptrB < 0) {
				char c = *ptrB;
				uchar x = (uchar)c;
				histB[x] += 1;
			}
			else
				histB[*ptrB] += 1;
			ptrB++;

			if (*ptrG < 0) {
				char c = *ptrG;
				uchar x = (uchar)c;
				histG[x] += 1;
			}
			else
				histG[*ptrG] += 1;
			ptrG++;

			if (*ptrR < 0) {
				char c = *ptrR;
				uchar x = (uchar)c;
				histR[x] += 1;
			}
			else
				histR[*ptrR] += 1;
			ptrR++;
		}
	}

	// Create image
	//______________________________________________________________________________IMPROVE VISUALIZATION!!!!!

	IplImage* hisB = cvCreateImage(cvSize(st*256, 600), IPL_DEPTH_8U, 1);
	cvSet(hisB, cvScalar(0,0,0));   // Initialize image (all black)
	IplImage* hisG = cvCreateImage(cvSize(st * 256, 600), IPL_DEPTH_8U, 1);
	cvSet(hisG, cvScalar(0, 0, 0));   // Initialize image (all black)
	IplImage* hisR = cvCreateImage(cvSize(st * 256, 600), IPL_DEPTH_8U, 1);
	cvSet(hisR, cvScalar(0, 0, 0));   // Initialize image (all black)

	// Create window to contain the image
	cvNamedWindow("histogramB", 2);
	cvNamedWindow("histogramG", 2);
	cvNamedWindow("histogramR", 2);
	hisB->origin = IPL_ORIGIN_BL;  // Set the origin in the bottom left corner
	hisG->origin = IPL_ORIGIN_BL;  // Set the origin in the bottom left corner
	hisR->origin = IPL_ORIGIN_BL;  // Set the origin in the bottom left corner
	// Draw a line for each bin
	for (int i = 0; i < 256; i++) {
		if (histB[i] != 0)
			cvLine(hisB, cvPoint(i*st, 0), cvPoint(i*st, histB[i] / 10), cvScalar(150, 150,150), 1, 4);
		if (histG[i] != 0)
			cvLine(hisG, cvPoint(i*st, 0), cvPoint(i*st, histG[i] / 10), cvScalar(150, 150, 150), 1, 4);
		if (histR[i] != 0)
			cvLine(hisR, cvPoint(i*st, 0), cvPoint(i*st, histR[i] / 10), cvScalar(150, 150, 150), 1, 4);
	}

	// Show on window
	cvShowImage("histogramB", hisB);
	cvShowImage("histogramG", hisG);
	cvShowImage("histogramR", hisR);
	cvWaitKey(0);
	
	return 0;
}