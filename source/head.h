#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <string>
#include <cmath>

using namespace cv;
using namespace std;

void init(Mat original, Mat _mask, string id);
Mat Result(Mat first, int counter, int fa);
int pendulum();
int friction();



extern vector <pair <int, int> > points;
extern pair <double, double> center_now;
extern string state; 
const int CAMERA_WIDTH = 400; //352
const int CAMERA_HEIGHT = 300; //288

