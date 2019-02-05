// ImageSubtractionCpp.sln
// main.cpp

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>

#include<conio.h>           




// global variables ///////////////////////////////////////////////////////////////////////////////
const cv::Scalar SCALAR_BLACK = cv::Scalar(0.0, 0.0, 0.0);
const cv::Scalar SCALAR_WHITE = cv::Scalar(255.0, 255.0, 255.0);
const cv::Scalar SCALAR_BLUE = cv::Scalar(255.0, 0.0, 0.0);
const cv::Scalar SCALAR_GREEN = cv::Scalar(0.0, 200.0, 0.0);
const cv::Scalar SCALAR_RED = cv::Scalar(0.0, 0.0, 255.0);

///////////////////////////////////////////////////////////////////////////////////////////////////
int main(void) {

	cv::VideoCapture capWebcam(0);            // declare a VideoCapture object and associate to webcam, 0 => use 1st webcam
	
	cv::Mat CameraFeed;
	cv::Mat CompareImg;

	if (capWebcam.isOpened() == false) {                                // check if VideoCapture object was associated to webcam successfully
		std::cout << "error: A Webcam has not been accessed successfully\n\n";     // if not, print error message to standard out
		_getch();                                                       // and exit program
		return(0);
	}
	
	capWebcam.read(CameraFeed); 
	capWebcam.read(CompareImg);
	
	char chCheckForEscKey = 0;

	while (capWebcam.isOpened() && chCheckForEscKey != 27) {

		capWebcam.read(CameraFeed);
		
        cv::Mat imgFrame1Copy = CameraFeed.clone();
		cv::Mat imgFrame2Copy = CompareImg.clone();

		cv::Mat imgDifference;
		cv::Mat imgThresh;

		cv::cvtColor(imgFrame1Copy, imgFrame1Copy, CV_BGR2GRAY);
		cv::cvtColor(imgFrame2Copy, imgFrame2Copy, CV_BGR2GRAY);

		cv::GaussianBlur(imgFrame1Copy, imgFrame1Copy, cv::Size(9,9), 0);
		cv::GaussianBlur(imgFrame2Copy, imgFrame2Copy, cv::Size(9,9), 0);

		cv::absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);

		cv::threshold(imgDifference, imgThresh, 30, 255.0, CV_THRESH_BINARY);

		cv::imshow("imgThresh", imgThresh);

		cv::Mat structuringElement3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
		cv::Mat structuringElement5x5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
		cv::Mat structuringElement7x7 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
		cv::Mat structuringElement9x9 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9));

		cv::dilate(imgThresh, imgThresh, structuringElement9x9);
		
		cv::dilate(imgThresh, imgThresh, structuringElement9x9);
		
		cv::erode(imgThresh, imgThresh, structuringElement9x9);

		cv::Mat imgThreshCopy = imgThresh.clone();

		std::vector<std::vector<cv::Point> > contours;

		cv::findContours(imgThreshCopy, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		cv::Mat imgContours(imgThresh.size(), CV_8UC3, SCALAR_BLACK);

		cv::drawContours(imgContours, contours, -1, SCALAR_WHITE, 0);


		cv::imshow("imgContours", imgContours);
	
		chCheckForEscKey = cv::waitKey(1);      // get key press in case user pressed esc

	}

	if (chCheckForEscKey != 27) {               // if the user did not press esc (i.e. we reached the end of the video)
		cv::waitKey(0);                         // hold the windows open to allow the "end of video" message to show
	}
	// note that if the user did press esc, we don't need to hold the windows open, we can simply let the program end which will close the windows

	return(0);
}