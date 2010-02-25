#include <cv.h> 
#include <cxcore.h> 
#include <highgui.h> 
#include "rle.h"
#include "RLEColor.h"
#include <vector>

#include <iostream> 
using namespace std; 





 
int main(int argc, char* argv[]) 
{ 
 //loading an image
	IplImage *img = cvLoadImage("lenaBinaria.bmp",0);
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


	RLEImage *myrle=new RLEImage(img);







	IplImage *rle=NULL;

//Descompressing from RLE format
	rle=myrle->Descompress();
	
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