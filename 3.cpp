void dcoff(int** arr1, int** arr2, int m, int n)
{

	int i = 0;
	int j = 0;
	int tp = 0;
	int fn = 0;
	int fp = 0;
	for (; i < m; i++)
		for (j = 0; j < n; j++)
		{
			if (arr1[i][j] == 255 && arr2[i][j] == 255)
				tp++;
			if (arr1[i][j] == 255 && arr2[i][j] == 0)
				fp++;
		}
	fn = (tp+fp)/2;
	double dc = 0;
	dc = ((double)(2 * tp) / (double)(fn + fp + (2 * tp))) * 100;
	cout << "Dice coefficient : " << dc << endl;
}
