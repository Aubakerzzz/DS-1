void kmean(int** arr, int m, int n)
{
	int i = 0;
	int** arr2 = new int* [m];
	
	for (; i < m; i++)
		arr2[i] = new int[n] {};
	int* cen1 = new int[m * n]{};
	int* cen2 = new int[m * n]{};
	int c1 = 40;
	int c2 = 170;
	int  d1 = 0;
	int  d2 = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	int m = 0;
	int avg1 = 41;
	int avg2 = 171;
	int sum;

	while (avg1 != c1 && avg2 != c2)
	{
		c1 = avg1;
		c2 = avg2;
		sum = 0;
		l = m = 1;
		for (j = 0; j < m; j++)
			for (k = 0; k < n; k++)
			{
				d1 = c1 - arr[j][k];
				d2 = c2 - arr[j][k];

				if (d1 < 0)
					d1 *= (-1);
				if (d2 < 0)
					d2 *= (-1);

				if (d1 < d2)
				{
					arr2[j][k] = c1;
					cen1[l-1] = arr[j][k];
					l++;
				}
				else if (d2 <= d1)
				{
					arr2[j][k] = c2;
					cen2[m-1] = arr[j][k];
					m++;
				}
			}
		
		for (int j = 0; j < l; j++)
			sum += cen1[j];
		avg1 = sum / l;

		sum = 0;
		for (int j = 0; j < m; j++)
			sum += cen2[j];
		avg2 = sum / m;
	} 

	for (j = 0; j < m; j++)
		for (k = 0; k < n; k++)
		{
			if (arr2[j][k] == c2)
				arr[j][k] = 0;
			if (arr2[j][k] == c1)
				arr[j][k] = 255;
			
		}
}
