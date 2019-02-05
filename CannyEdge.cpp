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

///////////////////////////////////////////////////////////////////////////////////////////////////////

//Camera Feed Function
	
	cv::VideoCapture capWebcamA(2);            // declare a VideoCapture object and associate to webcam, 0 => use 1st webcam
	cv::VideoCapture capWebcamB(1);            //If needed, can change webcam feed (useful as Softarm project may use different webcam)

	if (capWebcamA.isOpened() == false) {                                // check if VideoCapture object was associated to webcam successfully
		std::cout << "error: A Webcam has not been accessed successfully\n\n";     // if not, print error message to standard out
		_getch();                                                       // and exit program
		return(0);                                                         
	}
	else if (capWebcamB.isOpened() == false) {
		std::cout << "error: A Webcam has not been accessed successfully\n\n";
		_getch();
		return(0);
	}
/////////////////////////////////////////////////////////////////////////////////////////////////
	
    //Image Containers used in program(Large Matrix):
	
	cv::Mat imgOriginalA;  
    cv::Mat imgOriginalB; // input image
	cv::Mat imgGrayscaleA;       // grayscale of input image
	cv::Mat imgGrayscaleB;
	cv::Mat imgBlurredA;         // intermediate blured image
	cv::Mat imgBlurredB;
	cv::Mat imgCannyA;
	cv::Mat imgCannyB;           // Canny edge image

//////////////////////////////////////////////////////////////////////////////////////////////////
	
	//Function applying the Gaussian Blur filter as well as the Canny Edge Detection.
	
	char charCheckForEscKey = 0;

	while (charCheckForEscKey != 27 && capWebcamA.isOpened() && capWebcamB.isOpened()) {            // until the Esc key is pressed or webcam connection is lost
		bool blnFrameReadSuccessfullyA = capWebcamA.read(imgOriginalA);            // get next frame
		bool blnFrameReadSuccessfullyB = capWebcamB.read(imgOriginalB);
		
		if (!blnFrameReadSuccessfullyA || !blnFrameReadSuccessfullyB || imgOriginalA.empty() || imgOriginalB.empty()) {                 // if frame not read successfully
			std::cout << "error: frame not read from webcams\n";                 // print error message to standard out
			break;                                                              // and jump out of while loop
		}

		cv::cvtColor(imgOriginalA, imgGrayscaleA, CV_BGR2GRAY);                   // convert to grayscale
		cv::cvtColor(imgOriginalB, imgGrayscaleB, CV_BGR2GRAY);

		cv::GaussianBlur(imgGrayscaleA,              // input image (should be "imgGrayscale")
			imgBlurredA,                // output image
			cv::Size(1, 1),            // smoothing window width and height in pixels
			1);                      // sigma value, determines how much the image will be blurred (suggested value is "1.8")

		cv::GaussianBlur(imgGrayscaleB,              // input image (should be "imgGrayscale")
			imgBlurredB,                // output image
			cv::Size(1, 1),            // smoothing window width and height in pixels
			1);                      // sigma value, determines how much the image will be blurred (suggested value is "1.8")


		cv::Canny(imgBlurredA,                       // input image
			imgCannyA,                          // output image
		    40,                               // low threshold (suggested value is "50")
			40);                             // high threshold (suggested value is "100")
		                                    //for better details on how this affects the visions:
		                                   //https://docs.opencv.org/3.4/da/d22/tutorial_py_canny.html
		                                  // "Hysteresis Thresholding"

		cv::Canny(imgBlurredB,                       
			imgCannyB,                          
			40,                              
			40);


/////////////////////////////////////////////////////////////////////////////////////////////////        
        
     // Declare Windows and Images inside Windows

		cv::namedWindow("imgOriginalA", CV_WINDOW_NORMAL);       // note: you can use CV_WINDOW_NORMAL which allows resizing the window
		cv::namedWindow("imgCannyA", CV_WINDOW_NORMAL);          // or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
		//cv::namedWindow("imgBlurredA", CV_WINDOW_NORMAL);		// CV_WINDOW_AUTOSIZE is the default show windows
		//cv::namedWindow("imgGrayscaleA", CV_WINDOW_NORMAL);

		cv::imshow("imgOriginalA", imgOriginalA);                 // show the original webcam feed
		cv::imshow("imgCannyA", imgCannyA);                       //show the image with the Canny edge filter applied
		//cv::imshow("imgBlurredA", imgBlurredA);
		//cv::imshow("imgGrayscaleA", imgGrayscaleA);

		cv::namedWindow("imgOriginalB", CV_WINDOW_NORMAL);       // note: you can use CV_WINDOW_NORMAL which allows resizing the window
		cv::namedWindow("imgCannyB", CV_WINDOW_NORMAL);          // or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
		//cv::namedWindow("imgBlurredB", CV_WINDOW_NORMAL);		// CV_WINDOW_AUTOSIZE is the default show windows
		//cv::namedWindow("imgGrayscaleB", CV_WINDOW_NORMAL);

		cv::imshow("imgOriginalB", imgOriginalB);                 // show the original webcam feed
		cv::imshow("imgCannyB", imgCannyB);                       //show the image with the Canny edge filter applied
		//cv::imshow("imgBlurredB", imgBlurredB);
		//cv::imshow("imgGrayscalB", imgGrayscaleB);

		charCheckForEscKey = cv::waitKey(1);        // delay (in ms) and get key press, if any
	}                                               // end while

//////////////////////////////////////////////////////////////////////////////////////////////////
	return(0);
}










