//

//
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

int main(int argc, const char * argv[]) {
    
    string imageName("/Users/melvinjames/Desktop/Apples.png");
    if( argc > 1)
    {
        imageName = argv[1];
    }
    
    Mat image;
    image = imread(imageName.c_str(), IMREAD_COLOR); // Read the file
    
    
    if( image.empty() )                      // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.
    imshow( "Display window", image );
    
    // For adjusting Brightness
    Mat new_image = Mat::zeros( image.size(), image.type() );
    for( int y = 0; y < image.rows; y++ )
    {
        for( int x = 0; x < image.cols; x++ )
        {
            for( int c = 0; c < 3; c++ )
            {
                new_image.at<Vec3b>(y,x)[c] =
                saturate_cast<uchar>( image.at<Vec3b>(y,x)[c]  + 50 );
            }
        }
    }
    namedWindow("Brightness Adjusted Image", 1);
    
    imshow("Brightness Adjusted Image", new_image);
    //
    //For RGB to HSV
    Mat hsv_img = Mat::zeros( image.size(), image.type() );
    
    hsv_img = rgbtohsv(image);
    namedWindow("HSV Image", 1);
    
    imshow("HSV Image", hsv_img);
    //
    //Swap Colors
    Mat swap_img = Mat::zeros( image.size(), image.type() );
    
    swap_img = swap(image);
    
    namedWindow("Swapped Image", 1);
    
    imshow("Swapped Image", swap_img);
    //
    //Whiten red and draw circle
    Mat redetect_img = Mat::zeros( image.size(), image.type() );
    
    redetect_img = RGBdetection(image);
    
    namedWindow("Detection Image", 1);
    
    imshow("Detection Image", redetect_img);
    //
    
    
    
    waitKey(0);                         //to view output
    return 0;
}
