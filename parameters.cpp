#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
using namespace cv;
int WIDTH = 1000;
int HEIGHT = 650;
Scalar BLACK = Scalar(0, 0, 0);
Scalar WHITE = Scalar(255, 255, 255);
Scalar RED = Scalar(0, 0, 255);
Scalar GREEN = Scalar(0, 255, 0);
Scalar BLUE = Scalar(255, 0, 0);
Scalar PURPLE = Scalar(0, 255, 255);
Scalar YELLOW = Scalar(255, 255, 0);
Scalar BROWN = Scalar(255, 0, 255);

float y1(float x)
{
	return (-1.0 + 2.0 / (1.0 + exp(-x)));
}

float y2(float x)
{
	return sin(x);
}

float z(float x, float y)
{
	return y / x;
}

float INIT_X = -5.0;
float END_X = 5.0;
float INIT_Y = -1.0;
float END_Y = 1.0;
float DELTA = 0.2;
