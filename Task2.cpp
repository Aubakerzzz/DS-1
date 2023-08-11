#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"

using namespace cv;

#include<iostream>
using namespace std;



class node
{
public:
	int* index = new int[2];
	node* next;
	node()
	{
		index[0] = index[1] = 0;
		next = NULL;
	}
};

class Stack
{
public:
	node* top;
	int count;
	Stack()
	{
		top = NULL;
		count = 0;
	}
	void push(int i, int j)
	{
		node* temp = new node;
		temp->index[0] = i;
		temp->index[1] = j;
		temp->next = NULL;
		if (top == NULL)
		{
			top = temp;
			count++;
		}
		else
		{
			temp->next = top;
			top = temp;
			count++;
		}
	}
	int* pop()
	{
		if (top != NULL)
		{
			node* temp = top;
			top = top->next;
			int* n = new int[2];
			n[0] = temp->index[0];
			n[1] = temp->index[1];
			delete temp;
			count--;
			return n;
		}
		return NULL;
	}
	int* Top()
	{
		if (top != NULL)
			return top->index;
		return NULL;
	}
	bool isEmpty()
	{
		if (top == NULL)
			return 1;
		return 0;
	}
	void clear()
	{
		while (!isEmpty())
			pop();
	}
};

class Queue
{
public:
	/*node* front;
	node* rear;*/
	int data[1000][2]{};
	int size;
	int front, rear;
	Queue()
	{
		//front = rear = NULL;
		size = 0;
		front = rear = 0;
	}
	void enqueue(int i, int j)
	{
		/*node* temp = new node;
		temp->index[0] = i;
		temp->index[1] = j;
		temp->next = NULL;
		if (front == NULL)
		{
			front=rear = temp;
			size++;
		}
		else
		{
			rear->next = temp;
			rear = temp;
			size++;
		}*/
		rear = rear % 1000;
		data[rear][0] = i;
		data[rear][1] = j;
		rear++;
		size++;
	}
	int* dequeue()
	{
		/*if (front != NULL)
		{
			node* temp = front;
			front = front->next;
			int* n = new int[2];
			n[0] = temp->index[0];
			n[1] = temp->index[1];
			delete temp;
			size--;
			return n;
		}
		return NULL;*/

		int* n = new int[2];
		front = front % 1000;
		n[0] = data[front][0];
		n[1] = data[front][1];

		front++;
		size--;
		return n;

	}
	int* Front()
	{
		int* n = new int[2];
		n[0] = data[front][0];
		n[1] = data[front][1];
		return n;
		/*if (front != NULL)
			return front->index;
		return NULL;*/
	}
	bool isEmpty()
	{
		/*if (front == NULL)
			return 1;
		return 0;*/
		if (size == 0)
			return 1;
		return 0;
	}
	/*bool isExist(int i,int j)
	{
		node* curr=front;
		for (int l = 0; l < size && curr != NULL; l++)
		{
			if (curr->index[0] == i && curr->index[1] == j)
				return 1;
			curr = curr->next;
		}
		return 0;
	}*/
};

void ExtractGlxyRow(int** arr, int r, int c)
{
	Mat dupimg, dupcon;
	string str = "Input.png";
	string str1 = "1.png";
	dupimg = imread(str, IMREAD_GRAYSCALE);
	dupimg.convertTo(dupcon, CV_32S, 1, 0);
	int** dup = new int* [r];
	int i = 0, k = 0;
	for (i = 0; i < r; ++i)
		dup[i] = dupcon.ptr<int>(i);
	for (i = 0; i < r; ++i)
		for (k = 0; k < c; k++)
			dup[i][k] = 0;
	Queue q;
	int* index = NULL;
	//int neighbour[4]{0};

	// 255 enqueue  ; loop till end to queue then dequeue then check neighbour oof 8 if black then leave else enqueue it and then do it black 
	bool flag = 1;
	int t = 49;
	int x = 0, y = 0;
	for (i = 0; i < r; ++i)
	{
		for (k = 0; k < c; k++)
		{
			if (arr[i][k] == 255)
			{

				q.enqueue(i, k);
				dup[i][k] = 255;
				arr[i][k] = 0;
				do
				{
					index = NULL;
					index = q.dequeue();
					if (arr[index[0]][index[1] + 1] == 255)
					{
						if (dup[index[0]][index[1] + 1] != 255)
						{
							//cout << index[0] << "   " << index[1] + 1 << endl;
							q.enqueue(index[0], index[1] + 1);
							dup[index[0]][index[1] + 1] = 255;
							arr[index[0]][index[1] + 1] = 0;
						}
					}
					if (arr[index[0] + 1][index[1] + 1] == 255)
					{
						if (dup[index[0] + 1][index[1] + 1] != 255)
						{
							//cout << index[0] + 1 << "   " << index[1] + 1 << endl;
							q.enqueue(index[0] + 1, index[1] + 1);
							dup[index[0] + 1][index[1] + 1] = 255;
							arr[index[0] + 1][index[1] + 1] = 0;
						}
					}
					if (arr[index[0] + 1][index[1]] == 255)
					{
						if (dup[index[0] + 1][index[1]] != 255)
						{
							//cout << index[0] + 1 << "   " << index[1] << endl;
							q.enqueue(index[0] + 1, index[1]);
							dup[index[0] + 1][index[1]] = 255;
							arr[index[0] + 1][index[1]] = 0;
						}
					}
					if (arr[index[0] + 1][index[1] - 1] == 255)
					{
						if (dup[index[0] + 1][index[1] - 1] != 255)
						{
							//cout << index[0] + 1 << "   " << index[1] - 1 << endl;
							q.enqueue(index[0] + 1, index[1] - 1);
							dup[index[0] + 1][index[1] - 1] = 255;
							arr[index[0] + 1][index[1] - 1] = 0;
						}
					}

					if (arr[index[0]][index[1] - 1] == 255)
					{
						if (dup[index[0]][index[1] - 1] != 255)
						{
							//cout << index[0] << "   " << index[1] - 1 << endl;
							q.enqueue(index[0], index[1] - 1);
							dup[index[0]][index[1] - 1] = 255;
							arr[index[0]][index[1] - 1] = 0;
						}
					}
					if (arr[index[0] - 1][index[1]] == 255)
					{
						if (dup[index[0] - 1][index[1]] != 255)
						{
							//cout << index[0] - 1 << "   " << index[1] << endl;
							q.enqueue(index[0] + 1, index[1]);
							dup[index[0] - 1][index[1]] = 255;
							arr[index[0] - 1][index[1]] = 0;
						}
					}
					if (arr[index[0] - 1][index[1] - 1] == 255)
					{
						if (dup[index[0] - 1][index[1] - 1] != 255)
						{
							//cout << index[0] - 1 << "   " << index[1] - 1 << endl;
							q.enqueue(index[0] - 1, index[1] - 1);
							dup[index[0] - 1][index[1] - 1] = 255;
							arr[index[0] - 1][index[1] - 1] = 0;
						}
					}
					if (arr[index[0] - 1][index[1] + 1] == 255)
					{
						if (dup[index[0] - 1][index[1] + 1] != 255)
						{
							//cout << index[0] - 1 << "   " << index[1] + 1 << endl;
							q.enqueue(index[0] - 1, index[1] + 1);
							dup[index[0] - 1][index[1] + 1] = 255;
							arr[index[0] - 1][index[1] + 1] = 0;
						}
					}

				} while (!q.isEmpty());
				cout << "Image Exported" << endl;
				str1[0] = t;
				cout << str1 << endl;
				imwrite(str1, dupcon);
				t++;

				for (x = 0; x < r; x++)
					for (y = 0; y < c; y++)
						dup[x][y] = 0;

			}
		}

	}


}

int main()
{
	Mat img, con, img2[10];
	string str = "Input.png";
	img = imread(str, IMREAD_GRAYSCALE);
	img.convertTo(con, CV_32S, 1, 0);
	int m = con.rows, n = con.cols;
	cout << m << "\t" << n << endl;
	int** arr = new int* [m];
	for (int i = 0; i < m; ++i)
	{
		arr[i] = con.ptr<int>(i);
	}
	ExtractGlxyRow(arr, m, n);
	//imwrite(str, con);
}
