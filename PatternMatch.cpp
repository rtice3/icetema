#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>

#include <stdio.h>


/// Global Variables
cv::Mat img, templ, result;

std::string image_window = "Source Image";

int match_method = 5;

cv::VideoWriter video;

/// Function Headers
void MatchingMethod( int, void* );

/** @function main */
int main( int argc, char** argv ) {

    cv::VideoCapture cap(0); // open the video camera no. 0

    templ = cv::imread( "../target.jpg" , 1 );

    if (!cap.isOpened())  // if not success, exit program
    {
        std::cout << "Cannot open the video cam" << std::endl;
        return -1;
    }

    double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    video.open("out.avi", CV_FOURCC('X','V','I','D'), cap.get(CV_CAP_PROP_FPS), cv::Size(720, 720), true);

    if(!video.isOpened()) {
        std::cout << "Failed to open dest vid..." << std::endl;
        return -1;
    }

    std::cout << "Frame size : " << dWidth << " x " << dHeight << std::endl;

    cv::namedWindow(image_window, CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

    while (1) {
        bool bSuccess = cap.read(img); // read a new frame from video

        if (!bSuccess) {
           std::cout << "Cannot read a frame from video stream" << std::endl;
           break;
        }
        if(!img.empty())
            MatchingMethod( 0, 0 );
        if(cv::waitKey(30) > 0) {
            std::cout << "esc key is pressed by user" << std::endl;
            break; 
        }
    }
    return 0;
}

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod( int, void* )
{
  /// Source image to display
  cv::Mat img_display;
  img.copyTo( img_display );

  /// Create the result matrix
  int result_cols =  img.cols - templ.cols + 1;
  int result_rows = img.rows - templ.rows + 1;

  result.create( result_rows, result_cols, CV_32FC1 );

  /// Do the Matching and Normalize
  cv::matchTemplate( img, templ, result, match_method );
  cv::normalize( result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );

  /// Localizing the best match with minMaxLoc
  double minVal, maxVal; 
  cv::Point minLoc, maxLoc, matchLoc;

  cv::minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );

  /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
  if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
  else
    { matchLoc = maxLoc; }

  /// Show me what you got
  cv::rectangle( img_display, matchLoc, cv::Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), cv::Scalar::all(0), 2, 8, 0 );
  cv::rectangle( result, matchLoc, cv::Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), cv::Scalar::all(0), 2, 8, 0 );

  cv::Rect myROI((1280 - 720) / 2, 0, 720, 720);
  cv::Mat croppedImage = img_display(myROI);
  video << croppedImage;
  cv::imshow( image_window, croppedImage );

  return;
}