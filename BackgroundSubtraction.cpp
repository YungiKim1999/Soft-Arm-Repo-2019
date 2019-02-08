//This "BackgroundSubtraction" code takes the first frame(when the program is executed) as the background
//and compares the current frame with the background/first frame. The difference is shown as foreground and is 
//extracted onto a new image.
//This code uses functions from the OpenCV library. Specifically, OpenCV version 2.4.13.7.

//INPUTS: CameraFeed(the webcam feed)
//OUTPUTS:Foreground(object)

//Author: Daniel Kim

//Credits: -Chris Dahms (https://github.com/MicrocontrollersAndMore/OpenCV_3_Windows_10_Installation_Tutorial)
//         - Phillip Wagner(https://bytefish.de/blog/extracting_contours_with_opencv/)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//LIBRARIES USED

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<conio.h>           
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//GLOBAL VARIABLES

const cv::Scalar SCALAR_BLACK = cv::Scalar(0.0, 0.0, 0.0);
const cv::Scalar SCALAR_WHITE = cv::Scalar(255.0, 255.0, 255.0);
const cv::Scalar SCALAR_BLUE = cv::Scalar(255.0, 0.0, 0.0);
const cv::Scalar SCALAR_GREEN = cv::Scalar(0.0, 200.0, 0.0);
const cv::Scalar SCALAR_RED = cv::Scalar(0.0, 0.0, 255.0);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void) {

	cv::VideoCapture capWebcam(0);            // declare a VideoCapture object and associate to webcam, "0" => use 1st webcam
	
	cv::Mat CameraFeed;  
	cv::Mat CompareImg;

	if (capWebcam.isOpened() == false) {                                // check if VideoCapture object was associated to webcam successfully
		std::cout << "error: A Webcam has not been accessed successfully\n\n";     // if not, print error message to standard out
		_getch();                                                       // and exit program
		return(0);
	}
	
	capWebcam.read(CameraFeed);   //Read images found on the webcam and
	capWebcam.read(CompareImg);   //put data into matrices.
	                              //Note: only "CameraFeed" matrix will be updated with current frame data.
	                              //      "CompareImg" matrix will stay the same
	
	char chCheckForEscKey = 0;

	while (capWebcam.isOpened() && chCheckForEscKey != 27) {           //Check that Webcam is still open or if ESC key is pressed

		capWebcam.read(CameraFeed);  //Get the Next frame of the webcam feed
		
        cv::Mat CameraFeedCopy = CameraFeed.clone();  //Clone the "CameraFeed" and "CompareImg" matrices onto
		cv::Mat CompareImgCopy = CompareImg.clone();  //New matrices ("CameraFeedCopy" and "CompareImgCopy")

		cv::Mat ImgDifference;
		cv::Mat ImgThresh;

		cv::cvtColor(CameraFeedCopy, CameraFeedCopy, CV_BGR2GRAY);   //convert the colour of "CameraFeedCopy" into grayscale.
		cv::cvtColor(CompareImgCopy, CompareImgCopy, CV_BGR2GRAY);   //convert the colour of "CompareImgCopy" into grayscale.

		cv::GaussianBlur(CameraFeedCopy, CameraFeedCopy, cv::Size(9,9), 0);   //Add Gaussian Blur to "CameraFeedCopy" to reduce noise.
		cv::GaussianBlur(CompareImgCopy, CompareImgCopy, cv::Size(9,9), 0);   //Add Gaussian Blur to "CompareImgCopy" to reduce noise.

		cv::absdiff(CameraFeedCopy, CompareImgCopy, ImgDifference);   //Find the absolute difference between the two matrices:
		                                                              //"CameraFeedCopy" and "CompareImgCopy", save to "ImgDifference"

		cv::threshold(ImgDifference, ImgThresh, 30, 255.0, CV_THRESH_BINARY);   //Threshold the "ImgDifference", save to "ImgThresh"

		cv::Mat structuringElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(21, 21)); //Declare a Structuring Element in order
		                                                                                          //To dilate and erode image.
		                      //Note: To increase/decrease dilation/erosion, change size of structuring element (odd numbers, same number).
        
		cv::dilate(ImgThresh, ImgThresh, structuringElement);     //Dilate and Erode Image.
		cv::dilate(ImgThresh, ImgThresh, structuringElement);
		cv::erode(ImgThresh, ImgThresh, structuringElement);

		cv::Mat ImgThreshCopy = ImgThresh.clone();   //Create a copy of the "ImgThresh" matrix. Store into "ImgThreshCopy"

		std::vector<std::vector<cv::Point> > contours;     //Declare a vector "contours"
		cv::Mat ImgContours(ImgThresh.size(), CV_8UC3, SCALAR_BLACK); //Declare a matrix with black pixels "ImgContours"
		
		cv::findContours(ImgThreshCopy, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);  //Find contours of "ImgThreshCopy"
        cv::drawContours(ImgContours, contours, -1, SCALAR_WHITE,-1);  //Draw contours onto the "ImgContours" Matrix
       
		cv::Mat FinalImg(CameraFeed.rows, CameraFeed.cols, CV_8UC3);   //Declare matrix of same size as CameraFeed as "FinalImg"

	    FinalImg.setTo(cv::Scalar(0,0,0)); //Set all pixels in "FinalImg" to black
        CameraFeed.copyTo(FinalImg, ImgContours); // Copy "CameraFeed" onto "FinalImg", using contours from "ImgContours"
		
		cv::imshow("imgThresh", ImgThresh);           //Show solutions to user
	    cv::imshow("imgContours", ImgContours);
        cv::imshow("FinalImg", FinalImg);
        cv::imshow("original", CameraFeed);

        chCheckForEscKey = cv::waitKey(1);      // get key press in case user pressed esc

	}

	if (chCheckForEscKey != 27) {               // Check whether user hasn't pressed the ESC Key
		cv::waitKey(0);                         // If user hasn't pressed ESC key, display window.
	}
	
	return(0);
}