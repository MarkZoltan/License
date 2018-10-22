//
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/objdetect/objdetect.hpp>
//#include "opencv2/imgcodecs.hpp"
//
//#include<iostream>
//using namespace cv;
//
//cv::CascadeClassifier face_cascade;
//cv::CascadeClassifier eye_cascade;
//
//int sobel(int, char** argv)
//{
//	Mat src, src_gray;
//	Mat grad;
//	const char* window_name = "Sobel Demo - Simple Edge Detector";
//	int scale = 1;
//	int delta = 0;
//	int ddepth = CV_16S;
//	src = imread(argv[1], IMREAD_COLOR); // Incarca imaginea
//	if (src.empty())
//	{
//		return -1;
//	}
//	GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
//	cvtColor(src, src_gray, COLOR_BGR2GRAY);
//	Mat grad_x, grad_y;
//	Mat abs_grad_x, abs_grad_y;
//	Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
//	Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
//	convertScaleAbs(grad_x, abs_grad_x);
//	convertScaleAbs(grad_y, abs_grad_y);
//	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
//	imshow(window_name, grad);
//	waitKey(0);
//	return -1;
//}
//
////Detect Ochiul
//std::vector<cv::Rect> detectEye(cv::Mat& im, cv::Mat& tpl, cv::Rect& rect)
//{
//	std::vector<cv::Rect> faces, eyes,eys;
//	face_cascade.detectMultiScale(im, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
//
//	for (int i = 0; i < faces.size(); i++)
//	{
//		cv::Mat face = im(faces[i]);
//		eye_cascade.detectMultiScale(face, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(20, 20));
//
//		if (eyes.size())
//		{
//			rect = eyes[0] + cv::Point(faces[i].x, faces[i].y);
//			tpl = im(rect);
//			std::cout << "h";
//			eys.push_back(rect);
//		}
//	}
//
//	return eys;
//}
//
//
//void trackEye(cv::Mat& im, cv::Mat& tpl, cv::Rect& rect)
//{
//	cv::Size size(rect.width * 2, rect.height * 2);
//	cv::Rect window(rect + size - cv::Point(size.width / 2, size.height / 2));
//
//	window &= cv::Rect(0, 0, im.cols, im.rows);
//
//	cv::Mat dst(window.width - tpl.rows + 1, window.height - tpl.cols + 1, CV_32FC1);
//	cv::matchTemplate(im(window), tpl, dst, CV_TM_SQDIFF_NORMED);
//
//	double minval, maxval;
//	cv::Point minloc, maxloc;
//	cv::minMaxLoc(dst, &minval, &maxval, &minloc, &maxloc);
//
//	if (minval <= 0.2)
//	{
//		rect.x = window.x + minloc.x;
//		rect.y = window.y + minloc.y;
//	}
//	else
//		rect.x = rect.y = rect.width = rect.height = 0;
//}
//
//// comparator de imagine 
//double getSimilarity(const Mat A, const Mat B)
//{
//	if (A.rows > 0 && A.rows == B.rows && A.cols > 0 && A.cols == B.cols) {
//
//		double errorL2 = norm(A, B, CV_L2);
//		double similarity = errorL2 / (double)(A.rows * A.cols);
//		return similarity;
//	}
//	else {
//		return 100000000.0;
//	}
//}
//
//int main1(int argc, char** argv)
//{
//
//	face_cascade.load("haarcascade_frontalface_alt2.xml");
//	eye_cascade.load("haarcascade_eye.xml");
//
//	// Deschide webcam
//	cv::VideoCapture cap(0);
//
//	// Verif daca totul e ok
//	if (face_cascade.empty() || eye_cascade.empty() || !cap.isOpened())
//		return 1;
//
//	//Seteaza calitatea la 320x240
//	cap.set(CV_CAP_PROP_FRAME_WIDTH, 3 * 720);
//	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 2 * 576);
//
//	cv::Mat frame, eye_tpl, color_tpl;
//	cv::Rect eye_bb;
//
//	while (cv::waitKey(30) != 'q')
//	{
//		cap >> frame;
//		if (frame.empty())
//			break;
//
//		// Arunca framul orizontal
//		//cv::flip(frame, frame, 1);
//
//
//		// Ajusteaza imaginea folosind un egalizator
//		cv::Mat gray;
//		cv::cvtColor(frame, gray, CV_BGR2GRAY);
//		if (cv::waitKey(30) == 'r') {
//			eye_bb.width = 0;
//			eye_bb.height = 0;
//		}
//		if (eye_bb.width == 0 && eye_bb.height == 0)
//		{
//
//			//Incearca sa gaseasca ochiul
//			detectEye(gray, eye_tpl, eye_bb);
//			frame(eye_bb).copyTo(color_tpl);
//			if (color_tpl.rows == 0 || color_tpl.cols == 0)continue;
//			circle(color_tpl, Point(eye_tpl.rows / 2, eye_tpl.cols / 2), ((eye_tpl.rows / 2 < eye_tpl.cols / 2) ? eye_tpl.rows / 2 : eye_tpl.cols / 2) - 25, Scalar(255, 255, 255));
//			circle(color_tpl, Point(eye_tpl.rows / 2, eye_tpl.cols / 2), ((eye_tpl.rows < eye_tpl.cols) ? eye_tpl.rows / 4 : eye_tpl.cols / 4), Scalar(255, 255, 255));
//			std::to_string(10);
//			cv::imshow("eye", color_tpl);
//		}
//		else
//		{
//
//			trackEye(gray, eye_tpl, eye_bb);
//
//			// Deseneaza casuta pt ochi
//
//			cv::rectangle(frame, eye_bb, CV_RGB(0, 300, 0));
//		}
//
//		//Arata video
//		cv::imshow("video", frame);
//	}
//
//	return 0;
//}