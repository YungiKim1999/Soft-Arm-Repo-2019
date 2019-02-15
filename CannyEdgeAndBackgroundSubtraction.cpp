// The CannyEdgeAndBAckgroundSubtraction program combines the outputs of the "BackgroundSubtraction" program and the "CannyEdge" program to 
//give the final result. 
//Note that this program uses the OpenCV library and namespace. This program specifically uses the OpenCV Version 2.4.13.7.

//Inputs: 2 x Camera Feeds (capWebCamA and capWebCamB)
//Outputs: 2 x filtered images (FinalProdctA and FinalProductB)
//Author: Daniel Kim
//Credits:  Chris Dahms(https://github.com/MicrocontrollersAndMore)
//          Philipp Wagner(https://bytefish.de/blog/extracting_contours_with_opencv/)


#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<conio.h>  

//Global Constants
const cv::Scalar SCALAR_BLACK = cv::Scalar(0.0, 0.0, 0.0);
const cv::Scalar SCALAR_WHITE = cv::Scalar(255.0, 255.0, 255.0);
const cv::Scalar SCALAR_BLUE = cv::Scalar(255.0, 0.0, 0.0);
const cv::Scalar SCALAR_GREEN = cv::Scalar(0.0, 200.0, 0.0);
const cv::Scalar SCALAR_RED = cv::Scalar(0.0, 0.0, 255.0);

int main(void){


cv::VideoCapture capWebcamA(2);            // Declare 2 VideoCapture objects associate them to the 2 webcams used
cv::VideoCapture capWebcamB(0);            //Note: The numbers in the brackets might need to be changed in order to get the right Webcam

if (capWebcamA.isOpened() == false) {                                          // check if first VideoCapture object was associated
	std::cout << "error: A Webcam has not been accessed successfully\n\n";     // to webcam successfully if not,
	_getch();                                                                  // print error message to standard out and exit program
	return(0);
}
else if (capWebcamB.isOpened() == false) {                                    // check if Second VideoCapture object was associated
	std::cout << "error: A Webcam has not been accessed successfully\n\n";    // to webcam successfully if not,
	_getch();                                                                 // print error message to standard out and exit program
	return(0);
}

cv::Mat CamFeedA;        //Camera feed 1
cv::Mat CamFeedB;        //Camera feed 2
cv::Mat BackgroundA;     //The first frame of Camera feed 1(Background)
cv::Mat BackgroundB;     //The first frame of Camera feed 2(Background)

capWebcamA.read(CamFeedA);    //Read first frame of Camera feed 1
capWebcamA.read(BackgroundA);

capWebcamB.read(CamFeedB);   //Read first frame of Camera feed 2
capWebcamB.read(BackgroundB);

char chCheckForEscKey = 0;

while (capWebcamA.isOpened() && chCheckForEscKey != 27 && capWebcamB.isOpened()) { //Check that Webcam is still open or if ESC key is pressed

	capWebcamA.read(CamFeedA);     //Get the next fram from respective Camera feeds
	capWebcamB.read(CamFeedB);

	cv::Mat CamFeedCopyA = CamFeedA.clone();  //Clone Camera feeds onto another matrix.
	cv::Mat CamFeedCopyB = CamFeedB.clone();  
	
	cv::Mat BackgroundACopy = BackgroundA.clone();  //Clone the backgrounds of both Camera Feeds
	cv::Mat BackgroundBCopy = BackgroundB.clone();  


	cv::Mat ImgDifferenceA;   //Difference between background of camera feed 1 and current camera feed 1
	cv::Mat ImgDifferenceB;   //Difference between background of camera feed 2 and current camera feed 2
	cv::Mat ImgThreshA;       //Threshold of the difference(camera feed 1)
	cv::Mat ImgThreshB;       //Threshold of the difference(camera feed 2)

	cv::cvtColor(CamFeedCopyA, CamFeedCopyA, CV_BGR2GRAY);   //convert the colour of "CamFeedCopyA" into grayscale.
	cv::cvtColor(CamFeedCopyB, CamFeedCopyB, CV_BGR2GRAY);   //convert the colour of "CamFeedCopyB" into grayscale.

	cv::cvtColor(BackgroundACopy, BackgroundACopy, CV_BGR2GRAY);   //convert the colour of "BackgroundACopy" into grayscale.
	cv::cvtColor(BackgroundBCopy, BackgroundBCopy, CV_BGR2GRAY);   //convert the colour of "BackgroundBCopy" into grayscale.


	cv::GaussianBlur(CamFeedCopyA, CamFeedCopyA, cv::Size(17,17), 11);   //Add Gaussian Blur to "CamFeedCopyA" to reduce noise.
	cv::GaussianBlur(CamFeedCopyB, CamFeedCopyB, cv::Size(17, 17), 11);   //Add Gaussian Blur to "CamFeedCopyB" to reduce noise.
	cv::GaussianBlur(BackgroundACopy, BackgroundACopy, cv::Size(17, 17), 11);   //Add Gaussian Blur to "BackgroundACopy" to reduce noise.
    cv::GaussianBlur(BackgroundBCopy, BackgroundBCopy, cv::Size(17, 17), 11);   //Add Gaussian Blur to "BackgroundBCopy" to reduce noise.
	
	cv::absdiff(CamFeedCopyA, BackgroundACopy, ImgDifferenceA);   //Find the absolute difference between the Camera feeds and background images
	cv::absdiff(CamFeedCopyB, BackgroundBCopy, ImgDifferenceB);	  // the result will be saved to "ImgDifferenceA" and "ImgDifferenceB"

	cv::threshold(ImgDifferenceA, ImgThreshA, 20, 255, CV_ADAPTIVE_THRESH_MEAN_C);   //Threshold the "ImgDifferenceA", save to "ImgThreshA"
	cv::threshold(ImgDifferenceB, ImgThreshB, 30, 255, CV_ADAPTIVE_THRESH_MEAN_C);   //Threshold the "ImgDifferenceB", save to "ImgThreshB"

	cv::Mat structuringElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(17, 17)); //Declare a Structuring Element in order
																							  //To dilate and erode image.
						  //Note: To increase/decrease dilation/erosion, change size of structuring element (odd numbers, same number).

	cv::dilate(ImgThreshA, ImgThreshA, structuringElement);     //Dilate and Erode Images.
	cv::dilate(ImgThreshA, ImgThreshA, structuringElement);
	cv::erode(ImgThreshA, ImgThreshA, structuringElement);

	cv::dilate(ImgThreshB, ImgThreshB, structuringElement);     //Dilate and Erode Images.
	cv::dilate(ImgThreshB, ImgThreshB, structuringElement);
	cv::erode(ImgThreshB, ImgThreshB, structuringElement);


	cv::Mat ImgThreshCopyA = ImgThreshA.clone();   //Create a copy of the "ImgThreshA" matrix. Store into "ImgThreshCopyA"
	cv::Mat ImgThreshCopyB = ImgThreshB.clone();   //Create a copy of the "ImgThreshB" matrix. Store into "ImgThreshCopyB"

	std::vector<std::vector<cv::Point> > ContoursA;     //Declare a vector "ContoursA"
	std::vector<std::vector<cv::Point> > ContoursB;     //Declare a vector "ContoursB"
	
	cv::Mat ImgContoursA(ImgThreshA.size(), CV_8UC3, SCALAR_BLACK); //Declare a matrix with black pixels "ImgContoursA"
	cv::Mat ImgContoursB(ImgThreshB.size(), CV_8UC3, SCALAR_BLACK); //Declare a matrix with black pixels "ImgContoursB"

	cv::findContours(ImgThreshCopyA, ContoursA, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);  //Find contours of "ImgThreshCopyA"
	cv::drawContours(ImgContoursA, ContoursA, -1, SCALAR_WHITE, -1);  //Draw contours onto the "ImgContoursA" Matrix

	cv::findContours(ImgThreshCopyB, ContoursB, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);  //Find contours of "ImgThreshCopyB"
	cv::drawContours(ImgContoursB, ContoursB, -1, SCALAR_WHITE, -1);  //Draw contours onto the "ImgContoursB" Matrix

	cv::Mat BGSubtractionA(CamFeedA.rows, CamFeedA.cols, CV_8UC3);   //Declare matrix of same size as CameraFeed as "BGSubtractionA"
	cv::Mat BGSubtractionB(CamFeedB.rows, CamFeedB.cols, CV_8UC3);   //Declare matrix of same size as CameraFeed as "BGSubtractionB"


	BGSubtractionA.setTo(cv::Scalar(0, 0, 0)); //Set all pixels in "BGSubtractionA" to black
	BGSubtractionB.setTo(cv::Scalar(0, 0, 0)); //Set all pixels in "BGSubtractionB" to black
	
	CamFeedA.copyTo(BGSubtractionA, ImgContoursA);   // Copy "CamFeedA" onto "BGSubtractionA", using contours from "ImgContoursA"
	CamFeedB.copyTo(BGSubtractionB, ImgContoursB);   // Copy "CamFeedB" onto "BGSubtractionB", using contours from "ImgContoursB"
	

	cv::Mat FinalProductA;    //the Final result/product of program (first webcam)
	cv::Mat FinalProductB;    //the Final result/product of program (second webcam)

	cv::Canny(BGSubtractionA,         // input image
		FinalProductA,                // output image
		50,                           // low threshold (suggested value is "50")
		50);                          // high threshold (suggested value is "100")
									  //for better details on how this affects the visions:
									  //https://docs.opencv.org/3.4/da/d22/tutorial_py_canny.html
									  // "Hysteresis Thresholding"

	cv::Canny(BGSubtractionB,         // input image
		FinalProductB,                // output image
		50,                           // low threshold (suggested value is "50")
		50);                          // high threshold (suggested value is "100")
									  //for better details on how this affects the visions:
									  //https://docs.opencv.org/3.4/da/d22/tutorial_py_canny.html
									  // "Hysteresis Thresholding"
	
	cv::namedWindow("ORIGINAL-A", CV_WINDOW_NORMAL);      // note: you can use CV_WINDOW_NORMAL which allows resizing the window
	cv::namedWindow("PRODUCT-A", CV_WINDOW_NORMAL);       // or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
														  // CV_WINDOW_AUTOSIZE is the default show windows
    cv::imshow("ORIGINAL-A", CamFeedA);                   // show the original webcam feed (first)
	cv::imshow("PRODUCT-A", FinalProductA);               //show the image with the filters applied

	cv::namedWindow("ORIGINAL-B", CV_WINDOW_NORMAL);      // note: you can use CV_WINDOW_NORMAL which allows resizing the window
	cv::namedWindow("PRODUCT-B", CV_WINDOW_NORMAL);       // or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
														  // CV_WINDOW_AUTOSIZE is the default show windows
    cv::imshow("ORIGINAL-B", CamFeedB);                   // show the original webcam feed (second)
	cv::imshow("PRODUCT-B", FinalProductB);               //show the image with the Filter applied

	chCheckForEscKey = cv::waitKey(1);      // get key press in case user pressed esc

}

if (chCheckForEscKey != 27) {               // Check whether user hasn't pressed the ESC Key
	cv::waitKey(0);                         // If user hasn't pressed ESC key, display window.
}

return(0);

}