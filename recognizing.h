#pragma once
#ifndef RECOGNIZING_H
#define RECOGNIZING_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include "pch.h"

using namespace std;
using namespace cv;


int y_searching(Mat bina, int fi_x, int st_x, int st_y, int ax_y, int i);
int* recognizing(Mat frame);


#endif // !RECOGNIZING_H
