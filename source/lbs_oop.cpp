#include "head.h"

#define ORIGINAL true
#define MASK false

using namespace cv;
using namespace std;

struct hist
{
	int values[256];
	int x;
	int y;
	int coof;
	int size;
	string name;
	hist()
	{
		for(int i = 0; i < 256; i++)
		    values[i] = 0;
		coof = 1e9;
	}
};

int mask_hist[256] = {};
hist **image_hists;
vector <hist> result;
int quad_size = 48; // default
int imax, jmax;
pair <double, double> center_now;

struct images
{
	Mat Image, new_image;
	int lenght;
	int height;
	unsigned char* data;
	unsigned char* new_data;
	bool cr_mode;

	images(Mat& a, bool mode) //if mode == true that will be image mode else mask mode
	{
		Image = a;
		lenght = a.cols;
		height = a.rows;
		data = (unsigned char*)a.data;
		new_image.create(height - 2, lenght -2, CV_8UC1);	
		new_data = (unsigned char*)new_image.data;
		if(mode)
		{
			image_hists = new hist *[(lenght-2)/quad_size+1];
			for(int i = 0; i < (lenght-2)/quad_size+1; i++) image_hists[i] = new hist[(height-2)/quad_size+1];
		}
		else 
quad_size = (lenght-2)/2;
		cr_mode = mode;
		bin(mode);
	}
	~images(){
		if(cr_mode){
		  for(int i = 0; i < (lenght-2)/quad_size+1; i++)
		    delete [] image_hists[i];
	      delete [] image_hists;
		}
	}

	int find_data(int x, int y, int width)
	{
		return (y-1)*width + x-1;
	}

	void bin(bool mode) //if mode == true that will be image mode else mask mode
	{
		int a1, a2;
		for(int j = 2; j < height-1; j++)
		{
			for(int i = 2; i < lenght; i++)
			{
				a1 = find_data(i, j, lenght);
				a2 = find_data(i-1, j-1, lenght-2);
				new_data[a2] = 0;
				if(data[a1] < data[find_data(i, j-1, lenght)]) new_data[a2] +=1;
				if(data[a1] < data[find_data(i+1, j-1, lenght)]) new_data[a2] +=2;
				if(data[a1] < data[find_data(i+1, j, lenght)]) new_data[a2] +=4;
				if(data[a1] < data[find_data(i+1, j+1, lenght)]) new_data[a2] +=8;
				if(data[a1] < data[find_data(i, j+1, lenght)]) new_data[a2] +=16;
				if(data[a1] < data[find_data(i-1, j+1, lenght)]) new_data[a2] +=32;
				if(data[a1] < data[find_data(i-1, j, lenght)]) new_data[a2] +=64;
				if(data[a1] < data[find_data(i-1, j-1, lenght)]) new_data[a2] +=128;
				if(mode)
				{
					image_hists[(i-2)/quad_size][(j-2)/quad_size].values[new_data[a2]]++;
					image_hists[(i-2)/quad_size][(j-2)/quad_size].x = i;
					image_hists[(i-2)/quad_size][(j-2)/quad_size].y = j;
					imax = 	(i-2)/quad_size;
					jmax = (j-2)/quad_size;	
				}
				else mask_hist[new_data[a2]]++;
			}
		}
		
	}
};





bool mat_less (const hist & m1, const hist & m2)
{
	return (m1.coof < m2.coof);
}

void choose(string id)  //mask number from 0 to n
{
	hist res;
	int r;
	for(int j = 0; j < jmax; j++)
	{
		for(int i = 0; i < imax; i++)
		{
			res = image_hists[i][j];
			res.coof = 0;
			for(int g = 0; g < 256; g++)
			{
				r = (image_hists[i][j].values[g]+image_hists[i+1][j].values[g]+image_hists[i+1][j+1].values[g]+image_hists[i][j+1].values[g] - mask_hist[g]);
				image_hists[i][j].values[g] = 0;
				image_hists[i][j].x = 0;
				image_hists[i][j].y = 0;
				res.coof += r*r;
			}
			res.coof = res.coof/quad_size/quad_size;
			res.size = quad_size;
			res.name = id;
			result.push_back(res);
		}
	}
}

void init(Mat original, Mat _mask, string id)
{
	images mask(_mask, MASK);
	images first(original, ORIGINAL);
	choose(id);
	imax = 0;
	jmax = 0;
	for(int i = 0; i < 256; i++)
	    mask_hist[i] = 0;

}
Mat Result(Mat first, int counter, int coef)
{
	sort(result.begin(), result.end(), mat_less);
	for(int i = 0; i < counter; i++)
	{
		//cout<<result[i].coof<<endl;  
		if(result[i].coof < coef) 
		{
			//cout<<result[i].name<<' '<< result[i].x << ' ' << result[i].y << endl;
			center_now.first = result[i].x;
			center_now.second = result[i].y;
			rectangle(first, Point(result[i].x-result[i].size,result[i].y-result[i].size), Point(result[i].x+result[i].size,result[i].y+result[i].size), Scalar(0));
		}
	}
	result.clear();
	return first;
}
