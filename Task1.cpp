#include "Task1.h"

CCL::CCL() {
	array = nullptr;
	check = nullptr;
	rows = cols = 0;
	depend = 100;
	current = 0;
}

CCL::CCL(string add) {
	Mat image;
	image = imread(add, IMREAD_GRAYSCALE);
	rows = image.rows;
	cols = image.cols;
	depend = 100;
	current = 0;

	array = new int* [rows];
	for (int i = 0; i < rows; i++) {
		*(array + i) = new int[cols];

		for (int j = 0; j < cols; j++)
			*(*(array + i) + j) = int(image.at<uchar>(i, j));
	}

	check = new int* [rows];
	for (int i = 0; i < rows; i++)
		*(check + i) = new int[cols];
}


void CCL::addDependencies(int**& dependencies) { // extending array of dependency 
	int** newTemp = new int* [depend];
	for (int i = 0; i < depend; i++) {
		newTemp[i] = new int[2];
		newTemp[i][0] = dependencies[i][0];
		newTemp[i][1] = dependencies[i][1];
	}

	delete[] dependencies;
	depend *= 2;
	dependencies = new int* [depend];

	for (int i = 0; i < depend; i++) {
		dependencies[i] = new int[2];

		if (i < depend / 2) {
			dependencies[i][0] = newTemp[i][0];
			dependencies[i][1] = newTemp[i][1];
		}
	}

	delete[] newTemp;
}


void CCL::firstScan(int i, int j, int& index) {
	if (array[i][j] == 0)	// Check for if value of pixel is zero	
		check[i][j] = 0;

	else if (i == 0) {

		if (j == 0)
			check[i][j] = index++;	// Check to assign new value in case of a different pop up

		else {

			if (array[i][j] == array[i][j - 1])		// Check for the left row neighbour
				check[i][j] = check[i][j - 1];

			else						// Check to assign new value in case of a different pop up
				check[i][j] = index++;
		}
	}

	else {

		if (array[i - 1][j] == array[i][j])		// Check for value on the above pixel
			check[i][j] = check[i - 1][j];

		else if (j == 0) {

			if (array[i - 1][j + 1] == array[i][j])	// Check for the upper right diagonal pixel
				check[i][j] = check[i - 1][j + 1];

			else						// Check to assign new value in case of a different pop up
				check[i][j] = index++;
		}

		else {

			if (array[i - 1][j - 1] == array[i][j])	// Check for the upper left diagonal pixel
				check[i][j] = check[i - 1][j - 1];

			else if (array[i][j] == array[i][j - 1])		// Check for the left row neighbour
				check[i][j] = check[i][j - 1];

			else if (j == 764)
				check[i][j] = index++;	// Check to assign new value in case of a different pop up

			else {

				if (array[i - 1][j + 1] == array[i][j])	// Check for the upper right diagonal pixel
					check[i][j] = check[i - 1][j + 1];

				else					// Check to assign new value in case of a different pop up
					check[i][j] = index++;
			}
		}
	}
}

bool CCL::find(int** dependencies, int& one, int& two) {  // if equal then return 1
	for (int i = 0; i < current; i++)
		if (dependencies[i][0] == two && dependencies[i][1] == one)
			return true;
	return false;
}

void CCL::order(int& num1, int& num2) { // swaping 
	if (num2 > num1) {
		int temp = num1;
		num1 = num2;
		num2 = temp;
	}
}


void CCL::reduceDependencies(int**& dependencies) {
	int max = 0;
	for (int i = 0; i < current; i++) {

		if (dependencies[i][0] > max)
			max = dependencies[i][0];

		if (dependencies[i][1] > max)
			max = dependencies[i][1];
	}

	for (int i = 0; i < current; i++) {

		for (int j = max; j > 0; j--) {

			if (j == dependencies[i][1]) {

				for (int k = current - 1; k > -1; k--) {

					if (dependencies[k][0] == j)
						dependencies[k][0] = dependencies[i][0];
				}
			}
		}
	}


	for (int i = current - 1; i > -1; i--) {

		for (int j = current - 1; j > -1; j--) {

			for (int k = max; k > 0; k--) {

				if ((dependencies[i][1] == dependencies[j][1]) && (dependencies[i][1] == k && dependencies[j][1] == k) && (i != j) && (dependencies[i][0] != dependencies[j][0])) {

					if (dependencies[i][0] > dependencies[j][0]) {

						for (int l = 0; l < current; l++) {

							if (dependencies[l][0] == dependencies[i][0] && l != i) {

								int num1 = dependencies[l][0];
								int num2 = dependencies[i][0];
								order(num1, num2);
								int flag = find(dependencies, num1, num2);
								if (!flag) {
									dependencies[current][0] = num2;
									dependencies[current++][1] = num1;
								}

								dependencies[l][0] = dependencies[j][0];
							}
						}

						dependencies[i][0] = dependencies[j][0];
					}


					else if (dependencies[j][0] > dependencies[i][0]) {

						for (int l = 0; l < current; l++) {

							if (dependencies[l][0] == dependencies[j][0] && l != j) {

								int num1 = dependencies[l][0];
								int num2 = dependencies[j][0];
								order(num1, num2);
								int flag = find(dependencies, num1, num2);
								if (!flag) {
									dependencies[current][0] = num2;
									dependencies[current++][1] = num1;
								}

								dependencies[l][0] = dependencies[i][0];
							}
						}

						dependencies[j][0] = dependencies[i][0];
					}

					if (current == depend)
						addDependencies(dependencies);
				}
			}
		}
	}
}

void CCL::checkDependency(int**& dependencies, int i, int j) {
	int num1 = 0, num2 = 0;

	if (check[i][j] != 0 && check[i][j - 1] != 0 && check[i][j] != check[i][j - 1]) {		// Checl for left negihbour
		num1 = check[i][j];
		num2 = check[i][j - 1];
	}

	else if (check[i][j] != 0 && check[i - 1][j - 1] != 0 && check[i][j] != check[i - 1][j - 1]) {	// Check for upper left neighbour
		num1 = check[i][j];
		num2 = check[i - 1][j - 1];
	}

	else if (check[i][j] != 0 && check[i - 1][j + 1] != 0 && check[i][j] != check[i - 1][j + 1]) {	// Check for upper right neighbour
		num1 = check[i][j];
		num2 = check[i - 1][j + 1];
	}

	order(num1, num2);
	int flag = find(dependencies, num1, num2);
	if (!flag) {
		dependencies[current][0] = num2;
		dependencies[current++][1] = num1;
	}

	if (current == depend)
		addDependencies(dependencies);
}


void CCL::compute() {
	int index = 1;

	int** dependencies = new int* [depend];
	for (int i = 0; i < depend; i++)
		dependencies[i] = new int[2];

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			firstScan(i, j, index);

	for (int i = 1; i < rows; i++)
		for (int j = 1; j < cols; j++)
			checkDependency(dependencies, i, j);

	reduceDependencies(dependencies);				// rreduce equivlant scans

	for (int i = current - 1; i >= 0; i--) {		// reduced mapping	
		for (int j = 0; j < rows; j++) {
			for (int k = 0; k < cols; k++) {
				if (check[j][k] == dependencies[i][1]) {
					check[j][k] = dependencies[i][0];
				}
			}
		}
	}

	int max = -1, pixel = 1;
	for (int i = 0; i < rows; i++)			// finding max
		for (int j = 0; j < cols; j++)
			if (check[i][j] > max)
				max = check[i][j];

	int* arrayyy = new int[max] { 0 };

	for (int i = 0; i < rows; i++)			// finding max block
		for (int j = 0; j < cols; j++)
			arrayyy[check[i][j]] += 1;

	for (int i = 1; i < max; i++)
		if (arrayyy[i] > arrayyy[pixel])
			pixel = i;

	for (int i = 0; i < rows; i++)			// making one block of image
		for (int j = 0; j < cols; j++) {
			if (check[i][j] != pixel)
				check[i][j] = 0;
			else if (check[i][j] == pixel)
				check[i][j] = 255;
		}
}

int** CCL::getArray() {
	return check;
}

int CCL::getCols() {
	return cols;
}

int CCL::getRows() {
	return rows;
}


