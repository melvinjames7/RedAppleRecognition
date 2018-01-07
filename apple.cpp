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

Mat draw_circle(Mat image,Mat original)             //To draw red circle in B/W image
{
    int i=0,width=0,X=0,Y=0;
    for( int y = 0; y < image.rows; y++ )
    {
        for( int x = 0; x < image.cols; x++ )
        {
            float r = (image.at<Vec3b>(y,x)[2]);
            
            if(r==255)
            {  i++;}
            else
            {
                if(i>width)
                {
                    X=x;
                    Y=y;
                    width = i;
                    
                }i=0;
            }
        }i=0;
    }X-=width/2;
    
    circle(original, Point(X,Y), width/2+1, Scalar(0,0,255),3);
    namedWindow("Apple Detected", 1);
    
    imshow("Apple Detected", original);
    
    circle(image, Point(X,Y), width/2+1, Scalar(0,0,255),3);
    
    return image;
}
Mat RGBdetection(Mat image)
{
    Mat d_image = Mat::zeros( image.size(), image.type() );  //for new brightness adjusted image
    for( int y = 0; y < image.rows; y++ )
    { for( int x = 0; x < image.cols; x++ )
    {
        float r = (image.at<Vec3b>(y,x)[2]);
        float g = (image.at<Vec3b>(y,x)[1]);
        float b = (image.at<Vec3b>(y,x)[0]);
        
        if(r-g>50 && r-b>50){
            r = 255; g=255; b=255;      //white
        }
        else{
            r = 0; g = 0; b = 0;        //black
        }
        
        d_image.at<Vec3b>(y,x)[0] =
        saturate_cast<uchar>( b);
        d_image.at<Vec3b>(y,x)[1] =
        saturate_cast<uchar>( g );
        d_image.at<Vec3b>(y,x)[2] =
        saturate_cast<uchar>( r);
        
    }
    }
    
    medianBlur(d_image, d_image, 17);                   //to filter noise
    d_image = draw_circle(d_image,image);              //function call to draw circle
    
    return d_image;
}
Mat HSVdetection(Mat hsv_image)
{
    Mat d_image = Mat::zeros( hsv_image.size(), hsv_image.type() );  //for new brightness adjusted image
    for( int y = 0; y < hsv_image.rows; y++ )
    { for( int x = 0; x < hsv_image.cols; x++ )
    {
        float h = (hsv_image.at<Vec3b>(y,x)[2]);
        float s = (hsv_image.at<Vec3b>(y,x)[1]);
        float v = (hsv_image.at<Vec3b>(y,x)[0]);
        
        if(h>30 && s>70 &&v<35){
            
            h = 255; s=255; v=255;      //white
        }
        else{
            h = 0; s = 0; v = 0;
        }
        
        d_image.at<Vec3b>(y,x)[0] =
        saturate_cast<uchar>( v);
        d_image.at<Vec3b>(y,x)[1] =
        saturate_cast<uchar>( s );
        d_image.at<Vec3b>(y,x)[2] =
        saturate_cast<uchar>( h);
        
    }
    }
    
    medianBlur(d_image, d_image, 17);
    d_image = draw_circle(d_image,hsv_image);              //function call to draw circle
    return d_image;
}
Mat swap(Mat image)                                 //for swapping Red and Green
{
    Mat swap_image = Mat::zeros( image.size(), image.type() );
    for( int y = 0; y < image.rows; y++ )
    { for( int x = 0; x < image.cols; x++ )
    {
        float r = (image.at<Vec3b>(y,x)[2]);
        float g = (image.at<Vec3b>(y,x)[1]);
        
        swap_image.at<Vec3b>(y,x)[1] =
        saturate_cast<uchar>( r );
        swap_image.at<Vec3b>(y,x)[2] =
        saturate_cast<uchar>( g);
        
    }
    }
    return swap_image;
}
Mat rgbtohsv(Mat image)                             //Converting RGB to HSV
{
    Mat hsv_image = Mat::zeros( image.size(), image.type() ),hsvcopy,simphsv;
    for( int y = 0; y < image.rows; y++ )
    { for( int x = 0; x < image.cols; x++ )
    {
        float b = (image.at<Vec3b>(y,x)[0])/255.0f;
        float g = (image.at<Vec3b>(y,x)[1])/255.0f;
        float r = (image.at<Vec3b>(y,x)[2])/255.0f;
        
        double delta, min1, Max1;
        double h = 0, s, v;
        
        min1 = min(min(r, g), b);
        Max1 = max(max(r,g), b);
        delta = Max1 - min1;
        
        if (Max1 == 0.0)
            s = 0;
        else
            s = delta / Max1;
        
        if (s == 0)
            h = 0.0;
        
        else
        {
            if (r == Max1)
                h = fmod((g - b)/ delta,6);
            else if (g == Max1)
                h = 2 + (b - r) / delta;
            else if (b == Max1)
                h = 4 + (r - g) / delta;
            
            h *= 60;
            
            if (h < 0.0)
                h = h + 360;
        }
        v=Max1;
        hsv_image.at<Vec3b>(y,x)[0] =
        saturate_cast<uchar>( h);
        hsv_image.at<Vec3b>(y,x)[1] =
        saturate_cast<uchar>( s *255);
        hsv_image.at<Vec3b>(y,x)[2] =
        saturate_cast<uchar>( v *255);    }
    }
    simphsv = hsv_image;
    namedWindow("hsv Image", 1);
    imshow("hsv Image", simphsv);
    hsvcopy=HSVdetection(hsv_image);            //Function call for converting to binary image
    
    namedWindow("hsv Detection Image", 1);
    imshow("hsv Detection Image", hsvcopy);
    
    
    return simphsv;
}

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
