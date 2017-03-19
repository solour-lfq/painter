#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <sstream>
#include <string>
using namespace cv;
using namespace std;

void draw_line(Mat src, const Point& p1, const Point& p2, const Scalar& color, int thickness = 3)
{
	line(src, p1, p2, color, thickness, CV_AA);
}

void draw_point(Mat src, const Point& p1, const Scalar& color, int thickness = 9)
{
	line(src, p1, p1, color, thickness, CV_AA);
}

void draw_segment(Mat src, const Point& p1, const Point& p2, const Scalar& color, int thickness1 = 3, int thickness2 = 9)
{
	draw_line(src, p1, p2, color, thickness1);
	draw_point(src, p1, color, thickness2);
	draw_point(src, p2, color, thickness2);
}

void draw_arrow(Mat img, Point pStart, Point pEnd, const Scalar& color, int len = 30, int alpha = 20, int thickness = 3, int lineType = CV_AA)
{    
	const double PI = 3.1415926;    
	Point arrow;    
	double angle = atan2((double)(pStart.y - pEnd.y), (double)(pStart.x - pEnd.x));  
	line(img, pStart, pEnd, color, thickness, lineType);   
	arrow.x = pEnd.x + len * cos(angle + PI * alpha / 180);     
	arrow.y = pEnd.y + len * sin(angle + PI * alpha / 180);  
	line(img, pEnd, arrow, color, thickness, lineType);   
	arrow.x = pEnd.x + len * cos(angle - PI * alpha / 180);     
	arrow.y = pEnd.y + len * sin(angle - PI * alpha / 180);    
	line(img, pEnd, arrow, color, thickness, lineType);
}

void XY_display(Mat src, const Scalar& color, float (*f)(float), const Scalar& color2, float init_x, float end_x, float delta, float init_y, float end_y, bool seg = false, float r1 = 1.0 / 6.0, float r2 = 5.0 / 6.0, float r3 = 1.0 / 8.0, float r4 = 7.0 / 8.0)
{
	int width = src.cols, height = src.rows;
	Point o(width * r3, height * r2);
	Point ox(width * r3, height * r1);
	Point oy(width * r4, height * r2);
	draw_arrow(src, o, ox, color);
	draw_arrow(src, o, oy, color);
//	putText(src, "O", Point(o.x - 30, o.y + 30), 0, 1.0, color, 2);
	
	if ((init_x < 0) && (end_x > 0))
	{
		int temp = o.x + (oy.x - o.x) * (0.0 - init_x) / (end_x - init_x);
		draw_arrow(src, Point(temp, o.y), Point(temp, ox.y), color);
		putText(src, "X", Point(temp - 21, ox.y - 50), CV_FONT_HERSHEY_COMPLEX, 2.0, color, 3, CV_AA); 
	}
	
	if ((init_y < 0) && (end_y > 0))
	{
		int temp = o.y - (o.y - ox.y) * (0.0 - init_y) / (end_y - init_y);
		draw_arrow(src, Point(o.x, temp), Point(oy.x, temp), color);
		putText(src, "Y", Point(oy.x + 30, temp), CV_FONT_HERSHEY_COMPLEX, 2.0, color, 3, CV_AA); 
	}
	
	/*
	float local_min = f(init), local_max = f(init) , x = init, y = 0.0, rx = 0.0, ry = 0.0, rx_ = 0.0, ry_ = 0.0;
	while (x < end)
	{
		y = f(x);
		if (y < local_min)
		{
			local_min = y;
		}
		if (y > local_max)
		{
			local_max = y;
		}
		x += delta;
	}
	*/
	float x = init_x, y = 0.0, rx = 0.0, ry = 0.0, rx_ = 0.0, ry_ = 0.0;
	y = f(x);
	rx = o.x + (oy.x - o.x) * (x - init_x) / (end_x - init_x);
	ry = o.y - (o.y - ox.y) * (y - init_y) / (end_y - init_y);
	while (x < end_x)
	{
		rx_ = rx;
		ry_ = ry;
		y = f(x);
		rx = o.x + (oy.x - o.x) * (x - init_x) / (end_x - init_x);
		ry = o.y - (o.y - ox.y) * (y - init_y) / (end_y - init_y);
		if (seg)
		{
			draw_segment(src, Point(rx_, ry_), Point(rx, ry), color2, 5, 15);		
		}
		else
		{
			draw_line(src, Point(rx_, ry_), Point(rx, ry), color2, 5);
		}
		x += delta;
	}	
	ostringstream oss1, oss2, oss3;
	string interp;
	oss1 << "From x = [" << init_x << ", " << end_x << "]";
	interp = oss1.str();
	putText(src, interp, Point(o.x + 100, o.y + 20), CV_FONT_HERSHEY_COMPLEX, 0.5, color, 1, CV_AA);
	oss2 <<"delta = " << delta;
	interp = oss2.str();
	putText(src, interp, Point(o.x + 100, o.y + 40), CV_FONT_HERSHEY_COMPLEX, 0.5, color, 1,CV_AA);
	oss3 << "y = [" << init_y << ", " << end_y << "]";
	interp = oss3.str();
	putText(src, interp, Point(o.x + 100, o.y + 60), CV_FONT_HERSHEY_COMPLEX, 0.5, color, 1, CV_AA);
}

void XY_animate(Mat src, int iteration, const Scalar& color, float (*y)(float), const Scalar& color2, float init_x, float end_x, float delta, float init_y, float end_y, bool seg = false, float r1 = 1.0 / 6.0, float r2 = 5.0 / 6.0, float r3 = 1.0 / 8.0, float r4 = 7.0 / 8.0)
{
	int width = src.cols, height = src.rows;
	iteration = (iteration > 0) ? iteration : 1000;
	namedWindow("y=f(x)");
	for (int i = 0; i < iteration; ++i)
	{
		src = Mat(height, width, CV_8UC3, Scalar(0, 0, 0));
		waitKey(10);
		XY_display(src, color, y, color2, init_x + i * 0.1, end_x + i * 0.1, delta, init_y, end_y, seg, r1, r2, r3, r4);
		imshow("y=f(x)", src);
	}	
}

void XYZ_display(Mat src, const Scalar& color1, const Point& o, const Point& ox, const Point& oy, const Point& oz, const Scalar& color2, float (*f)(float, float), float end_x = 5.0, float end_y = 5.0, float end_z = 5.0, float delta_x = 0.3, float delta_y = 0.3)
{
	int width = src.cols, height = src.rows;
	draw_arrow(src, o, oz, color1, 30, 20, 2);
	draw_arrow(src, o, ox, color1, 30, 20, 2);
	draw_arrow(src, o, oy, color1, 30, 20, 2);
	float theta1 = atan((float)(oy.y - o.y) / (oy.x - o.x));
	float theta2 = atan((float)(ox.y - o.y) / (o.x - ox.x));
	Point next(0, 0), value(0, 0);
	float z = 0.0;
	int scale_x = end_x / delta_x, scale_y = end_y / delta_y;
	vector< vector<Point> > net;
	net.clear();
	for (int i = 1; i <= scale_x + 1; ++i)
	{
		vector<Point> temp;
		temp.clear();
		for (int j = 1; j <= scale_y + 1; ++j)
		{
			temp.push_back(Point(0, 0));
		}
		net.push_back(temp);
	}
	for (int ix = 1; ix <= scale_x; ++ix)
	{
		for (int iy = 1; iy <= scale_y; ++iy)
		{
			float x = ix * delta_x, y = iy * delta_y;
			float rx = x / end_x * sqrt(float((o.x - ox.x) * (o.x - ox.x) + (o.y - ox.y) * (o.y - ox.y)));
			float ry = y / end_y * sqrt(float((o.x - oy.x) * (o.x - oy.x) + (o.y - oy.y) * (o.y - oy.y)));
			next = Point(o.x - rx * cos(theta2) + ry * cos(theta1), o.y + rx * sin(theta2) + ry * sin(theta1));
			z = f(x, y);
			value = Point(next.x, next.y - z * (o.y - oz.y) / end_z);
			net[ix][iy] = value;
			draw_point(src, next, color2, 2);
			draw_segment(src, next, value, Scalar(0, 255 / scale_x * ix, 255 / scale_y * iy), 1, 3);
		}
	}
	for (int i = 2; i <= scale_x - 1; ++i)
	{
		for (int j = 2; j <= scale_y - 1; ++j)
		{
			draw_line(src, net[i][j], net[i - 1][j], Scalar(0, 255 / scale_x * (i - 1), 255 / scale_y * j), 1);
			draw_line(src, net[i][j], net[i][j - 1], Scalar(0, 255 / scale_x * i, 255 / scale_y * (j - 1)), 1);
			draw_line(src, net[i][j], net[i + 1][j], Scalar(0, 255 / scale_x * (i + 1), 255 / scale_y * j), 1);
			draw_line(src, net[i][j], net[i][j + 1], Scalar(0, 255 / scale_x * i, 255 / scale_y * (j + 1)), 1);			
		}	
	}
	ostringstream oss1, oss2, oss3;
	string interp;
	oss1 << "From x = [0, " << end_x << "], delta = " << delta_x;
	interp = oss1.str();
	putText(src, interp, Point(20, 100), CV_FONT_HERSHEY_COMPLEX, 0.3, color1, 1, CV_AA);
	oss2 << "From y = [0, " << end_y << "], delta = " << delta_y;
	interp = oss2.str();
	putText(src, interp, Point(20, 120), CV_FONT_HERSHEY_COMPLEX, 0.3, color1, 1, CV_AA);
	oss3 << "From z = [0, " << end_z << "]";
	interp = oss3.str();
	putText(src, interp, Point(20, 140), CV_FONT_HERSHEY_COMPLEX, 0.3, color1, 1, CV_AA);
	putText(src, "X", Point(ox.x - 20, ox.y + 30), CV_FONT_HERSHEY_COMPLEX, 1, color1, 1, CV_AA);
	putText(src, "Y", Point(oy.x + 10, oy.y + 10), CV_FONT_HERSHEY_COMPLEX, 1, color1, 1, CV_AA);
	putText(src, "Z", Point(oz.x - 10, oz.y - 10), CV_FONT_HERSHEY_COMPLEX, 1, color1, 1, CV_AA);
}

void XYZ_animate(Mat src, const Scalar& color1, const Point& o, const Point& ox, const Point& oy, const Point& oz, const Scalar& color2, float (*f)(float, float), float end_x = 5.0, float end_y = 5.0, float end_z = 5.0, float delta_x = 0.1, float delta_y = 0.1)
{
	int width = src.cols, height = src.rows;
	draw_arrow(src, o, oz, color1, 30, 20, 2);
	draw_arrow(src, o, ox, color1, 30, 20, 2);
	draw_arrow(src, o, oy, color1, 30, 20, 2);
	float theta1 = atan((float)(oy.y - o.y) / (oy.x - o.x));
	float theta2 = atan((float)(ox.y - o.y) / (o.x - ox.x));
	Point next(0, 0), value(0, 0);
	float z = 0.0;
	int scale_x = end_x / delta_x, scale_y = end_y / delta_y;
	vector< vector<Point> > net;
	net.clear();
	for (int i = 1; i <= scale_x + 1; ++i)
	{
		vector<Point> temp;
		temp.clear();
		for (int j = 1; j <= scale_y + 1; ++j)
		{
			temp.push_back(Point(0, 0));
		}
		net.push_back(temp);
	}
	namedWindow("A");
	while(1)
	{
		src = Mat(height, width, CV_8UC3, Scalar(0, 0, 0));
		draw_arrow(src, o, oz, color1, 30, 20, 2);
		draw_arrow(src, o, ox, color1, 30, 20, 2);
		draw_arrow(src, o, oy, color1, 30, 20, 2);
		waitKey(50);
		for (int ix = 1; ix <= scale_x; ++ix)
		{
			for (int iy = 1; iy <= scale_y; ++iy)
			{	
				float x = ix * delta_x, y = iy * delta_y;
				float rx = x / end_x * sqrt(float((o.x - ox.x) * (o.x - ox.x) + (o.y - ox.y) * (o.y - ox.y)));
				float ry = y / end_y * sqrt(float((o.x - oy.x) * (o.x - oy.x) + (o.y - oy.y) * (o.y - oy.y)));
				next = Point(o.x - rx * cos(theta2) + ry * cos(theta1), o.y + rx * sin(theta2) + ry * sin(theta1));
				z = (rand() % 500) / 100.0;
				value = Point(next.x, next.y - z * (o.y - oz.y) / end_z);
				net[ix][iy] = value;
				draw_point(src, next, color2, 2);
				draw_segment(src, next, value, Scalar(0, 255 / scale_x * ix, 255 / scale_y * iy), 1, 3);
			}
		}
		for (int i = 2; i <= scale_x - 1; ++i)	
		{
			for (int j = 2; j <= scale_y - 1; ++j)
			{
				draw_line(src, net[i][j], net[i - 1][j], Scalar(0, 255 / scale_x * (i - 1), 255 / scale_y * j), 1);
				draw_line(src, net[i][j], net[i][j - 1], Scalar(0, 255 / scale_x * i, 255 / scale_y * (j - 1)), 1);
				draw_line(src, net[i][j], net[i + 1][j], Scalar(0, 255 / scale_x * (i + 1), 255 / scale_y * j), 1);
				draw_line(src, net[i][j], net[i][j + 1], Scalar(0, 255 / scale_x * i, 255 / scale_y * (j + 1)), 1);			
			}	
		}
		imshow("A", src);
	}
}




