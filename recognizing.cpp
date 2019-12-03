#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int y_searching(Mat bina, int fi_x, int st_x, int st_y, int ax_y, int i){
	int count = 0;
	int result = 1;
	for (int j = -2; j < 5; j++) {
		for (int x = fi_x; x < st_x; x++) {
			uchar b = bina.at<Vec3b>((st_y + i * ax_y) + j, x)[0];
			uchar g = bina.at<Vec3b>((st_y + i * ax_y) + j, x)[1];
			uchar r = bina.at<Vec3b>((st_y + i * ax_y) + j, x)[2];

			uchar pixel = (b + g + r) / 3;

			if (pixel > 70 && pixel < 220)
				count += 1;
			if (count > 50) {
				result = 0;
				break;
			}
		}
	}
	return result;
}


int* recognizing(Mat frame) {

	Mat currentlot = frame;
	if (currentlot.empty()) {
		cout << "Could not open or find the image" << endl;
	}

	int height = currentlot.rows;
	int width = currentlot.cols;

	Mat bina = currentlot;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			uchar b = currentlot.at<Vec3b>(y, x)[0];
			uchar g = currentlot.at<Vec3b>(y, x)[1];
			uchar r = currentlot.at<Vec3b>(y, x)[2];

			uchar gray = (b + g + r) / 3;

			if (gray > 220) {
				bina.at<Vec3b>(y, x)[0] = 255;
				bina.at<Vec3b>(y, x)[1] = 255;
				bina.at<Vec3b>(y, x)[2] = 255;
			}
			else if (gray < 70) {
				bina.at<Vec3b>(y, x)[0] = 0;
				bina.at<Vec3b>(y, x)[1] = 0;
				bina.at<Vec3b>(y, x)[2] = 0;
			}
		}
	}

	int sta_x = 0;
	int fia_x = 0;
	int inlot = 0;
	//find width in a section  
	for (int x = width / 2; x > 0; x--) {
		uchar b = bina.at<Vec3b>(height / 2, x)[0];
		uchar g = bina.at<Vec3b>(height / 2, x)[1];
		uchar r = bina.at<Vec3b>(height / 2, x)[2];

		uchar gray = (b + g + r) / 3;

		if (gray == 255 && inlot == 2) {
			fia_x = x;
			break;
		}
		else if (gray == 0 && inlot == 1) {
			inlot += 1;
			sta_x = x;
		}
		else if (gray == 255 && inlot == 0) {
			inlot += 1;
		}
	}

	int stb_x = 0;
	int fib_x = 0;
	inlot = 0;
	//find width in a bsection  
	for (int x = width / 2; x < width; x++) {
		uchar b = bina.at<Vec3b>(height / 2, x)[0];
		uchar g = bina.at<Vec3b>(height / 2, x)[1];
		uchar r = bina.at<Vec3b>(height / 2, x)[2];

		uchar gray = (b + g + r) / 3;

		if (gray == 255 && inlot == 2) {
			fib_x = x;
			break;
		}
		else if (gray == 0 && inlot == 1) {
			inlot += 1;
			stb_x = x;
		}
		else if (gray == 255 && inlot == 0) {
			inlot += 1;
		}
	}


	//find real height
	int st_y = 0;
	int fi_y = 0;
	for (int y = height / 2; y > 0; y--) {
		uchar b = bina.at<Vec3b>(y, width / 2)[0];
		uchar g = bina.at<Vec3b>(y, width / 2)[1];
		uchar r = bina.at<Vec3b>(y, width / 2)[2];

		uchar gray = (b + g + r) / 3;

		if (!(gray < 30)) {
			st_y = y;
			break;
		}
	}
	fi_y = height - st_y;

	//searching asection 
	int asection[5];
	int ax_y = (fi_y - st_y) / 6;
	int count = 1;
	asection[count - 1] = y_searching(bina, fia_x, sta_x, st_y, ax_y, count);
	count = 2;
	asection[count - 1] = y_searching(bina, fia_x, sta_x, st_y, ax_y, count);
	count = 3;
	asection[count - 1] = y_searching(bina, fia_x, sta_x, st_y, ax_y, count);
	count = 4;
	asection[count - 1] = y_searching(bina, fia_x, sta_x, st_y, ax_y, count);
	count = 5;
	asection[count - 1] = y_searching(bina, fia_x, sta_x, st_y, ax_y, count);


	int cotemp = fib_x;
	fib_x = stb_x;
	stb_x = cotemp;

	//searching bsection 
	int bsection[5];
	count = 1;
	bsection[count - 1] = y_searching(bina, fib_x, stb_x, st_y, ax_y, count);
	count = 2;
	bsection[count - 1] = y_searching(bina, fib_x, stb_x, st_y, ax_y, count);
	count = 3;
	bsection[count - 1] = y_searching(bina, fib_x, stb_x, st_y, ax_y, count);
	count = 4;
	bsection[count - 1] = y_searching(bina, fib_x, stb_x, st_y, ax_y, count);
	count = 5;
	bsection[count - 1] = y_searching(bina, fib_x, stb_x, st_y, ax_y, count);


	static int section[10];

	for (int i = 0; i < 5; i++) {
		section[i] = asection[i];
	}
	for (int i = 0; i < 5; i++) {
		section[5 + i] = bsection[i];
	}
	
	return section;
}
