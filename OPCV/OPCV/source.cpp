#include <iostream>
#include <queue>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Histogram.cpp"

using namespace std;
using namespace cv;

Mat Four_Label(const Mat &image);

int main()
{

	Mat img;
	img = imread("c.jpg" , CV_LOAD_IMAGE_GRAYSCALE);

	Mat img_bw;
	Mat img_label;

	if (!img.empty())
	{
		Histogram1D h;
		MatND his = h.getHistogram(img);

		namedWindow("Image");
		namedWindow("Img_BW");
		namedWindow("Img_Label");

		threshold(img, img_bw, h.getOtsu(his, img), 255, CV_THRESH_BINARY);
		img_label = Four_Label(img_bw);

		
		imshow("Image", img);
		imshow("Img_BW", img_bw);
		imshow("Img_Label", img_label);

		waitKey(0);

		return 0;
	}
	else
		cout << "FAIL" << endl;
}

Mat Four_Label(const Mat &image) {
	//이미지 크기에 맞는 배열 생성
	int** arr = new int*[image.rows];
	for (int i = 0; i < image.rows;i++)
		arr[i] = new int[image.cols ];


	//이미지 값 복사
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			arr[i][j] = (int)image.at<uchar>(i, j);
			if (arr[i][j] == 255)
				arr[i][j] = -1;
		}
	}
	int label = 1;

	Mat m;
	cvtColor(image, m, CV_GRAY2RGB);

	for (int i = 1; i < image.rows - 2; i++) {
		for (int j = 1; j < image.cols - 2; j++) {
			if (arr[i][j] == 0) {
				queue<Point> q;
				q.push(Point(j, i));

				int r = rand() % 256;
				int g = rand() % 256;
				int b = rand() % 256;

				while (!q.empty())
				{
					Point p = q.front();
					q.pop();

					if (arr[p.y][p.x] == 0) {
						int left = p.x;
						int right = p.x;

						while (arr[p.y][left - 1] == 0)
						{
							if (left - 1 < 0)
								break;
							else
								left--;
						}
						while (arr[p.y][right + 1] == 0)
						{
							if (right + 1 > image.cols)
								break;
							else
								right++;
						}

						for (int c = left; c <= right; c++)
						{
							arr[p.y][c] = label;
							Vec3b &vec = m.at<Vec3b>(p.y, c);
							vec[0] = r;
							vec[1] = g;
							vec[2] = b;

							if (p.y - 1 >= 0)
							{
								if (arr[p.y - 1][c] == 0 && (c == left || arr[p.y - 1][c - 1] != 0))
									q.push(Point(c, p.y - 1));
							}
							if (p.y + 1 < image.rows)
							{
								if (arr[p.y + 1][c] == 0 && (c == left || arr[p.y + 1][c - 1] != 0))
									q.push(Point(c, p.y + 1));
							}
						}
					}
				}
				label++;
			}
		}
	}
	return m;
}



