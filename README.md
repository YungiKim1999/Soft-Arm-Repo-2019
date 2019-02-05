# Soft-Arm-Repo-2019

INTRODUCTION

This Repository contains the vision enhancemet codes for the Soft Arm Summer Research Project for 2019.
Specifically, this repository contains a "Background Subtraction" code and a "Canny Edge Detection" code.
Both are written in C++ and require openCV version 2.4.13.6 or 2.4.13.7 in order to run.
Credits to Chris Dahms (MicrocontrollersAndMore) for tutorials as well as sample codes which made this possible.

BACKGROUND SUBTRACTION

The objective of this code is to subtract the background of the camera feed.
This will lead to the Soft Arm being the foreground while the background is eliminated.
This is useful as we can extract the Soft Arm and isolate it from the background, making the detection
of the Soft Arm easier.
Overview of Code:
1. First frame of camera feed (first frame which the camera sees when code is compiled) is taken as the background.
2. The background frame and the current camera frame get converted to a greyscale image (using cv::cvtcolor) as well as a 
   Gaussian blur (using cv::GaussianBlur)
3. The camera frame is compared to the background frame and the absolute difference is produced (using cv::absdiff).
4. The absolute difference is then goes through a threshold filter to increase visibility of differences (using cv::threshold)
5. The thresholded image is dilated and eroded to further increase major differences in the two frames and minimise 
   minor differences (using "cv::getStructuringElement" as the window for dilation and erosion and using "cv::dilate" and "cv::erode"
   to dilate and erode image)
6. The dilated and eroded image is then used to find contours/outline of the foreground (using cv:findContours)
7. This is drawn onto a black background (using cv::drawContours)

Note: More steps to add in order to reach final goal.

CANNY EDGE DETECTION

The objective of this code is to outline the edges of the camera feed.
This will lead to emphasizing the small differences in the shape of the Soft Arm which maybe obstructed due to colour.
This is useful to isolate features which may help identify as the Soft Arm as opposed to the background
Overview of Code:
1. The camera feed is changed to greyscale and also goes through a Gaussian blur (using cv::cvtcolor and cv::GaussianBlur)
2. This is then fed into the Canny Edge Detection function (using cv::Canny)
3. This is shown to user (end of code).

///////////////////////////////////////////////////////////END OF README//////////////////////////////////////////////////////////////


   
