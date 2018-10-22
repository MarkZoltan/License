#include "Ochi.h"

cv::CascadeClassifier face_cascade;
cv::CascadeClassifier eye_cascade;

std::vector<cv::Mat >selectieImagini;

std::string errorMessage = "";
cv::Mat machEye;

std::vector<cv::Rect> detectEye(cv::Mat& im)
{
	std::vector<cv::Rect> faces, eyes, eys;
	face_cascade.detectMultiScale(im, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30)); // in poza curenta gaseste fetele

	for (int i = 0; i < faces.size(); i++)
	{
		cv::Mat face = im(faces[i]);
		eye_cascade.detectMultiScale(face, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(20, 20)); // iar acuma gaseste ochi

		for (int j = 0; j < eyes.size(); j++)
		{
			cv::Rect rect = eyes[j] + cv::Point(faces[i].x, faces[i].y);
			eys.push_back(rect);
		}
	}
	return eys;
}
void trackEye(cv::Mat& im, cv::Mat& tpl, cv::Rect& rect)
{
	cv::Size size(rect.width * 2, rect.height * 2);
	cv::Rect window(rect + size - cv::Point(size.width / 2, size.height / 2));

	window &= cv::Rect(0, 0, im.cols, im.rows);

	cv::Mat dst(window.width - tpl.rows + 1, window.height - tpl.cols + 1, CV_32FC1);
	cv::matchTemplate(im(window), tpl, dst, CV_TM_SQDIFF_NORMED);

	double minval, maxval;
	cv::Point minloc, maxloc;
	cv::minMaxLoc(dst, &minval, &maxval, &minloc, &maxloc);

	if (minval <= 0.2)
	{
		rect.x = window.x + minloc.x;
		rect.y = window.y + minloc.y;
	}
	else
		rect.x = rect.y = rect.width = rect.height = 0;
}
void afisRect(cv::Mat&im, std::string windowName, std::vector<cv::Rect>rects) {

	std::vector<cv::Mat> imgs;
	if (!selectieImagini.empty())selectieImagini.clear();
	for (int i = 0; i < rects.size(); i++) {
		cv::Mat imagine;
		im(rects[i]).copyTo(imagine);
		cv::putText(imagine, std::to_string(i), cv::Point(5, 15), cv::FONT_ITALIC, 0.5, cv::Scalar(0, 0, 255), 1, CV_AA);
		int r = ((imagine.rows / 2 < imagine.cols / 2) ? imagine.rows / 2 : imagine.cols / 2) - 25;
		if(r>0)
		cv::circle(imagine, cv::Point(imagine.rows / 2, imagine.cols / 2), r, cv::Scalar(255, 255, 255));
		imgs.push_back(imagine);
		cv::Mat imagine1;
		im(rects[i]).copyTo(imagine1);
		selectieImagini.push_back(imagine1);
	}
	//cvDestroyWindow(windowName.c_str());
	ShowManyImages(windowName, imgs);
}

void afisareFrame(cv::Mat & frame)
{
	cv::putText(frame, " Press R to capture ", cv::Point(0, 30), cv::FONT_ITALIC, 1, cv::Scalar(0, 0, 255), 1, CV_AA);
	cv::putText(frame, " Press Q to exit ", cv::Point(0, 60), cv::FONT_ITALIC, 1, cv::Scalar(0, 0, 255), 1, CV_AA);
	cv::putText(frame, " Press T to train ", cv::Point(0, 90), cv::FONT_ITALIC, 1, cv::Scalar(0, 0, 255), 1, CV_AA);
	cv::putText(frame, " Press Y to reset ", cv::Point(0, 120), cv::FONT_ITALIC, 1, cv::Scalar(0, 0, 255), 1, CV_AA);
	cv::putText(frame, " Press E to train multiple ", cv::Point(0, 150), cv::FONT_ITALIC, 1, cv::Scalar(0, 0, 255), 1, CV_AA);
	cv::putText(frame, errorMessage, cv::Point(10, 180), cv::FONT_ITALIC, 1, cv::Scalar(0, 0, 255), 1, CV_AA);

	if (!machEye.empty()) {
		//std::cout << frame.cols<<" \n";
		machEye.copyTo(frame(cv::Rect(1000, 0, machEye.cols, machEye.rows)));
	}

	cv::imshow("video", frame);
}
void capturare(cv::Mat & frame)
{
	std::vector<cv::Rect> eyes_rect;
	cv::Mat gray;

	cv::cvtColor(frame, gray, CV_BGR2GRAY);

	//Incearca sa gaseasca ochiul
	eyes_rect = detectEye(gray);

	if (eyes_rect.size() > 10)
	{
		std::cout << "ignora ochii cu nr mai mare de 10";
		errorMessage = "ignora ochii cu nr mai mare de 10";
	}
	if (eyes_rect.empty())
	{
		std::cout << "NU A FOST GASIT\n";
		errorMessage = "NU A FOST GASIT";
	}
	else {
		afisRect(frame, "eyes", eyes_rect);
		errorMessage = "";
	}

}

bool isPositive(std::string result)
{
	return ((std::stod(result) >= 0.5));
}
int sel(char c)
{
	if ((c < '0') || (c > '9'))
		return -1;
	return c - '0';

}
void analizImg(cv::Mat & frame) {
	saveImage("ochi.jpg", frame);
	std::string m=comdPyth("./ochi.jpg", -1);
	cv::Scalar sc (0, 0, 255);
	std::string m2 = "Ochi necunoscut";
	if (isPositive(m)) {
		sc = cv::Scalar(0, 255, 0);
		m2 = "Ochi recunoscut";
	}


	cv::putText(frame, m2.c_str(), cv::Point(5, 20), cv::FONT_ITALIC, 0.25, sc, 1, CV_AA);
	cv::putText(frame, m.c_str(), cv::Point(5, 30), cv::FONT_ITALIC, 0.25, sc, 1, CV_AA);
	machEye = frame;

	cvDestroyWindow("eyes");
	//cv::imshow("eyes", frame);

}
void train(cv::Mat & frame,int e) {
	//int e;
	std::cout << "1 sau 0:"; 
	//std::cin >> e;
	e = (e == 0 ? 0 : 1);
	saveImage("ochi.jpg", frame);

	std::string filename1 = "./savedImgs/" + std::to_string(e) + "/" + getTime() + ".jpg";
	cv::imwrite(filename1, frame);

	std::string m = comdPyth("./ochi.jpg", e);
	//cv::putText(frame, m.c_str(), cv::Point(0, 30), cv::FONT_ITALIC, 0.25, cv::Scalar(0, 0, 255), 1, CV_AA);
	
	analizImg(frame);
	// cv::imshow("eyes", frame);

	
}

void trainMultiple() {
	ssystem("py trainModule.py");
}
bool mainLoop(cv::Mat & frame)
{
	static bool isTraining = false;
	static bool one = false;
	static int selectie = -1;
	if (frame.empty())
		return false;
	char c = cv::waitKey(30);
	switch (c)
	{
	case 'Q':
	case 'q':
		return false;
	case 'E':
	case 'e':
		trainMultiple();
		break;
	case 'Y':
	case 'y':
		std::cout << "Are you sure?(y/N)";
		char r;
		std::cin >> r;
		if (r == 'y') {
			comdPyth("", -1); //ruleaz pythonu
		}
		break;
	case 'T':
	case 't':
		isTraining = true;
	case 'R':
	case 'r':
		capturare(frame);// realiz captura
	default:
		afisareFrame(frame);//afiseaa frame-l curent
		break;
	}

	int s = sel(c);
	if (!(s < 0))
	{
		
		if (one)
		{
			one = false;
			train(selectieImagini[selectie],s);// antreneaza reteaua neuronala
			selectie = -1;
			errorMessage = "";
		}
		else if (isTraining) {
			selectie = s;
			one = true; isTraining = false;
			cvDestroyWindow("eyes");
			errorMessage = "1 sau 0:";
		}
		else
			analizImg(selectieImagini[s]);// analizeaza reteaua neuronala
	}
	return true;
}

int main(int argc, char** argv)
{
	face_cascade.load("haarcascade_frontalface_alt2.xml");
	eye_cascade.load("haarcascade_eye.xml");

	// Deschide webcam
	cv::VideoCapture cap(0);

	// Verif daca totul e ok
	if (face_cascade.empty() || eye_cascade.empty() || !cap.isOpened())
		return 1;

	//Seteaza calitatea la 320x240
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 3 * 720);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 2 * 576);

	cv::Mat frame, eye_tpl, color_tpl;
	std::vector<cv::Rect> eyes_rect;
	cv::Rect eye_bb;
	bool isNewCapture = false;
	cap >> frame;

	while (mainLoop(frame))
	{
		cap >> frame;
	}
	return 0;
}