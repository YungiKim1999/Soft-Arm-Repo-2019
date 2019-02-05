// OpenCV Canny Edge Detection Test Code in C++
//Standard Webcam on Laptop to be used for Demonstration purposes
//source from https://github.com/MicrocontrollersAndMore/OpenCV_3_Windows_10_Installation_Tutorial

////////////////////////////////////////////////////////////////////////////////////////////////////

//Libraries Used:

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<conio.h>           


int main() {

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
    //Camera Feed Function
	
	cv::VideoCapture capWebcamA(2);            // Declare the webcam used by changing number inside brackets
	cv::VideoCapture capWebcamB(1);            // This code is optimised for 2 camera feeds. 

	if (capWebcamA.isOpened() == false) {                                          // check if first VideoCapture object was associated
		std::cout << "error: A Webcam has not been accessed successfully\n\n";     // to webcam successfully if not,
		_getch();                                                                  // print error message to standard out and exit program
		return(0);                                                         
	}
	else if (capWebcamB.isOpened() == false) {                                    // check if first VideoCapture object was associated
		std::cout << "error: A Webcam has not been accessed successfully\n\n";    // to webcam successfully if not,
		_getch();                                                                 // print error message to standard out and exit program
		return(0);
	}
/////////////////////////////////////////////////////////////////////////////////////////////////
	
    //Image Containers used in program(Large Matrix):
	
	cv::Mat imgOriginalA;        // input image 1
    cv::Mat imgOriginalB;        // input image 2
	cv::Mat imgGrayscaleA;       // grayscale of input image 1
	cv::Mat imgGrayscaleB;       // grayscale of input image 2
	cv::Mat imgBlurredA;         // intermediate blurred image 1
	cv::Mat imgBlurredB;         // intermediate blurred image 2
	cv::Mat imgCannyA;           // Canny edge image 1
	cv::Mat imgCannyB;           // Canny edge image 2

//////////////////////////////////////////////////////////////////////////////////////////////////
	
	//Function applying the Gaussian Blur filter as well as the Canny Edge Detection.
	
	char charCheckForEscKey = 0;

	while (charCheckForEscKey != 27 && capWebcamA.isOpened() && capWebcamB.isOpened()) {    // until the Esc key is pressed 
		bool blnFrameReadSuccessfullyA = capWebcamA.read(imgOriginalA);                     // or either webcam connection 
		bool blnFrameReadSuccessfullyB = capWebcamB.read(imgOriginalB);                     // is lost get next frame
		
		if (!blnFrameReadSuccessfullyA || !blnFrameReadSuccessfullyB || imgOriginalA.empty() || imgOriginalB.empty()) {   // if either frame 
			std::cout << "error: frame not read from webcams\n";                // not read successfully print error message to standard out
			break;                                                              // and jump out of while loop
		}

		cv::cvtColor(imgOriginalA, imgGrayscaleA, CV_BGR2GRAY);                   // convert both camera feeds to grayscale
		cv::cvtColor(imgOriginalB, imgGrayscaleB, CV_BGR2GRAY);                    

		cv::GaussianBlur(imgGrayscaleA,   // input image (should be "imgGrayscaleA")
			imgBlurredA,                  // output image
			cv::Size(1, 1),               // smoothing window width and height in pixels(adjust size for difference blur (odd, same numbers))
			1);                           // sigma value, determines how much the image will be blurred (suggested value is "1.8")

		cv::GaussianBlur(imgGrayscaleB,   // input image (should be "imgGrayscaleB")
			imgBlurredB,                  // output image
			cv::Size(1, 1),               // smoothing window width and height in pixels (adjust size for difference blur (odd, same numbers))
			1);                           // sigma value, determines how much the image will be blurred (suggested value is "1.8")


		cv::Canny(imgBlurredA,            // input image
			imgCannyA,                    // output image
		    40,                           // low threshold (suggested value is "50")
			40);                          // high threshold (suggested value is "100")
		                                  //for better details on how this affects the visions:
		                                  //https://docs.opencv.org/3.4/da/d22/tutorial_py_canny.html
		                                  // "Hysteresis Thresholding"

		cv::Canny(imgBlurredB,            // input image          
			imgCannyB,                    // output image    
			40,                           // low threshold (suggested value is "50")                             
			40);                          // high threshold (suggested value is "100")
		                                  //for better details on how this affects the visions:
										  //https://docs.opencv.org/3.4/da/d22/tutorial_py_canny.html
										  // "Hysteresis Thresholding"

/////////////////////////////////////////////////////////////////////////////////////////////////        
        
     // Declare Windows and Images inside Windows

		cv::namedWindow("imgOriginalA", CV_WINDOW_NORMAL);      // note: you can use CV_WINDOW_NORMAL which allows resizing the window
		cv::namedWindow("imgCannyA", CV_WINDOW_NORMAL);         // or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
				                                                // CV_WINDOW_AUTOSIZE is the default show windows
		

		cv::imshow("imgOriginalA", imgOriginalA);               // show the original webcam feed (first)
		cv::imshow("imgCannyA", imgCannyA);                     //show the image with the Canny edge filter applied
		
		cv::namedWindow("imgOriginalB", CV_WINDOW_NORMAL);      // note: you can use CV_WINDOW_NORMAL which allows resizing the window
		cv::namedWindow("imgCannyB", CV_WINDOW_NORMAL);         // or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
		                                                        // CV_WINDOW_AUTOSIZE is the default show windows
		

		cv::imshow("imgOriginalB", imgOriginalB);                 // show the original webcam feed (second)
		cv::imshow("imgCannyB", imgCannyB);                       //show the image with the Canny edge filter applied

		charCheckForEscKey = cv::waitKey(1);        // delay (in ms) and get key press, if any
	}                                               // end while

//////////////////////////////////////////////////////////////////////////////////////////////////
	return(0);
}










