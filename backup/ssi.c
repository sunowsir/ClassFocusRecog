/*
	* File     : ssi.c
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年02月26日 星期日 19时56分24秒
*/

// #include "ssi.h"


//test.c
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <stdio.h>

int main(int argc, char** argv) {
	//only support png image
	if ( argc != 2 )
	{
		printf("usage: test <Image_Path>\n");
		return -1;
	}

	CvMat* mat = cvLoadImageM(argv[1], CV_LOAD_IMAGE_COLOR);
	if (!mat) {
		printf("cvLoadImageM error\n");
		return -1;
	}
	cvNamedWindow("image", CV_WINDOW_AUTOSIZE);


	/* CvRect r = cvRect(20, 20, 400, 400); //x,y,w,h */
	/* CvScalar c = cvScalar(255, 255, 0, 0); */
	/* cvRectangleR(mat, r, c, 5, 8, 0); */


	CvPoint p1 = cvPoint(20, 20); //x,y
	CvPoint p2 = cvPoint(420, 420); //x,y
	CvScalar c = cvScalar(255, 255, 0, 0);
	cvRectangle(mat, p1, p2, c, 5, 8, 0);


	cvShowImage ("image", mat);
	cvWaitKey(0);
}
