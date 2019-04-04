#pragma once
#include "opencv2/imgproc.hpp"
#include "matchingMethod.h"
#include <vector>

using namespace cv;
using namespace std;
class KMatch
{
public:
	KMatch();
	~KMatch();
	double matchingTime;
	Mat img_display;//img to display
	/*
	img				source image
	templ			template
	countToMatch	count to match
	value			the minimun score ,default = 0.9
	pyrClass		pyramid  class ,default = 3
	accuracy		accuracy(it should be less then 10 degree) ,default =1.0
	drawMatch		wether or not to draw rect ,default=true.
	*/
	void matchingMethod(cv::Mat img, cv::Mat templ, int countToMatch, double value, int pyrClass, float accuracy, bool drawMatch);
	struct resultmsg
	{
		double score;
		Point center;
		float angle;
	};
	vector<resultmsg> resultmsgvec;
private:

};






