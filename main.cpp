#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
using namespace cv;
using namespace std;

extern int HEIGHT;
extern int WIDTH;
extern float INIT_X;
extern float END_X;
extern float INIT_Y;
extern float END_Y;
extern float DELTA;
extern Scalar BLACK;
extern Scalar WHITE;
extern Scalar RED;
extern Scalar BLUE;
extern Scalar GREEN;
extern Scalar PURPLE;
extern Scalar YELLOW;
extern Scalar BROWN;

extern float y1(float);
extern float y2(float);
extern float z(float, float);

void draw_line(Mat, const Point&, const Point&, const Scalar&, int thickness = 3);

void draw_point(Mat, const Point&, const Scalar&, int thickness = 9);

void draw_segment(Mat, const Point&, const Point&, const Scalar&, int thickness1 = 3, int thickness2 = 9);

void draw_arrow(Mat, Point, Point, const Scalar&, int len = 30, int alpha = 20, int thickness = 3, int lineType = CV_AA);

void XY_display(Mat, const Scalar&, float (*y)(float), const Scalar&, float, float, float, float, float, bool seg = false, float r1 = 1.0 / 6.0, float r2 = 5.0 / 6.0, float r3 = 1.0 / 8.0, float r4 = 7.0 / 8.0);

void XY_animate(Mat, int, const Scalar&, float (*y)(float), const Scalar&, float, float, float, float, float, bool seg = false, float r1 = 1.0 / 6.0, float r2 = 5.0 / 6.0, float r3 = 1.0 / 8.0, float r4 = 7.0 / 8.0);

void XYZ_display(Mat, const Scalar&, const Point&, const Point&, const Point&, const Point&, const Scalar&, float (*f)(float, float), float end_x = 5.0, float end_y = 5.0, float end_z = 5.0, float delta_x = 0.3, float delta_y = 0.3);

void XYZ_animate(Mat, const Scalar&, const Point&, const Point&, const Point&, const Point&, const Scalar&, float (*f)(float, float), float end_x = 5.0, float end_y = 5.0, float end_z = 5.0, float delta_x = 0.1, float delta_y = 0.1);

int main()
{
	Mat canvas(HEIGHT, WIDTH, CV_8UC3, BLACK);
//	XY_animate(canvas, 0, WHITE, y2, YELLOW, INIT_X, END_X, DELTA, INIT_Y, END_Y);
	XYZ_animate(canvas, WHITE, Point(300, 300), Point(120, 450), Point(800, 400), Point(300, 50), RED, z);
	waitKey(0);
	return 0;
}
