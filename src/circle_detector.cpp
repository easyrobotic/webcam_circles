
//OpenCV
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

//std
#include <iostream>
#include <cstdlib>
#include <vector>

//constants
const int GAUSSIAN_BLUR_SIZE = 7; //gb_size
const double GAUSSIAN_BLUR_SIGMA = 2; //gb_sigma
const double CANNY_EDGE_TH = 150; //ceth
const double HOUGH_ACCUM_RESOLUTION = 2; //har
const double MIN_CIRCLE_DIST = 40; //mincdist
const double HOUGH_ACCUM_TH = 70; //houghacth
const int MIN_RADIUS = 20; //minrad
const int MAX_RADIUS = 100; //maxrad


 cv::Mat compute_circle (cv::Mat image,  std::vector<cv::Vec3f> circles,const int gb_size,const double gb_sigma,const double ceth,const double har,const double mincdist, const double houghacth,const int minrad,const int maxrad){
  cv::Point center;
  int radius;
  cv::Mat gray_image;
  cv::Mat img;
  image.copyTo(img);
  cv::cvtColor(img, gray_image, CV_BGR2GRAY); // converting the image into a gray image
  cv::GaussianBlur(gray_image, gray_image, cv::Size(gb_size, gb_size), gb_sigma );
  cv::HoughCircles(gray_image, circles, CV_HOUGH_GRADIENT, har, mincdist, ceth, houghacth, minrad, maxrad );

  for(unsigned int ii = 0; ii < circles.size(); ii++ )
  {
      if ( circles[ii][0] != -1 )
      {
              center = cv::Point(cvRound(circles[ii][0]), cvRound(circles[ii][1]));
              radius = cvRound(circles[ii][2]);
              cv::circle(img, center, 5, cv::Scalar(0,0,255), -1, 8, 0 );// circle center in green
              cv::circle(img, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );// circle perimeter in red
      }
  }
  return img;

}

int main(int argc, char *argv[])
{
    cv::VideoCapture camera; //OpenCV video capture object
    cv::Mat image; //OpenCV image object
	int cam_id; //camera id . Associated to device number in /dev/videoX
    cv::Mat gray_image;
    std::vector<cv::Vec3f> circles;
    cv::Point center;
    int radius;

	//check user args
	switch(argc)
	{
		case 1: //no argument provided, so try /dev/video0
			cam_id = 0;
			break;
		case 2: //an argument is provided. Get it and set cam_id
			cam_id = atoi(argv[1]);
			break;
		default:
			std::cout << "Invalid number of arguments. Call program as: webcam_capture [video_device_id]. " << std::endl;
			std::cout << "EXIT program." << std::endl;
			break;
	}

	//advertising to the user
	std::cout << "Opening video device " << cam_id << std::endl;

    //open the video stream and make sure it's opened
    if( !camera.open(cam_id) )
	{
        std::cout << "Error opening the camera. May be invalid device id. EXIT program." << std::endl;
        return -1;
    }

    //Process loop. Capture and point feature extraction. User can quit pressing a key
    while(1)
	{
		//Read image and check it. Blocking call up to a new image arrives from camera.
        if(!camera.read(image))
		{
            std::cout << "No image" << std::endl;
            cv::waitKey();
        }

    //**************** Find circles in the image ****************************

        //clear previous circles
        circles.clear();

//original
        cv::Mat original_houghc = compute_circle (image, circles, GAUSSIAN_BLUR_SIZE, GAUSSIAN_BLUR_SIGMA, CANNY_EDGE_TH , HOUGH_ACCUM_RESOLUTION, MIN_CIRCLE_DIST, HOUGH_ACCUM_TH, MIN_RADIUS, MAX_RADIUS);

        cv::Mat output_image2 = compute_circle (image, circles, GAUSSIAN_BLUR_SIZE, GAUSSIAN_BLUR_SIGMA, CANNY_EDGE_TH , HOUGH_ACCUM_RESOLUTION, MIN_CIRCLE_DIST, HOUGH_ACCUM_TH, 50, MAX_RADIUS);

        cv::Mat output_image3 = compute_circle (image, circles, GAUSSIAN_BLUR_SIZE, GAUSSIAN_BLUR_SIGMA, CANNY_EDGE_TH , HOUGH_ACCUM_RESOLUTION, MIN_CIRCLE_DIST, HOUGH_ACCUM_TH, MIN_RADIUS, 250);

        cv::Mat output_image4 = compute_circle (image, circles, GAUSSIAN_BLUR_SIZE, GAUSSIAN_BLUR_SIGMA, 300 , HOUGH_ACCUM_RESOLUTION, MIN_CIRCLE_DIST, HOUGH_ACCUM_TH, MIN_RADIUS, MAX_RADIUS);

        cv::Mat output_image5 = compute_circle (image, circles, GAUSSIAN_BLUR_SIZE, GAUSSIAN_BLUR_SIGMA, CANNY_EDGE_TH , 9, MIN_CIRCLE_DIST, HOUGH_ACCUM_TH, MIN_RADIUS, MAX_RADIUS);

        cv::Mat output_image6 = compute_circle (image, circles, GAUSSIAN_BLUR_SIZE, GAUSSIAN_BLUR_SIGMA, CANNY_EDGE_TH , HOUGH_ACCUM_RESOLUTION, 4, HOUGH_ACCUM_TH, MIN_RADIUS, MAX_RADIUS);

        //***********Showing different created images *************//

        cv::Mat Output1;
        cv::Mat Output2;

        cv::Mat matArray[3] = {original_houghc,output_image2,output_image3};
        cv::hconcat(matArray,3,Output1);
        cv::Mat matArray2[3] = {output_image4,output_image5,output_image6};
        cv::hconcat(matArray2,3,Output2);
        cv::imshow("houghcircles: original, min_radius=50 , max_radius =250", Output1);
        cv::imshow("houghcircles: cannyedgeth= 300,  hough_accum_resolution =9, mincircledist=4", Output2);
//  cv::HoughCircles(gray_image, circles, CV_HOUGH_GRADIENT,  mincdist,  houghacth,  );
/*
const int GAUSSIAN_BLUR_SIZE = 7; //gb_size
const double GAUSSIAN_BLUR_SIGMA = 2; //gb_sigma
const double CANNY_EDGE_TH = 150; //ceth
const double HOUGH_ACCUM_RESOLUTION = 2; //har
const double MIN_CIRCLE_DIST = 40; //mincdist
const double HOUGH_ACCUM_TH = 70; //houghacth
const int MIN_RADIUS = 20; //minrad
const int MAX_RADIUS = 100; //maxrad
*/


    //********************************************************************

        //show image
      /*  cv::imshow("Output Window", output_image);
        cv::imshow("Output Window2", output_image2);

        cv::imshow("Output Window3", output_image3);

        cv::imshow("Output Window4", output_image4);

        cv::imshow("Output Window5", output_image5);
*/


		//Waits 1 millisecond to check if a key has been pressed. If so, breaks the loop. Otherwise continues.
        if((unsigned char)(cv::waitKey(1)) == 'q') break;
    }
}
