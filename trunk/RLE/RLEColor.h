#ifndef RLECOLOR_H
#define RLECOLOR_H

#include "rle.h"
#include <cv.h>
#include <vector>

using namespace std;

/** --------------------------------------------------------------------------------
--------------------------------------------------------------------------------
* @name : RLE
* @author : k_ta
* @date : 26/11/2009
* @Description:
				This class is a kind of wrapper to convert an IplImage color/grayscale
				with 1, 2 or 3 channels
--------------------------------------------------------------------------------
-------------------------------------------------------------------------------- **/

class RLE
{
	RLEImage *red;
	RLEImage *green;
	RLEImage *blue;
	RLEImage *gray;

	

	public :
		RLE(){};
		RLE(IplImage *img);
		~RLE();
		
		void CompressImage(IplImage *src);
		void DescompressImage(IplImage *img);
  

};




#endif//RLECOLOR_H