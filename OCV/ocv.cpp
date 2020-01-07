//#include "pch.h"

#include <iostream>

#include<opencv\cv.h>
#include<opencv\highgui.h>
#include<opencv\cxcore.h>

#include<stdio.h>
#include<Windows.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<string.h>
#include<assert.h>
#include<string.h>
#define unsigned char uchar

int N_MASK[3][3] = { { 0,1,0 }, { 1,1,1 }, {0,1,0} };

int** i_alloc(int size_x, int size_y)
{

	int** m;
	int i;

	if ((m = (int**)calloc(size_y, sizeof(int*))) == NULL)
	{
		printf("i_alloc error 1\7\n");
		exit(0);
	}

	for (i = 0; i < size_y; i++)
		if ((m[i] = (int*)calloc(size_x, sizeof(int))) == NULL) {
			printf("i_alloc error 2\7\n");
			exit(0);

		}
	return m;
}
uchar** uc_alloc(int size_x, int size_y)
{

	uchar** m;
	int i;

	if ((m = (uchar * *)calloc(size_y, sizeof(uchar*))) == NULL)
	{
		printf("uc_alloc error 1\7\n");
		exit(0);
	}

	for (i = 0; i < size_y; i++)
		if ((m[i] = (uchar*)calloc(size_x, sizeof(uchar))) == NULL) {
			printf("uc_alloc error 2\7\n");
			exit(0);

		}
	return m;
}
void read_ucmatrix(int size_x, int size_y, uchar** ucmatrix, char* filename)
{
	int i;
	FILE* f;
	if ((fopen_s(&f, filename, "rb")) != NULL)
	{
		printf("%s File open Error! \n", filename);
		exit(0);

	}
	for (i = 0; i < size_y; i++)
		if (fread(ucmatrix[i], sizeof(uchar), size_x, f) != size_x)
		{
			printf("Data Read Error! \n");
			exit(0);

		}
	fclose(f);
}


void MakeBinary(uchar** img, uchar** out, int Row, int Col, double avg) {

	int i, j;

	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			if (img[i][j] > avg) out[i][j] = 255;   //Change value for book with avg-30

			else out[i][j] = 0;
		}
	}

}void Erosion(uchar** Data_In, uchar** Data_Out, int Size, int BLK_SIZE)
{
	int i, j, smin, value;
	int indexi, indexj;
	int maski, maskj;

	for (i = -Size; i < BLK_SIZE - Size; i++)
	{
		for (j = -Size; j < BLK_SIZE - Size; j++)
		{
			smin = 256;
			for (maski = 0; maski <= 2 * Size; maski++)
			{
				for (maskj = 0; maskj <= 2 * Size; maskj++)
				{
					if (N_MASK[maski][maskj] != 0)
					{
						indexi = i + maski;
						indexj = j + maskj;

						if (indexi < 0) indexi = -indexi;
						else if (indexi >= BLK_SIZE) indexi = 2 * BLK_SIZE - indexi - 1;
						if (indexj < 0) indexj = -indexj;
						else if (indexj >= BLK_SIZE) indexj = 2 * BLK_SIZE - indexj - 1;

						//circular method
						/*if (indexi < 0) indexi += BLK_SIZE;
						else if (indexi >= BLK_SIZE) indexi -= BLK_SIZE;
						if (indexj < 0) indexj += BLK_SIZE;
						else if (indexj >= BLK_SIZE) indexj -= BLK_SIZE;*/

						if (Data_In[indexi][indexj] < smin)
						{
							value = (int)Data_In[indexi][indexj] - (int)N_MASK[maski][maskj];
							if (value < smin) smin = value;
						}
					}
				}
			}
			Data_Out[i + Size][j + Size] = (smin < 0) ? (uchar)0 : (uchar)smin;
		}
	}
}

void Dilation(uchar** Data_In, uchar** Data_Out, int Size, int BLK_SIZE)
{
	int i, j, smax, value;
	int indexi, indexj;
	int maski, maskj;

	for (i = -Size; i < BLK_SIZE - Size; i++)
	{
		for (j = -Size; j < BLK_SIZE - Size; j++)
		{
			smax = 0;
			for (maski = 0; maski <= 2 * Size; maski++)
			{
				for (maskj = 0; maskj <= 2 * Size; maskj++)
				{
					if (N_MASK[maski][maskj] != 0)
					{
						indexi = i + maski;
						indexj = j + maskj;

						if (indexi < 0) indexi = -indexi;
						else if (indexi >= BLK_SIZE) indexi = 2 * BLK_SIZE - indexi - 1;
						if (indexj < 0) indexj = -indexj;
						else if (indexj >= BLK_SIZE) indexj = 2 * BLK_SIZE - indexj - 1;

						//circular method
						/*if (indexi < 0) indexi += BLK_SIZE;
						else if (indexi >= BLK_SIZE) indexi -= BLK_SIZE;
						if (indexj < 0) indexj += BLK_SIZE;
						else if (indexj >= BLK_SIZE) indexj -= BLK_SIZE;*/

						if (Data_In[indexi][indexj] > smax)
						{
							value = (int)Data_In[indexi][indexj] + (int)N_MASK[maski][maskj];
							if (value > smax) smax = value;
						}
					}
				}
			}
			Data_Out[i + Size][j + Size] = (smax > 255) ? (uchar)255 : (uchar)smax;
		}
	}
}
//void Dilation(uchar** Data_In, uchar** Data_Out, int Size, int BLK_SIZE)
//{
//   int i, j, smax, value;
//   int maski, maskj;
//   int indexi, indexj;
//   
//   //int** N_MASK;
//   for (i = -Size; i < BLK_SIZE - Size; i++) {
//      for (j = -Size; j < BLK_SIZE - Size; j++) {
//         smax = 0;
//         for (maski = 0; maski <= 2 * Size; maski++) {
//            for (maskj = 0; maskj <= 2 * Size; maskj++) {
//               if (N_MASK[maski][maskj] != 0) {
//                  indexi = i + maski;
//                  indexj = j + maskj;
//
//                  if (indexi < 0) indexi = -indexi;
//                  else if (indexi >= BLK_SIZE) indexi = 2 * BLK_SIZE - indexi - 1;
//                  if (indexj < 0) indexj = -indexj;
//                  else if (indexj >= BLK_SIZE) indexj = 2 * BLK_SIZE - indexj - 1;
//                  if (Data_In[indexi][indexj] > smax) {
//                     value = (int)Data_In[indexi][indexj] + (int)N_MASK[maski][maskj];
//                     if (value > smax)smax = value;
//                  }
//               }
//            }
//         }
//         Data_Out[i + Size][j + Size] = (smax > 255) ? (uchar)255 : (uchar)smax;
//      }
//   }
//}
void BinaryErosion(uchar** Data_In, uchar** Data_Out, int Size, int BLK_SIZE)
{
	int i, j, count;
	int indexi, indexj;
	int maski, maskj;

	for (i = -Size; i < BLK_SIZE - Size; i++) {
		for (j = -Size; j < BLK_SIZE - Size; j++) {
			count = 0;
			for (maski = 0; maski <= 2 * Size; maski++) {
				for (maskj = 0; maskj <= 2 * Size; maskj++) {
					if (N_MASK[maski][maskj] != 0) {
						indexi = i + maski;
						indexj = j + maskj;

						if (indexi < 0) indexi = -indexi;
						else if (indexi >= BLK_SIZE)indexi = 2 * BLK_SIZE - indexi - 1;
						if (indexj < 0)indexj = -indexj;
						else if (indexj >= BLK_SIZE)indexj = 2 * BLK_SIZE - indexj - 1;


						if (Data_In[indexi][indexj] == 255) {
							count++;
						}
					}
				}
			}
			if (count == 5)
				Data_Out[i + Size][j + Size] = 255;
			else
				Data_Out[i + Size][j + Size] = 0;
		}
	}
}
void BinaryDilation(uchar** Data_In, uchar** Data_Out, int Size, int BLK_SIZE)
{
	int i, j, count;
	int maski, maskj;
	int indexi, indexj;

	for (i = -Size; i < BLK_SIZE - Size; i++) {
		for (j = -Size; j < BLK_SIZE - Size; j++) {
			count = 0;
			for (maski = 0; maski <= 2 * Size; maski++) {
				for (maskj = 0; maskj <= 2 * Size; maskj++) {
					if (N_MASK[maski][maskj] != 0) {
						indexi = i + maski;
						indexj = j + maskj;

						if (indexi < 0) indexi = -indexi;
						else if (indexi >= BLK_SIZE)indexi = 2 * BLK_SIZE - indexi - 1;
						if (indexj < 0)indexj = -indexj;
						else if (indexj >= BLK_SIZE)indexj = 2 * BLK_SIZE - indexj - 1;

						if (Data_In[indexi][indexj] == 255) {
							count++;
						}
					}
				}
			}
			if (count)
				Data_Out[i + Size][j + Size] = 255;
			else
				Data_Out[i + Size][j + Size] = 0;
		}
	}
}
void Opening(uchar** Data_In, uchar** Data_Out, int sizex, int sizey)
{
	uchar** TempBLK;
	TempBLK = uc_alloc(sizex, sizey);
	Erosion(Data_In, TempBLK, 1, sizey);
	Dilation(TempBLK, Data_Out, 1, sizex);
}

void Closing(uchar** Data_In, uchar** Data_Out, int sizex, int sizey)
{
	uchar** TempBLK;
	TempBLK = uc_alloc(sizex, sizey);
	Dilation(Data_In, TempBLK, 1, sizex);
	Erosion(TempBLK, Data_Out, 1, sizey);
}
void AddSub(uchar** img, uchar** Result, int Row, int Col, int flag, int factor)
{
	int i, j;
	double tmp;

	for (i = 0; i < Row; i++)
		for (j = 0; j < Col; j++)
		{
			if (flag == 0)
			{
				tmp = img[i][j] = tmp;
			}
			else
			{
				tmp = img[i][j] - factor;

				if (tmp > 255) tmp = 255;
				else if (tmp < 0) tmp = 0;

				Result[i][j] = tmp;
			}
		}
}

void write_ucmatrix(int size_x, int size_y, uchar** ucmatrix, char* filename)
{
	int i;
	FILE* f;

	if ((fopen_s(&f, filename, "wb")) != NULL)
	{
		printf("%s File open Error! \n", filename);
		exit(0);
	}

	for (i = 0; i < size_y; i++)
		if (fwrite(ucmatrix[i], sizeof(uchar), size_x, f) != size_x)
		{
			printf("Data Write Error! \n");
			exit(0);
		}
	fclose(f);

}
int main(int argc, char* argv[])
{
	srand(time(NULL));
	double sum;
	int i, j;
	int position, num;

	IplImage* cvImg;
	CvSize imgSize;

	uchar** img;
	uchar** outimg = 0;
	uchar** outimg8;
	uchar** outimg7;
	uchar** outimg6;
	uchar** outimg5;

	if (argc != 4)
	{
		printf("Exe imgData x_size y_size \n");
		exit(0);
	}
	imgSize.width = atoi(argv[2]);
	imgSize.height = atoi(argv[3]);


	img = uc_alloc(imgSize.width, imgSize.height); // 메모리 할당
	outimg = uc_alloc(imgSize.width, imgSize.height); // 메모리 할당
	outimg8 = uc_alloc(imgSize.width, imgSize.height); // 메모리 할당
	outimg7 = uc_alloc(imgSize.width, imgSize.height); // 메모리 할당
	outimg6 = uc_alloc(imgSize.width, imgSize.height); // 메모리 할당
	outimg5 = uc_alloc(imgSize.width, imgSize.height); // 메모리 할당 */
	read_ucmatrix(imgSize.width, imgSize.height, img, argv[1]); // 레나 이미지 읽기

	cvImg = cvCreateImage(imgSize, 8, 1);

	MakeBinary(img, img, imgSize.width, imgSize.height, 123);
	//Opening(img, outimg, imgSize.width, imgSize.height);
	BinaryDilation(img, outimg, 1, imgSize.height);
	for (i = 0; i < imgSize.height; i++)
		for (j = 0; j < imgSize.width; j++)
		{
			((uchar*)(cvImg->imageData + cvImg->widthStep * i))[j] = outimg[i][j];
		}
	cvShowImage("lena BinaryDilaion", cvImg);
	BinaryErosion(img, outimg, 1, imgSize.height);
	//Closing(img, outimg, imgSize.width, imgSize.height);
	for (i = 0; i < imgSize.height; i++)
		for (j = 0; j < imgSize.width; j++)
		{
			((uchar*)(cvImg->imageData + cvImg->widthStep * i))[j] = outimg[i][j];
		}
	cvNamedWindow("lena BinaryErosion", 1);
	cvShowImage("lena BinaryErosion", cvImg);
	cvWaitKey(0);
	cvDestroyWindow("image");
	cvReleaseImage(&cvImg);

	getchar();
	getchar();

	return 0;


}