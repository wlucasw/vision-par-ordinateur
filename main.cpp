//
//  main.cpp
//  Features
//
//  Created by Jean-Marie Normand on 28/11/2017.
//  Copyright Â© 2017 Jean-Marie Normand. All rights reserved.
//


#include "main.h"
//#include <unistd.h>
#include <iomanip>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

// Main Method
int main(int argc, char * argv[]) {

   // the (default) name of the image file
	/*
   String imName1= DEFAULT_IMG_NAME1;
   String imName2= DEFAULT_IMG_NAME2;
  
   */
	String imName1= "mongolfière g.jpg";
	String imName2= "mongolfière d.jpg";
	
   // If we give an argument then open it instead of the default image
   if(argc == 2) {
      imName1 = argv[1];
   }
   if(argc == 3) {
      imName1 = argv[1];
      imName2 = argv[2];
   }
   
   // Reading the image (and forcing it to grayscale)
   im1 = imread(imName1);
   // reading second image
   im2 = imread(imName2);
   
   if(!im1.data || im1.empty() || (im1.rows == 0)|| (im1.cols == 0)) {
      cout << "Could not load image 1 !" << endl;
      cout << "Exiting now..." << endl;
      return 1;
   }
   
   if(!im2.data || im2.empty() || (im2.rows == 0)|| (im2.cols == 0)) {
      cout << "Could not load image 2 !" << endl;
      cout << "Exiting now..." << endl;
      return 1;
   }
   
   // Showing image 1
   namedWindow("Image 1");
   imshow("Image 1", im1);
   
   // Showing image 2
   namedWindow("Image 2");
   imshow("Image 2", im2);
   
   // First we create a feature detector for each image (but we could use only for both images !)
   _myFeatureDetector1.changeFeatureDetector(_CORNER_ORB);
   _myFeatureDetector2.changeFeatureDetector(_CORNER_ORB);
   
   // We detect keypoints in img1
   _myFeatureDetector1.setImage(im1);
   _myFeatureDetector1.detectFeatures();
   
   // Display keypoints of image1
   Mat imKP1; 
   imKP1 = _myFeatureDetector1.displayFeatures();
   namedWindow("KeyPoints Image 1");
   imshow("KeyPoints Image 1", imKP1);
   
   // We detect keypoints in img2
    _myFeatureDetector2.setImage(im2);
	_myFeatureDetector2.detectFeatures();
   
   // Display keypoints in image2
    Mat imKP2; 
    imKP2 = _myFeatureDetector2.displayFeatures();
    namedWindow("KeyPoints Image 2");
	imshow("KeyPoints Image 2", imKP2);
   
   // Computing Descriptors
   _myFeatureExtractor1.changeDescriptorExtractor(_DESC_ORB);
   _myFeatureExtractor2.changeDescriptorExtractor(_DESC_ORB);
   
   
   Mat imDesc1 = _myFeatureExtractor1.computeDescriptors(_myFeatureDetector1.getImage(), _myFeatureDetector1.getFeatures());
   Mat imDesc2 = _myFeatureExtractor2.computeDescriptors(_myFeatureDetector2.getImage(), _myFeatureDetector2.getFeatures());
 
	namedWindow("Extractors Image 1");
	imshow("Extractors Image 1", imDesc1);

	namedWindow("Extractors Image 2");
	imshow("Extractors Image 2", imDesc2);

   // First we create a feature detector for each image (but we could use only for both images !)
   _myDescriptorMatcher.changeMatchingMethod(_BRUTE_FORCE);
   
   // Matching keypoints
   _myDescriptorMatcher.match(_myFeatureExtractor1.getDescriptors(),
                              _myFeatureExtractor2.getDescriptors(), _myMatches, _myBestMatches);
   
   // Drawing matching result
   matchingResult = _myDescriptorMatcher.drawMatchingResults(im1,_myFeatureDetector1.getFeatures(),im2,_myFeatureDetector2.getFeatures(),_myBestMatches);
   
   namedWindow("Matching results");
   imshow("Matching results", matchingResult);
   
   
   //COMMENT CODE BELOW
   std::vector< Point2f > im1Pts;
   std::vector< Point2f > im2Pts;
   
   std::vector<KeyPoint> kpIm1 = _myFeatureDetector1.getFeatures();
   std::vector<KeyPoint> kpIm2 = _myFeatureDetector2.getFeatures();
   
   // Number of best matches
   cout << "*** NUMBER OF BEST MATCHES: "<<_myBestMatches.size()<<endl;
   
   for( int i = 0; i < _myBestMatches.size(); i++ )
   {
      //-- Get the keypoints from the good matches
      im1Pts.push_back( kpIm1[ _myBestMatches[i].queryIdx ].pt );
      im2Pts.push_back( kpIm2[ _myBestMatches[i].trainIdx ].pt );
   }
   // COMMENT CODE ABOVE
   
   
   
   // BELOW: FIND THE HOMOGRAPHY MATRIX AND STICH IMAGE
   if(im1Pts.size()>0) {
      homography = findHomography( im2Pts, im1Pts, RANSAC, 1.5 );
      cout << "Homography: " << homography << endl;
      // Use the Homography Matrix to warp the images
      cv::Mat result;
      
      if(!homography.empty()){
         warpPerspective(im2,result,homography,cv::Size(im1.cols+im2.cols,im2.rows));
         cv::Mat half(result,cv::Rect(0,0,im1.cols,im1.rows));
         im1.copyTo(half);
         
         imshow( "Warping", result );
      }
   }
   // ABOVE: FIND THE HOMOGRAPHY MATRIX AND STICH IMAGE
   
   
   // Launching the infinite loop
   retK = 0;
   
   while(retK != ESC_KEY) {
      
      // waiting for a key press
      retK = cv::waitKey(1);
   }

   // Done...
   return 0;
}
