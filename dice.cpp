#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace cv;
using namespace std;
Mat src; Mat src_gray;
Mat dice[7];
int thresh = 70;
RNG rng(0);
void thresh_callback();
int IsValid (int x,int y,Point tl,Point br)
{
	if(x<0||y<0||x>=(br.x-tl.x)||y>=(br.y-tl.y))
		return 0;
	else return 1;
}
void dfs_visit(int i,int j,Point tl, Point br, Mat b)

{
	int k,l;
	b.at<uchar>(i,j)=255;
	for( k=i-1;k<i+2;k++) 
	{
		for( l=j-1;l<j+2;l++)
		{
			

			if(IsValid(k,l,tl,br))
				if(b.at<uchar>(k,l)==0)
				{
					imshow("visited",b);
					waitKey(1);
					dfs_visit (k,l,tl,br,b);
				}		
		}
	}
}
int main()
{  
	src = imread("dice.png", 1 );
	cvtColor( src, src_gray, CV_BGR2GRAY );
  	GaussianBlur( src_gray, src_gray, Size(3,3) ,1.5,1.5);
 	for(int i=0;i<src_gray.rows;i++)
		for(int j=0;j<src_gray.cols;j++)
		{
			if(src_gray.at<uchar>(i,j)<190)
					src_gray.at<uchar>(i,j)=0;
			else
				src_gray.at<uchar>(i,j)=255;		
		}
	for(int i=0;i<7;i++)
	{
		dice[i]=Mat  (150,150,CV_8UC1,Scalar(0));
	}	
  	thresh_callback();
  	imshow("asd",src_gray);
    waitKey(0);
    return(0);
}
void thresh_callback()
{
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
  	Canny( src_gray, canny_output, thresh, thresh*3, 3 );
  	Point tl[7],br[7];
  	findContours( canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  	Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  	int pips[contours.size()];
  	for( int i = 0; i< contours.size(); i++ )
  	{
     	// int count = 0;
  	     Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
  	     drawContours( src, contours, i, color, 2, 8, hierarchy, 0, Point() );
  	     rectangle( src,boundingRect( contours[i] ).tl(), boundingRect( contours[i] ).br(), color, 2, 8, 0 );
  	     tl[i]=boundingRect( contours[i] ).tl();
  	     br[i]=boundingRect( contours[i] ).br();
    	/* for(int i=tl.x;i<br.x;i++)
    	 	for (int j=tl.y;j<br.y;j++)
		 	{
    	 	if(src_gray.at<uchar>(i,j)==0)
			{
				dfs_visit(i,j,tl,br,src_gray);
				count++;

			}
			pips[i]=count;	
			cout<<count<<endl;
   		}*/
    }   
    for(int i=0;i<7;i++)
    {
    	dice[i]=Mat (br[i].x-tl[i].x,br[i].y-tl[i].y,CV_8UC1,Scalar(0));
    	for(int j=0;j<dice[i].rows;j++)
    		for(int k=0;k<dice[i].cols;k++)
    		{
    			dice[i].at<uchar>(j,k)=src_gray.at<uchar>(j+tl[i].y,k+tl[i].x);
    		}
    }
	
 /*   for(int i=0;i<7;i++)
    {
		dfs_visit(0,0,tl[i],br[i],dice[i]);
		dfs_visit(0,br[i].x-tl[i].x,tl[i],br[i],dice[i]);
		dfs_visit(br[i].y-tl[i].y,br[i].x-tl[i].x,tl[i],br[i],dice[i]);
		dfs_visit(br[i].y-tl[i].y,0,tl[i],br[i],dice[i]);
	}*/
	int count[contours.size()];
	for(int a=0;a<7;a++)
	{
		floodFill(dice[a],cv::Point(0,0),Scalar(255));
		floodFill(dice[a],cv::Point(0,dice[a].rows-1),Scalar(255));
		floodFill(dice[a],cv::Point(dice[a].cols-1,0),Scalar(255));
		floodFill(dice[a],cv::Point(dice[a].cols-1,dice[a].rows-1),Scalar(255));
		vector<Vec4i> hierarchy1;
		vector<vector<Point> > contour1;
		findContours( dice[a], contour1,hierarchy1, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
		count[a]=contour1.size()-1;
		cout<<count[a]<<endl;
	}	
//	imshow("dice1",dice[0]);
//	imshow("dice2",dice[1]);
//	imshow("dice3",dice[2]);
//	imshow("dice4",dice[3]);
//	imshow("dice5",dice[4]);
//	imshow("dice6",dice[5]);
//	imshow("dice7",dice[6]);
  	cout<<contours.size()<<endl;
  	imshow( "Contours", src );
}

