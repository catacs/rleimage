#ifndef RLE_H
#define RLE_H
#include <cv.h>
#include <vector>
#include <map>
using namespace std;



/** --------------------------------------------------------------------------------
--------------------------------------------------------------------------------
* @name : RLE
* @author : k_ta
* @date : 25/11/2009
* @Description:
				This class is the main class for create a RLE 
				image format.	
--------------------------------------------------------------------------------
-------------------------------------------------------------------------------- **/


class segment
{
	public:

		char color;
		int row_s;
		int column;
		int length;
		int first;
		int last;
		int blob;
		//next segment in the blob
		segment *b_next;
		segment *r_next;

		segment(){};

		segment(int row_s,int column_s,int length,int first,int last,int blob,segment *b_next,segment *r_next);

};

class row
{
	public:

		segment* first;
		segment* last;
		row(segment *first,segment *last);
};


class blob
{
	public:
		int size;
		segment *first;
		segment *last;
		blob(segment *first,segment *last);
};

class pos
{
public:
	int ini;
	int fin;
	pos* nxt;

	pos(int a,int b)
	{
		ini=a;fin=b;
	}
};


class RLEImage
{	
	vector<row *> rows;
	vector<blob *> blobs;
	segment *first;
	segment *last;
	int num_segments;
	int num_blobs;

	//Este vector se usa para almacenar los solapamientos a la hora de 
	//realizar la Erosion 
	map<segment *,pos*>  ovlp;

	

	

	// We use this variable to store the info from the OpenCv image 
	// that we receive to compress to RLE.

	IplImage *info;
	void Constructrows(IplImage *img);
	void Createblobs();
	int touchedblobs(segment* s,segment **ini);
	void fusionblobs(int b1,int b2);



	bool DeleteSegmentsOnRow(int r);
	void CalculateOverlapedSegments(int ant,int act);
	void ProcessErosion(int i);
public:



	


	




/** --------------------------------------------------------------------------------
--------------------------------------------------------------------------------
* @name : RLEImage();
* @author : k_ta
* @date : 25/11/2009
* @Description: Constructor
				
--------------------------------------------------------------------------------
-------------------------------------------------------------------------------- **/


		RLEImage();

		
/** --------------------------------------------------------------------------------
--------------------------------------------------------------------------------
* @name : RLEImage(IplImage *img);
* @author : k_ta
* @date : 25/11/2009
* @Description:
				Copy constructor
				
--------------------------------------------------------------------------------
-------------------------------------------------------------------------------- **/


		RLEImage(IplImage *img);

/** --------------------------------------------------------------------------------
--------------------------------------------------------------------------------
* @name : ~RLEImage(IplImage *img);
* @author : k_ta
* @date : 25/11/2009
* @Description:
				Destructor
				
--------------------------------------------------------------------------------
-------------------------------------------------------------------------------- **/


		~RLEImage();

/** --------------------------------------------------------------------------------
--------------------------------------------------------------------------------
* @name : Compress(IplImage *img);
* @author : k_ta
* @date : 26/11/2009
* @Description:
		        With this function we transform a IplImage in the RLE
				format.
				
--------------------------------------------------------------------------------
-------------------------------------------------------------------------------- **/
		void Compress(IplImage *img);

/** --------------------------------------------------------------------------------
--------------------------------------------------------------------------------
* @name : Descompress(IplImage *img);
* @author : k_ta
* @date : 26/11/2009
* @Description:
		        With this function we take the RLE created and restore it to the 
				IplImage format.
				
--------------------------------------------------------------------------------
-------------------------------------------------------------------------------- **/
		void insert_segment(segment *seg,segment *aux,int r,int c);
		IplImage* Descompress();


		void Erosion(char color,char background);
		void Dilatation(char color,char background);

		void CompressParche(IplImage *img);

};


#endif //RLE_H