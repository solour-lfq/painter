#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <random>
using namespace std;
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

normal_distribution<float> n(0.0, 0.1);
default_random_engine e;

inline float noise()
{
	return n(e);
}

float y1(float x)
{
	return 4 * CV_PI * 0.0635 * x * x * exp(-0.5 * x * x);
}

float y2(float x)
{
	float mu = 1.414;
	float s2 = 0.5;
	return 1.0 / sqrt(2 * CV_PI * s2) * exp(-0.5 * (x - mu) * (x - mu) / s2);
}

float z1(float x, float y)
{
	float m1_x = 3.8, m1_y = 3.8;
	float m2_x = 3.0, m2_y = 5.0; 
	float m3_x = 2.0, m3_y = 1.0;
	float m4_x = 2.0, m4_y = 2.0;
	float s1 = 0.2, s2 = 0.2, s3 = 0.08, s4 = 0.08;
	float x1 = 0.0, x2 = 0.0, x3 = 0.0, x4 = 0.0;
	x1 = 1.0 / 2 / CV_PI / s1 * exp(-0.5 * (((x - m1_x) * (x - m1_x) / s1) + ((y - m1_y) * (y - m1_y) / s1)));
	x2 = 1.0 / 2 / CV_PI / s2 * exp(-0.5 * (((x - m2_x) * (x - m2_x) / s2) + ((y - m2_y) * (y - m2_y) / s2)));
	x3 = 1.0 / 2 / CV_PI / s3 * exp(-0.5 * (((x - m3_x) * (x - m3_x) / s3) + ((y - m3_y) * (y - m3_y) / s3)));
	x4 = 1.0 / 2 / CV_PI / s4 * exp(-0.5 * (((x - m4_x) * (x - m4_x) / s4) + ((y - m4_y) * (y - m4_y) / s4)));
	return (x1 + x2 + x3 + x4 + noise());
}

float z2(float x, float y)
{
	float m_x = 2.5, m_y = 2.5;
	float s = 1.0;
	float x_;
	x_ = 1.0 / 2 / CV_PI / s * exp(-0.5 * (((x - m_x) * (x - m_x) / s) + ((y - m_y) * (y - m_y) / s)));
	return x_ + noise() * 0.5;
}

float INIT_X = -5.0;
float END_X = 5.0;
float INIT_Y = -1.0;
float END_Y = 1.0;
float DELTA = 0.2;
