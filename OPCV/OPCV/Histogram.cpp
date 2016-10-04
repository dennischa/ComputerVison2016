#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace std;
using namespace cv;

class Histogram1D {
private:
	float w[256], y[256], yy[256], h;
	float v_between;
	int histSize[1];
	float hranges[2]; 
	const float* ranges[1];
	int channels[1];

public:
	Histogram1D() { 
		histSize[0] = 256;
		hranges[0] = 0.0;
		hranges[1] = 255.0;
		ranges[0] = hranges;
		channels[0] = 0;
	}
	MatND getHistogram(const Mat &image) {
		MatND hist;	
		calcHist(&image, 1, channels, Mat(), hist, 1, histSize, ranges);
		return hist;
	}

	float getOtsu(const MatND his, const Mat &img) {
		//base case
		w[0] = his.at<float>(0) / (256 * 256);
		y[0] = 0;

		float max;
		float tsh = 0;
		float u = 0;

		for (int i = 0; i < 256; i++)
			u += i * his.at<float>(i) / (256 * 256);

		for (int t = 1; t < 256; t++)
		{
			h = his.at<float>(t)/ (256 * 256);
			
			w[t] = w[t - 1] + h;
			y[t] = (w[t - 1] * y[t - 1] + t * h) / w[t];
			yy[t] = (u - w[t] * y[t]) / (1 - w[t]);

			v_between = w[t] * (1 - w[t]) * pow((y[t] - yy[t]) , 2);
			
			if (t == 1)
			{
				max = v_between;
				tsh = t;
			}
			else if (v_between > max)
			{
				max = v_between;
				tsh = t;
			}
		}
		return tsh;
	}
};