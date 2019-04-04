#include "KMatch.h"

//double matchingTime;
//Mat img_display;//img to display
//vector<resultmsg> resultmsgvec;//vector to store results

KMatch::KMatch()
{
}


KMatch::~KMatch()
{
}

void KMatch::matchingMethod(Mat img, Mat templ, int countToMatch, double value = 0.9, int pyrClass = 3, float accuracy = 1, bool drawMatch = true)
{
	double matchingStart = static_cast<double>(cvGetTickCount());

	resultmsgvec.clear();//clear vector

	int match_method = CV_TM_CCOEFF_NORMED;
	//pyramid 
	Mat pyrSrc = img.clone();
	Mat pyrTmp = templ.clone();

	double pyrScale = 1.0;//default
	for (int i = 0; i < pyrClass; i++) {
		pyrDown(pyrSrc, pyrSrc);
		pyrDown(pyrTmp, pyrTmp);
		pyrScale = pow(2, pyrClass);
	}

	img.copyTo(img_display);

	int result_cols = pyrSrc.cols - pyrTmp.cols + 1;
	int result_rows = pyrSrc.rows - pyrTmp.rows + 1;

	vector<Mat>m_result;
	Mat templ_rot;

	//angle to rot
	float step = accuracy;
	for (int angle = 0; angle < 360; angle += step)
	{
		Mat M = getRotationMatrix2D(Point(pyrTmp.cols / 2, pyrTmp.rows / 2), double(angle), 1.0);
		warpAffine(pyrTmp, templ_rot, M, pyrTmp.size(), INTER_NEAREST, BORDER_CONSTANT, Scalar(255));

		//define the result Mat
		Mat result = Mat(result_rows, result_cols, CV_32FC1);

		//output match result
		matchTemplate(pyrSrc, templ_rot, result, match_method);

		m_result.push_back(result);
	}

	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	//create a maxValue map to store results    
	Mat MaxValue = Mat(result_rows, result_cols, CV_32FC1, -1.0);
	Mat MaxValueAngle = Mat(result_rows, result_cols, CV_32FC1, 0.0);

	for (int i = 0; i < result_cols; i++)
	{
		for (int j = 0; j < result_rows; j++)
		{
			for (int n = 0; n < m_result.size(); n++)
			{
				if (m_result[n].at<float>(j, i) > MaxValue.at<float>(j, i))
				{
					MaxValue.at<float>(j, i) = m_result[n].at<float>(j, i);
					MaxValueAngle.at<float>(j, i) = n * step;
				}
			}
		}
	}

	//create a mask
	Mat result_mask = Mat::zeros(MaxValue.size(), CV_8UC1);

	for (int i = 0; i < countToMatch; i++)
	{
		minMaxLoc(MaxValue, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
		matchLoc = maxLoc;

		//draw Rect
		float maxAngle = MaxValueAngle.at<float>(maxLoc.y, maxLoc.x);
		double angle_rot = -maxAngle / 180 * 3.1415926;

		Point center = Point(matchLoc.x*pyrScale + templ.cols / 2, matchLoc.y*pyrScale + templ.rows / 2);

		resultmsg r;
		r.angle = maxAngle;
		r.center = center;
		r.score = maxVal;

		if (r.score > value)
		{
			resultmsgvec.push_back(r);
		}

		if (drawMatch)//draw match result
		{
			Point LT = Point(center.x - templ.cols / 2 * cos(angle_rot) + templ.rows / 2 * sin(angle_rot), center.y - templ.cols / 2 * sin(angle_rot) - templ.rows / 2 * cos(angle_rot));
			Point RT = Point(center.x + templ.cols / 2 * cos(angle_rot) + templ.rows / 2 * sin(angle_rot), center.y + templ.cols / 2 * sin(angle_rot) - templ.rows / 2 * cos(angle_rot));
			Point RB = Point(center.x + templ.cols / 2 * cos(angle_rot) - templ.rows / 2 * sin(angle_rot), center.y + templ.cols / 2 * sin(angle_rot) + templ.rows / 2 * cos(angle_rot));
			Point LB = Point(center.x - templ.cols / 2 * cos(angle_rot) - templ.rows / 2 * sin(angle_rot), center.y - templ.cols / 2 * sin(angle_rot) + templ.rows / 2 * cos(angle_rot));

			line(img_display, LT, RT, Scalar(0, 0, 0));
			line(img_display, RT, RB, Scalar(0, 0, 0));
			line(img_display, RB, LB, Scalar(0, 0, 0));
			line(img_display, LB, LT, Scalar(0, 0, 0));

			string angleText;
			if (accuracy >= 1)
			{
				printf("MaxLoc = (%d ,%d)\t;angle = %d\t;maxScore = %f \n", matchLoc.x*(int)pyrScale, matchLoc.y*(int)pyrScale, (int)maxAngle, maxAngle);
				angleText = to_string((int)maxAngle) + "   " + to_string(maxVal);
			}
			else
			{
				printf("MaxLoc = (%d ,%d)\t;angle = %d\t;maxScore = %f \n", matchLoc.x*(int)pyrScale, matchLoc.y*(int)pyrScale, (float)maxAngle, maxAngle);
				angleText = to_string((float)maxAngle) + "   " + to_string(maxVal);
			}

			CvFont m_font;
			putText(img_display, angleText, matchLoc*pyrScale, CV_FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0, 0, 255), 1);
		}

		int P0X = matchLoc.x - pyrTmp.cols / 2 > 0 ? matchLoc.x - pyrTmp.cols / 2 : 0;
		int P0Y = matchLoc.y - pyrTmp.rows / 2 > 0 ? matchLoc.y - pyrTmp.rows / 2 : 0;
		int P1X = matchLoc.x + pyrTmp.cols / 2 > MaxValue.cols ? MaxValue.cols : matchLoc.x + pyrTmp.cols / 2;
		int P1Y = matchLoc.y + pyrTmp.rows / 2 > MaxValue.rows ? MaxValue.rows : matchLoc.y + pyrTmp.rows / 2;
		Rect r1(Point(P0X, P0Y), Point(P1X, P1Y));
		result_mask(r1).setTo(255);

		MaxValue.setTo(-1.0, result_mask);
	}

	matchingTime = ((double)cvGetTickCount() - matchingStart) / cvGetTickFrequency();

	return;
}
