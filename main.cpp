#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

#include "KMatch.h"
//test
using namespace std;
using namespace cv;
bool use_mask;
Mat mask;
const char* image_window = "Source Image";
const char* result_window = "Result window";

int main(int argc, char** argv)
{
	/*check parameter*/
	if (argc < 3)
	{
		cout << "Not enough parameters" << endl;
		cout << "Usage:\n./MatchTemplate_Demo <image_name> <template_name> [<mask_name>]" << endl;
		return -1;
	}
	cv::Mat img = cv::imread(argv[1], IMREAD_GRAYSCALE);
	cv::Mat templ = cv::imread(argv[2], IMREAD_GRAYSCALE);
	if (argc > 3) {
		use_mask  = true;
		mask = imread(argv[3], IMREAD_COLOR);
	}
	if (img.empty() || templ.empty() || (use_mask && mask.empty()))
	{
		cout << "Can't read one of the images" << endl;
		return -1;
	}
	/*check parameter*/



	namedWindow(image_window, WINDOW_AUTOSIZE);
	//namedWindow(result_window, WINDOW_AUTOSIZE);
	KMatch m_KMatch;
	m_KMatch.matchingMethod(img, templ, 7, 0.9, 3, 10, false);

	cout << "matching time :" << m_KMatch.matchingTime << "us" << endl;

	//imshow(image_window, m_KMatch.img_display);
	int n = resultmsgvec.size();
	for (int i = 0; i < m_KMatch.resultmsgvec.size(); i++)
	{
		KMatch sub_Match;

	
	
	}
	//print results
	//vector<resultmsg>::iterator it;
	cout << resultmsgvec.size() << endl;
	for (int i = 0; i < resultmsgvec.size(); i++)
	{
		cout<<resultmsgvec[i].angle<<endl;	
	}

	waitKey(0);
	return 0;
}

