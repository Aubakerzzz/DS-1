#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <iostream>

using namespace cv;
using namespace std;

//White == 255
//Black == 0

int main() {

	// storing the different color pixels
	Vec3b* colors = new Vec3b[10];
	colors[0] = Vec3b(10, 17, 255);
	colors[1] = Vec3b(192, 25, 191);
	colors[2] = Vec3b(255, 0, 0);
	colors[3] = Vec3b(0, 0, 255);
	colors[4] = Vec3b(0, 255, 0);
	colors[5] = Vec3b(0, 127, 255);
	colors[6] = Vec3b(0, 255, 255);
	colors[7] = Vec3b(130, 0, 75);
	colors[8] = Vec3b(123, 104, 238);
	colors[9] = Vec3b(173, 255, 47);

	// vectors to store the marked pixels
	Vec3b background = Vec3b(0, 0, 0);	// black background
	Vec3b foreground = Vec3b(255, 255, 255);	//white foreground

	const int NUM_IMAGES = 45;
	// reading the images
	string path = "Segmented Outputs/";
	Mat* images = new Mat[NUM_IMAGES];
	for (int i = 0; i < NUM_IMAGES; i++)
		images[i] = imread(path + to_string(i + 1) + ".bmp", IMREAD_COLOR);
	for (int k = 0; k < 45; k++) {
	
	// list to store the image
	int** labels_list = new int*[images[k].rows];
	for (int i = 0; i < images[k].rows; i++) {
		labels_list[i] = new int[images[k].cols];
	}

		for (int i = 0; i < images[k].rows; i++) {
			for (int j = 0; j < images[k].cols; j++) {
				if (images[k].at<Vec3b>(i, j) == background)
					labels_list[i][j] = 0;
				else
					labels_list[i][j] = -1;
			}
		}

		imshow("Hello", images[k]);
		waitKey(0);


		int equivalence_list[500][2] = {};
		int equivalence_length = 50;
		int equivalence_occupied = 0;
		//checking for the components 8-connectivity and finding the connected region
		int label = 0;
		for (int i = 1; i < images[k].rows - 1; i++) {
			for (int j = 1; j < images[k].cols - 1; j++) {
				Vec3b& current_Pixel = images[k].at<Vec3b>(i, j);
				// if the pixel is a background pixel
				if (current_Pixel == background) continue;
				// if the pixele is a foreground pixel
				if (current_Pixel == foreground) {
					// if all four neighbours are zero
					if (images[1].at<Vec3b>(i, j - 1) == background && images[k].at<Vec3b>(i - 1, j) == background && images[k].at<Vec3b>(i - 1, j - 1) == background && images[k].at<Vec3b>(i - 1, j + 1) == background) {
						labels_list[i][j] = ++label;
						equivalence_list[equivalence_occupied][0] = label;
						equivalence_list[equivalence_occupied][1] = label;
						equivalence_occupied++;
					}
					//if only one neighbour is a foreground pixel
					else if (images[1].at<Vec3b>(i, j - 1) == foreground && images[k].at<Vec3b>(i - 1, j) == background && images[k].at<Vec3b>(i - 1, j - 1) == background && images[k].at<Vec3b>(i - 1, j + 1) == background) {
						labels_list[i][j] = labels_list[i][j - 1];
					}
					else if (images[1].at<Vec3b>(i, j - 1) == background && images[k].at<Vec3b>(i - 1, j) == foreground && images[k].at<Vec3b>(i - 1, j - 1) == background && images[k].at<Vec3b>(i - 1, j + 1) == background) {
						labels_list[i][j] = labels_list[i - 1][j];
					}
					else if (images[1].at<Vec3b>(i, j - 1) == background && images[k].at<Vec3b>(i - 1, j) == background && images[k].at<Vec3b>(i - 1, j - 1) == foreground && images[k].at<Vec3b>(i - 1, j + 1) == background) {
						labels_list[i][j] = labels_list[i - 1][j - 1];
					}
					else if (images[1].at<Vec3b>(i, j - 1) == background && images[k].at<Vec3b>(i - 1, j) == background && images[k].at<Vec3b>(i - 1, j - 1) == background && images[k].at<Vec3b>(i - 1, j + 1) == foreground) {
						labels_list[i][j] = labels_list[i - 1][j + 1];
					}
					else {
						Vec3b leftp = images[k].at<Vec3b>(i, j - 1);//left
						Vec3b topp = images[k].at<Vec3b>(i - 1, j);//top
						Vec3b left_upperp = images[k].at<Vec3b>(i - 1, j - 1);//left upper diagonal
						Vec3b right_upperp = images[k].at<Vec3b>(i - 1, j + 1);//right upper diagonal

						// if all the neighbours are foreground
						if (leftp == foreground && topp == foreground && left_upperp == foreground && right_upperp == foreground) {
							// get the minimum among all
							int min_label = min(labels_list[i][j - 1], min(labels_list[i - 1][j], min(labels_list[i - 1][j - 1], labels_list[i - 1][j + 1])));
							// assign the minimum to the list
							labels_list[i][j] = min_label;
							for (int m = 0; m < equivalence_occupied; m++) {
								if (equivalence_list[m][0] == labels_list[i][j - 1])
									equivalence_list[m][1] = min_label;
								else if (equivalence_list[m][0] == labels_list[i - 1][j])
									equivalence_list[m][1] = min_label;
								else if (equivalence_list[m][0] == labels_list[i - 1][j - 1])
									equivalence_list[m][1] = min_label;
								else if (equivalence_list[m][0] == labels_list[i - 1][j + 1])
									equivalence_list[m][1] = min_label;
							}
						}
						else if (leftp == foreground && topp == foreground && left_upperp == foreground && right_upperp != foreground) {
							// get the minimum among all
							int min_label = min(labels_list[i][j - 1], min(labels_list[i - 1][j], labels_list[i - 1][j - 1]));
							// assign the minimum to the list
							labels_list[i][j] = min_label;
							for (int m = 0; m < equivalence_occupied; m++) {
								if (equivalence_list[m][0] == labels_list[i][j - 1])
									equivalence_list[m][1] = min_label;
								else if (equivalence_list[m][0] == labels_list[i - 1][j])
									equivalence_list[m][1] = min_label;
								else if (equivalence_list[m][0] == labels_list[i - 1][j - 1])
									equivalence_list[m][1] = min_label;
							}
						}
					}
				}
			}
		}

		// final connectivity checking
		for (int i = 0; i < images[k].rows; i++) {
			for (int j = 0; j < images[k].cols; j++) {
				if (images[k].at<Vec3b>(i, j) == foreground) {
					images[k].at<Vec3b>(i, j) = colors[3];
				}
				if (images[k].at<Vec3b>(i, j) == background) {
					images[k].at<Vec3b>(i, j) = colors[2];
				}


			}
		}


		imshow("Hello", images[k]);
		waitKey(0);
	}
	return 0;
}

