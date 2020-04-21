#include "head.h"

/** globals */
		VideoCapture capture(0);
		Mat3b canvas;
		const double FONT_SIZE = 0.7;
		string winName = "GUI v0.1";
		vector <pair <int, int> > points;
		/** description
		* 0 = esc, 1 = mode_pendulum, 2 = mode_rolling, 3 = set_mask, 4 = Start!
		*/
		const int N_buttons = 5;
		Rect buttons[N_buttons];
		string buttons_names[N_buttons] = {"esc", "pendulum", "rolling", "mask", "select points"};
		Mat choose_mask();
		void select_points();
		Mat mask;
		string state = "work";



void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{

	if( event == EVENT_LBUTTONDOWN)
	{
		points.emplace_back(x, y);
		printf(".");
	}
}


void callBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if (event == EVENT_LBUTTONDOWN)
    {
		for(int i = 0; i < N_buttons; i++)
		{
			if(buttons[i].contains(Point(x, y)))
			{
				//cout << buttons_names[i] << endl; 

				if(i == 0) state = "end";
				if(i == 1) state = "pendulum";
				if(i == 2) state = "friction";
				if(i == 3) choose_mask();
				if(i == 4) select_points();
			}
		}
		
    }  
    imshow(winName, canvas);
    waitKey(1);
}


Mat choose_mask()
{
	printf("click at the left upper conner and then right lower conner of the desired mask\n");
	Mat result, frame, mm;
	while( points.size() < 2)
	{
		capture.retrieve(frame);
		imshow("result", frame);
		waitKey(1);
		cvtColor(frame, mm, COLOR_RGB2GRAY);
		setMouseCallback("result", CallBackFunc, nullptr);
	}
	result = mm(Rect(points[0].first, points[0].second, points[1].first- points[0].first, points[1].second - points[0].second));
	imwrite("saved_photo/mask.png", result);
	mask = imread("saved_photo/mask.png", IMREAD_GRAYSCALE);
	printf("mask has been set!");
	points.clear();
	return result;
}

void select_points()
{
	points.clear();
	int n;
	printf("enter number of points: ");
	cin >> n;
	printf("click %d times\n", n);
	Mat result, frame, mm;
	while(points.size() < n)
	{
		capture.retrieve(frame);
		imshow("result", frame);
		setMouseCallback("result", CallBackFunc, nullptr);
		waitKey(1);
	}
	printf("all points selected!!!\n");
}

int main() 
{
    Mat frame, mm, img;
	namedWindow(winName);
    // Your buttons
	int button_length = 100, button_height = 50; 
	for(int i = 0; i < N_buttons; i++)
		buttons[i] = Rect(10, (button_height + 10) * i + 10, button_length, button_height);
	// The canvas
    canvas = Mat3b(CAMERA_HEIGHT, CAMERA_WIDTH + button_length + 20, Vec3b(0,0,0));
    // Draw the button
	for(int i = 0; i < N_buttons; i++)
	{
		canvas(buttons[i]) = Vec3b(200,200,200);
		putText(canvas(buttons[i]), buttons_names[i], Point(0, button_height/2), FONT_HERSHEY_TRIPLEX, FONT_SIZE, Scalar(0,0,0));
	}
	/*capture init*/
 
    capture.set(CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH);
    capture.set(CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT);
	//createTrackbar("brightness", "result", &a ,255, NULL);

	mask = imread("saved_photo/mask.png", IMREAD_GRAYSCALE);

	int coef = 30;
	createTrackbar("coef", winName, &coef ,255, nullptr);
	while(state != "end")
	{
		capture.retrieve(frame);
		cvtColor(frame, mm, COLOR_RGB2GRAY);
		init(mm, mask, "found!");
		img = Result(frame, 1, coef);
		if(state == "pendulum")
			pendulum();
		if(state == "friction")
			friction();
        imshow("result", img);
		setMouseCallback(winName, callBackFunc);
	    imshow(winName, canvas);
		waitKey(1);
	}
    return 0;
}