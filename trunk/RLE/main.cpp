#include <cv.h> 
#include <cxcore.h> 
#include <highgui.h> 
#include "rle.h"
#include "RLEColor.h"
#include <vector>
#include <time.h>
#include <iostream> 
using namespace std; 






int main(int argc, char* argv[]) 
{ 
	//loading an image
	IplImage *img = cvLoadImage("101.bmp",0);
	//if no image we finish
	if(!img)
	{
		cout <<"BAD IMAGE"<<endl;
		return 0;
	}

	//Creating a window for an image
	//cvNamedWindow( "IMAGE",CV_WINDOW_AUTOSIZE);

	//Showing the image loaded
	//cvShowImage("IMAGE",img);

	//Constructing the RLE

	cvNamedWindow( "INI",CV_WINDOW_AUTOSIZE);
	cvShowImage("INI",img);


	clock_t t1,t2;



	t1 = clock();
	RLEImage *myrle=new RLEImage(img);
	t2 = clock();

	//myrle->Erosion(0,0);

	cout<<"Comprimir:"<<endl;
	cout <<"--TIME:  "<<((double)( t2 - t1)) / CLOCKS_PER_SEC<<endl;



	t1 = clock();
	cvErode(img,img,0,5); 
	t2 = clock();


	cout<<"Erosion:"<<endl;
	cout <<"--TIME:  "<<((double)( t2 - t1)/CLOCKS_PER_SEC)<<endl;

	IplImage *rle=NULL;

	//Descompressing from RLE format

	t1 = clock();
	rle=myrle->Descompress();
	t2 = clock();

	cout<<"Descomprimir:"<<endl;
	cout <<"--TIME:  "<<((double)( t2 - t1)) / CLOCKS_PER_SEC<<endl;

	cvNamedWindow( "MAIN",CV_WINDOW_AUTOSIZE);
	cvShowImage("MAIN",rle);

	// Wait for a keystroke. If a positive argument is given, it will wait for  
	// that number of milliseconds and then continue.  If 0 is given, the 
	// program will wait indefinitely for a keypress. 

	cvWaitKey(0); 

	// Clean up (not really necessary, but good programming practice) 

	delete myrle;
	cvDestroyWindow("MAIN");
	cvDestroyWindow("INI");
	cvDestroyWindow("INI2");

	system("PAUSE"); 
	return EXIT_SUCCESS; 
}