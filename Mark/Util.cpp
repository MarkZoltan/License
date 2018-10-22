#include<fstream>
#include<cstdlib>
#include<sstream>
#include <time.h> 
#include"Ochi.h"

std::string getTime() {
	time_t rawtime;
	struct tm * timeinfo = new tm();

	time(&rawtime);
	 localtime_s(timeinfo,&rawtime);

	char buffer[80];
	strftime(buffer, 80, "%Y.%m.%d-%Hh%Mm%Ss", timeinfo);

	return std::string(buffer);
}// returneaza ora/data/minutul pt salavarea imaginilor

std::string ssystem(const char *command) {
	char tmpname[L_tmpnam];
	tmpnam_s(tmpname);
	std::string scommand = command;
	std::string cmd = scommand + " >> " + tmpname;
	std::system(cmd.c_str());
	std::ifstream file(tmpname, std::ios::in);
	std::string result;
	if (file) {
		while (!file.eof()) result.push_back(file.get());
		file.close();
	}
	remove(tmpname);
	return result;
} // executa comanda si returneaza output-ul


std::string comdPyth(std::string numeFisier, int acceptat)
{
	std::string bash = "py NN4.py " + numeFisier + " ";
	if (acceptat >= 0)
		bash += std::to_string(acceptat);
	std::string s = ssystem(bash.c_str());
	std::istringstream iss(s);
	std::string line;
	if (std::getline(iss, line))
	{
		std::cout << "LINE-> " + line + "  length: " << line.length() << std::endl;
		return line;
	}
	else return "No Mach";
}// ruleaza python-ul


void ShowManyImages(std::string title, std::vector<cv::Mat>& imgs) {
	int size;
	int i;
	int m, n;
	int x, y;
	int nArgs = imgs.size();
	// w - Maximum number of images in a row
	// h - Maximum number of images in a column
	int w, h;

	// scale - How much we have to resize the image
	float scale;
	int max;

	// If the number of arguments is lesser than 0 or greater than 12
	// return without displaying
	if (nArgs <= 0) {
		printf("Number of arguments too small....\n");
		return;
	}
	else if (nArgs > 14) {
		printf("Number of arguments too large, can only handle maximally 12 images at a time ...\n");
		return;
	}
	// Determine the size of the image,
	// and the number of rows/cols
	// from number of arguments
	else if (nArgs == 1) {
		w = h = 1;
		size = 300;
	}
	else if (nArgs == 2) {
		w = 2; h = 1;
		size = 300;
	}
	else if (nArgs == 3 || nArgs == 4) {
		w = 2; h = 2;
		size = 300;
	}
	else if (nArgs == 5 || nArgs == 6) {
		w = 3; h = 2;
		size = 200;
	}
	else if (nArgs == 7 || nArgs == 8) {
		w = 4; h = 2;
		size = 200;
	}
	else {
		w = 4; h = 3;
		size = 150;
	}
	

	// Create a new 3 channel image
	cv::Mat DispImage = cv::Mat::zeros(cv::Size(100 + size * w, 60 + size * h), CV_8UC3);

	// Used to get the arguments passed

	// Loop for nArgs number of arguments
	for (i = 0, m = 20, n = 20; i < nArgs; i++, m += (20 + size)) {
		// Get the Pointer to the IplImage
		cv::Mat img = imgs[i];

		// Check whether it is NULL or not
		// If it is NULL, release the image, and return
		if (img.empty()) {
			printf("Invalid arguments");
			return;
		}

		// Find the width and height of the image
		x = img.cols;
		y = img.rows;

		// Find whether height or width is greater in order to resize the image
		max = (x > y) ? x : y;

		// Find the scaling factor to resize the image
		scale = (float)((float)max / size);

		// Used to Align the images
		if (i % w == 0 && m != 20) {
			m = 20;
			n += 20 + size;
		}

		// Set the image ROI to display the current image
		// Resize the input image and copy the it to the Single Big Image
		cv::Rect ROI(m, n, (int)(x / scale), (int)(y / scale));
		cv::Mat temp; resize(img, temp, cv::Size(ROI.width, ROI.height));
		temp.copyTo(DispImage(ROI));
	}

	// Create a new window, and show the Single Big Image
	cv::namedWindow(title, 1);
	imshow(title, DispImage);
}
//prezinta fram-ul cu imaginile capturate pana in nr de 10

void saveImage(std::string numeFisier, cv::Mat & imagine) {
	if (imagine.rows == imagine.cols) {
		int dim = imagine.rows;
		if (dim < 100) {
			
			cv::Mat image(100, 100, imagine.type(), cv::Scalar(0, 0, 0));
			imagine.copyTo(image(cv::Rect((image.cols - imagine.cols) / 2, (image.rows - imagine.rows) / 2, imagine.cols, imagine.rows)));
			imagine = image;
		}
		else if(dim>100){
			int x = (dim - 100) / 2;
			cv::Rect r(x, x, 100, 100);
			imagine = imagine(r);
		}
	}
	else {
		std::cout << "Wrog img size\n";

	}
	cv::imwrite(numeFisier, imagine);
	std::string filename1 = "./savedImgs/" + getTime() + ".jpg";
	cv::imwrite(filename1, imagine);
}// converteste spre un size universal de 100x100