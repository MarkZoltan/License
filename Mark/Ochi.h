#pragma once
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include "opencv2/imgcodecs.hpp"

#include<iostream>
#include<string>

std::string ssystem(const char *command);

void ShowManyImages(std::string title,std::vector<cv::Mat>& imgs);

void saveImage(std::string numeFisier, cv::Mat & imagine);

std::string comdPyth(std::string numeFisier, int acceptat);

std::string getTime();