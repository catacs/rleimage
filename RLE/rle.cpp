#include "rle.h"
#include <iostream>
#include <fstream>
#include <cxcore.h> 
#include <highgui.h> 
#include <time.h>
using namespace std;
#define BLACK 255

segment::segment(int row_s,int column_s,int length,int first,int last,int blob,segment *b_next,segment *r_next)
{

	this->row_s=row_s;
	this->column=column_s;
	this->length=length;
	this->first=first;
	this->last=last;
	this->blob=blob;

	//next segment in the blob
	this->b_next=b_next;
	this->r_next=r_next;
}

row::row(segment *first,segment *last)
{
	this->first=first;
	this->last=last;
}

blob::blob(segment *first,segment *last)
{
	this->first=first;
	this->last=last;
	size=0;


}


RLEImage::RLEImage()
{
	this->num_segments=0;
	this->num_blobs=0;
	this->first=NULL;
	this->last=NULL;
}

RLEImage::~RLEImage()
{

	cvReleaseImage(&info);

	//hay que mantener un puntero a la sigueinte posicion
		segment *nxt=first;
	for(segment* s=rows[0]->first;s!=NULL;s=nxt)
	{
		nxt=s->r_next;
		delete s;
	}

	for(int i=0;i<(int)rows.size();i++)
	{
		delete rows[i];

	}
	for(int i=0;i<(int)blobs.size();i++)
	{
		delete blobs[i];

	}
	blobs.clear();
	rows.clear();
}
RLEImage::RLEImage(IplImage *img)
{

	info=cvCreateImage(cvGetSize(img),img->depth,1 );
	this->num_segments=0;
	this->num_blobs=0;
	this->first=NULL;
	this->last=NULL;
	info->imageData=NULL;
	info->imageDataOrigin=NULL;
	Compress(img);
}


bool overlap(segment *s, segment *r)
{
	//si se superponen los segmentos ( si yo te estoy tocando y tu em estas  tocando
	// entonces hay contacto)
	if(((r->column >= s->column && r->column <= s->column+s->length-1)||
		(r->column+r->length-1 >= s->column && r->column+r->length-1 <= s->column+s->length-1)) ||
		((s->column >= r->column && s->column <= r->column+r->length)||
		(s->column+s->length-1 >= r->column && s->column+s->length-1 <= r->column+r->length-1)))
	{
		return true;
	}

	return false;
}

int RLEImage::touchedblobs(segment* s,segment **ini)
{
	int n=0;
	bool flag=true;



	// hay una posible optimizacion
	/*
	Si  he llegado a un segmento que toca 
	y he analizado  todos los segmentos que estoy tocando
	al llegar a un segmento que ya no toco no hay mas posibilidad de encontrar
	un segmento que pueda tocar	
	*/
	for(segment* r=rows[s->row_s+1]->first;r && r->row_s==s->row_s+1 && r->column>s->column;r=r->r_next)
	{

		if(overlap(s,r))
		{
			//cout <<endl<< s <<"  ---  "<< r <<"  "<< n;
			n++;
			if(flag)
			{
				flag=false;
				*ini=r;
			}

		}

	}
	return n;

}

void RLEImage::fusionblobs(int b1,int b2)
{

	/*
	como tengo una variable que me determina el numero del blob al que esta asignado
	el segmento si el blob es grande es mas eficiente midificar los segmentos
	del blob mas pequeño
	*/
	if(b1>=(int)blobs.size() || b2 >=(int) blobs.size())
	{
		return;
	}

	if(blobs[b1]->size > blobs[b2]->size)
	{
		segment *aux=blobs[b2]->first;
		for(int it=0;it <blobs[b2]->size; it++)
		{
			aux->blob=b1;
			aux=aux->b_next;
		}
		blobs[b1]->last->b_next=blobs[b2]->first;
		blobs[b1]->last=blobs[b2]->last;
		blobs[b1]->size=blobs[b1]->size+blobs[b2]->size;


		blobs[b2]->first=NULL;
		blobs[b2]->last=NULL;
		blobs[b2]->size=-1;
		//cout<<endl<< "BLOB2 "<< b2 <<" en " <<"BLOB1 "<< b1<<endl;
	}

	else
	{
		segment *aux=blobs[b1]->first;
		for(int it=0;it <blobs[b1]->size; it++)
		{
			aux->blob=b2;
			aux=aux->b_next;
		}

		blobs[b2]->last->b_next=blobs[b1]->first;
		blobs[b2]->last=blobs[b1]->last;
		blobs[b2]->size=blobs[b2]->size+blobs[b1]->size;

		blobs[b1]->first=NULL;
		blobs[b1]->last=NULL;
		blobs[b1]->size=-1;

		//cout<<endl<< "BLOB1 "<< b1 <<" en " <<"BLOB2 "<< b2<<endl;

	}

}

//creamos los blobs
void RLEImage::Createblobs()
{
	int n;
	segment* ini=NULL;
	//recorremos filas
	for(int r=0;r<(int)rows.size()-1;r++)
	{
		//por cada fila los segmentos
		for(segment *s=rows[r]->first;s && s->row_s==r;s=s->r_next)
		{
			//si el segmento no tiene blob asignado
			if(s->blob==-1)
			{
				//se lo asignamos
				blobs.push_back(new blob(s,s));
				blobs[blobs.size()-1]->size++;
				s->blob=blobs.size()-1;

			}

			//miramos los segmentos que toca el segmento que estamos analizando
			n=touchedblobs(s,&ini);

			//para cada uno de los segmentos
			for(int it=0;it<n;it++)
			{
				//si pertenece a un blob los fusionamos con el blob del segmento actual
				if(ini->blob!=-1)
				{
					if(s->blob!=ini->blob)
						fusionblobs(s->blob,ini->blob);
				}
				// si no pertenece a ningun blob le asignamos el blob actual
				else if(ini->blob==-1)
				{
					blobs[s->blob]->last->b_next=ini;
					blobs[s->blob]->last=ini;
					ini->blob=s->blob;
					blobs[s->blob]->size++;

				}

				//pasamos al siguiente de los segmentos que toca mi blob
				ini=ini->r_next;


			}

		}


	}

}

void print(IplImage *imag)
{
	ofstream f("print.html");
	for(int i=0;i<imag->height;i++)
	{
		for(int j=0;j<imag->width;j++)
			f <<(int)((uchar*)(imag->imageData + imag->widthStep*(i)))[j];
		f<<"<br>";
	}
	f.close();
}

void RLEImage::Constructrows(IplImage *img)
{
	ofstream f("ima.html");
	segment *seg=NULL;
	segment *aux_seg=NULL;
	static const char NEGRO=0;
	int first_on_row=0;
	bool flag_first=true;


	int counter=0;
	char actualpixel=0;//((uchar*)(img->imageData + img->widthStep*0))[0];

	//constructing rows
	for(int r=0;r<img->height;r++)
	{
		
		first_on_row=0;
		rows.push_back(new row(NULL,NULL));

		//updating the last segment of last row that has segments
		if(r!=0 && seg && seg->row_s==r-1)
		{
			rows[rows.size()-2]->last=aux_seg;

		}

		actualpixel=(((uchar*)(img->imageData + img->widthStep*r))[0]);

		f<<"<br>";
		for(int c=0;c<img->width;c++)
		{



			//Si el pixel actual es negro lo proceso
			if(actualpixel==NEGRO)
			{


				//miramos  si el pixel anterior era negro tambien

				//pero debemos tener en cuenta que el primer pixel de la fila no tiene anterior


				if(c==0)
				{
					if(num_segments==0)
					{
						seg= new segment(r,c,1,r*img->width+c,r*img->width+c,-1,NULL,NULL);
						aux_seg=seg;
						num_segments++;
						first=seg;
					
												
					}
					else
					{
						seg=new segment(r,c,1,r*img->width+c,r*img->width+c,-1,NULL,NULL);
						aux_seg->r_next=seg;
						aux_seg=seg;
						num_segments++;
						

					}
					
					rows[rows.size()-1]->first=aux_seg;
					rows[rows.size()-1]->last=aux_seg;

				}
				else if(c!=0)
				{
					if((((uchar*)(img->imageData + img->widthStep*r))[c-1])!=NEGRO)
					{
						if(num_segments==0)
						{
							seg= new segment(r,c,1,r*img->width+c,r*img->width+c,-1,NULL,NULL);
							aux_seg=seg;
							num_segments++;
							first=seg;
							first_on_row++;
						}
						else
						{
							seg=new segment(r,c,1,r*img->width+c,r*img->width+c,-1,NULL,NULL);
							aux_seg->r_next=seg;
							aux_seg=seg;
							num_segments++;
							
						}

						if(!rows[rows.size()-1]->first)
						{
							
							rows[rows.size()-1]->first=aux_seg;
							rows[rows.size()-1]->last=aux_seg;
						}
					}
					else
					{
						seg->length++;
						seg->last++;
					}

		
					
					

				}

			}
			
			//Actualizamos el pixel;
			if(c!=img->width-1)actualpixel=(((uchar*)(img->imageData + img->widthStep*r))[c+1]);

		}
	}

	if(seg && seg->row_s==rows.size()-1)
	{
			rows[rows.size()-2]->last=aux_seg;

	}

	f.close();
}

void RLEImage::Compress(IplImage *img)
{
	//Se contruyen las filas
	Constructrows(img);

	//Se contruyen las manchas

	Createblobs();
}


void RLEImage::insert_segment(segment *seg,segment *aux,int r,int c)
{

}

IplImage* RLEImage::Descompress()
{
	//Creating the Image in OpenCV format(IplImage)
	IplImage *img=cvCreateImage(cvSize(info->width,info->height),IPL_DEPTH_8U,1);

	segment *seg=first;
	const char NEGRO=0;


	ofstream dat("orig.html");
	int count=0;

	//cout <<endl;

/*	for(int i=0;i<img->height;i++)
	{
		for(int j=0;j<img->width;j++)
		{
			((uchar*)(img->imageData + img->widthStep*i))[j]=255;
			//cout<<(int)((uchar*)(img->imageData + img->widthStep*i))[j]<<" ";
		}
		//cout <<endl;
	}
*/
	/*for (int i=0;i<(int)blobs.size();i++)

		for (segment *s=blobs[i]->first;s;s=s->b_next)
	{
		for(int pos=s->first;pos<s->last+1;pos++)
		{

			((uchar*)(img->imageData + img->widthStep*(pos/(img->width))))[pos%(img->width)]=NEGRO;
			dat << pos<<":"<<(int)((uchar*)(img->imageData + img->widthStep*(pos/(img->width))))[pos%(img->width)]<<endl;
		
			//cout << i<< "    "<< blobs[i]->size<<endl ;
		}





	}*/

	clock_t t1,t2;

	t1=clock();


	while(seg!=NULL)
	{
		for(int pos=seg->first;pos<seg->last+1;pos++)
		{

			((uchar*)(img->imageData + img->widthStep*(pos/(img->width))))[pos%(img->width)]=NEGRO;
			//dat << pos<<":"<<(int)((uchar*)(img->imageData + img->widthStep*(pos/(img->width))))[pos%(img->width)]<<endl;
		}

		seg=seg->r_next;

	}

	t2=clock();

	cout << "----Descompresion bucle: "<<(double)(t2-t1)/CLOCKS_PER_SEC<<endl;
	/*
	for(int i=0;i<img->height;i++)
	{
		for(int j=0;j<img->width;j++)
			cout<<(int)((uchar*)(img->imageData + img->widthStep*i))[j]<<" ";
		cout <<endl;
	}*/

	dat.close();
	return img;
}

void RLEImage::Erosion(char color, char background)
{




}
void RLEImage::Dilatation(char color,char background)
{

}


