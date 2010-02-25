#include "RLEColor.h"

#include <cv.h> 
#include <cxcore.h> 
#include <highgui.h> 
#include <iostream>

using namespace std;

RLE::RLE(IplImage *img)
{	
	red=NULL;
	green=NULL;
	blue=NULL;
	gray=NULL;
	CompressImage(img);

}

RLE::~RLE()
{	
	if(red!=NULL)delete red;
	if(red!=NULL)delete green;
	if(red!=NULL)delete blue;
	if(red!=NULL)delete gray;

}


void RLE::CompressImage(IplImage *src)
{
/*
	cout <<"align: "<< src->align<<endl;
	cout <<"alphaChannel: "<< src->alphaChannel<<endl;
	cout <<"BorderConst: "<< src->BorderConst<<endl;
	cout <<"BorderMode: "<< src->BorderMode<<endl;
	cout <<"channelSeq: "<< src->channelSeq<<endl;
	cout <<"colorModel: "<< src->colorModel<<endl;
	cout <<"dataOrde: "<< src->dataOrder<<endl;
	cout <<"depth: "<< src->depth<<endl;
	cout <<"height: "<< src->height<<endl;
	cout <<"ID: "<< src->ID<<endl;
	cout <<"imageId: "<< src->imageId<<endl;
	cout <<"imageSize: "<< src->imageSize<<endl;
	cout <<"nChannels: "<< src->nChannels<<endl;
	cout <<"nSize: "<< src->nSize<<endl;
	cout <<"origin: "<< src->origin<<endl;
	cout <<"width: "<< src->width<<endl;
	cout <<"widthStep: "<< src->widthStep<<endl;
*/
	if (src->nChannels == 1)
	{
		cout << src->align<<endl;
		gray=new RLEImage(src);
	}
	else if(src->nChannels == 2)
	{
		IplImage *Ired= cvCreateImage( cvGetSize(src), src->depth,1 );
		IplImage *Igreen = cvCreateImage( cvGetSize(src), src->depth,1 );
		cvSplit(src,Ired,Igreen,NULL,NULL);

		red=new RLEImage(Ired);
		green=new RLEImage(Igreen);

		//deleting the images in IplImage format that we have created
		cvReleaseImage(&Ired );
		cvReleaseImage(&Igreen);

	}

	else if(src->nChannels == 3 || src->nChannels == 4 )
	{

		IplImage *Ired= cvCreateImage( cvGetSize(src), src->depth,1 );
		IplImage *Igreen = cvCreateImage( cvGetSize(src), src->depth,1 );
		IplImage *Iblue = cvCreateImage( cvGetSize(src), src->depth,1 );
		cvSplit(src,Ired,Igreen,Iblue,NULL);

		red=new RLEImage(Ired);
		green=new RLEImage(Igreen);
		blue=new RLEImage(Iblue);
		
		// this show the 3 grayscale images
		/*
		cvNamedWindow( "Red",CV_WINDOW_AUTOSIZE);
		cvShowImage("Red",Ired);

		cvNamedWindow( "Green",CV_WINDOW_AUTOSIZE);
		cvShowImage("Green",Igreen);

		cvNamedWindow( "Blue",CV_WINDOW_AUTOSIZE);
		cvShowImage("Blue",Iblue);
		*/

		//deleting the images in IplImage format that we have created
		cvReleaseImage(&Ired );
		cvReleaseImage(&Igreen);
		cvReleaseImage(&Iblue);

	}
}
void RLE::DescompressImage(IplImage *src)
{
	if(gray!=NULL) src=gray->Descompress();
	else
	{
		IplImage *Ired;
		IplImage *Igreen;
		IplImage *Iblue;
		if(red!=NULL) Ired=gray->Descompress();
		if(green!=NULL) Igreen=green->Descompress();
		if(blue!=NULL) Iblue=blue->Descompress();

		cvNamedWindow( "Red",CV_WINDOW_AUTOSIZE);
		cvShowImage( "Red", Ired );

		cvNamedWindow( "Blue",CV_WINDOW_AUTOSIZE);
		cvShowImage( "Blue", Iblue );

		cvNamedWindow( "Green",CV_WINDOW_AUTOSIZE);
		cvShowImage( "Green", Igreen );

		cvWaitKey(0);

		cvDestroyWindow("Red");
		cvDestroyWindow("Blue");
		cvDestroyWindow("Blue");

		cvMerge(Ired,Iblue,Igreen,NULL,src);


	}

}