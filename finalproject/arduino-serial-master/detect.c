#include <stdio.h>

#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui_c.h>

#include "arduino-serial-lib.h"

#define THR 30

int xClick;
int yClick;
int mouseClicked = 0;

struct ctx {

	CvCapture	    *capture;	/* Capture handle */
	IplImage 		*image;
	IplImage		*hsv_image;
	IplImage 		*thr_image_rgb;
	IplImage 		*thr_image_hsv;
	IplImage 		*thr_image;
	IplImage		*temp_image1;
	
	CvSeq			*contour;
	CvMemStorage	*contour_st;
	CvMemStorage	*temp_st;

	int				pix_red;
	int				pix_green;
	int				pix_blue;
	int				pix_hue;
	int				pix_sat;
	int				pix_val;
	int				xCoor;
	int				yCoor;
};

void init_capture(struct ctx *ctx)
{
	ctx->capture = cvCaptureFromCAM(0);
	if (!ctx->capture) {
		fprintf(stderr, "Error initializing capture\n");
		exit(1);
	}
	ctx->image = cvQueryFrame(ctx->capture);  
}

void init_windows(void)
{
	cvNamedWindow("output", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("thresholded", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("output", 50, 50);
	cvMoveWindow("thresholded", 700, 50);
}

void init_images(struct ctx *ctx)
{
	ctx->hsv_image = cvCreateImage(cvGetSize(ctx->image), 8, 3);
	ctx->thr_image = cvCreateImage(cvGetSize(ctx->image), 8, 1);
	ctx->thr_image_rgb = cvCreateImage(cvGetSize(ctx->image), 8, 1);
	ctx->thr_image_hsv = cvCreateImage(cvGetSize(ctx->image), 8, 1);
	ctx->temp_image1 = cvCreateImage(cvGetSize(ctx->image), 8, 1);
	
	ctx->contour_st = cvCreateMemStorage(0);
	ctx->temp_st = cvCreateMemStorage(0);
}

void filter(struct ctx *ctx) 
{
	cvCvtColor(ctx->image, ctx->hsv_image, CV_BGR2HSV);
	cvInRangeS(ctx->image,
			   cvScalar(ctx->pix_red - THR,ctx->pix_green - THR,ctx->pix_blue - THR,255),
			   cvScalar(ctx->pix_red + THR,ctx->pix_green + THR,ctx->pix_blue + THR,255),
			   ctx->thr_image_rgb);
	cvInRangeS(ctx->hsv_image, 
			   cvScalar(ctx->pix_hue - THR,ctx->pix_sat - THR,ctx->pix_val - THR,255),
			   cvScalar(ctx->pix_hue + THR,255,ctx->pix_val + THR,255), 
			   ctx->thr_image_hsv);
	cvMul(ctx->thr_image_rgb, ctx->thr_image_hsv, ctx->thr_image, 1);
	cvSmooth(ctx->thr_image, ctx->thr_image, CV_GAUSSIAN, 5, 5, 0, 0);
	
}

void on_mouse(int event, int x, int y, int flags, void* param)
{
	if (event == CV_EVENT_LBUTTONDOWN){
		mouseClicked = 1;
		xClick= x;
		yClick = y;
		printf("X: %d, Y: %d \n", x, y);
	}
}

void getPixel(struct ctx *ctx)
{
	CvScalar rgb;
	CvScalar hsv;
	rgb= cvGet2D(ctx->image, yClick, xClick);
	hsv= cvGet2D(ctx->hsv_image, yClick, xClick);
	
	ctx->pix_red = rgb.val[0];
	ctx->pix_green = rgb.val[1];
	ctx->pix_blue = rgb.val[2];
	ctx->pix_hue = hsv.val[0];
	ctx->pix_sat = hsv.val[1];
	ctx->pix_val = hsv.val[2];
}

void identifyTarget(struct ctx *ctx)
{
	double area, max_area = 0.0;
	CvSeq *contours, *tmp, *contour = NULL;

	/* cvFindContours modifies input image, so make a copy */
	cvCopy(ctx->thr_image, ctx->temp_image1, NULL);
	cvFindContours(ctx->temp_image1, ctx->temp_st, &contours,
		       sizeof(CvContour), CV_RETR_EXTERNAL,
		       CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	/* Select contour having greatest area */
	for (tmp = contours; tmp; tmp = tmp->h_next) {
        area = fabs(cvContourArea(tmp, CV_WHOLE_SEQ, 0));
        if (area > max_area) {
            max_area = area;
            contour = tmp;
        }
    }

	/* Approximate contour with poly-line */
	if (contour) {
		contour = cvApproxPoly(contour, sizeof(CvContour),
				       ctx->contour_st, CV_POLY_APPROX_DP, 2,
				       1);
		ctx->contour = contour;
	}
	
	CvRect br;
	CvPoint center;
	
    br = cvBoundingRect(contour, 0);
    center.x = br.x+br.width/2;
    center.y = br.y+br.height/2;

    cvRectangle(ctx->image, cvPoint(br.x, br.y),
                cvPoint(br.x+br.width, br.y+br.height),
                cvScalar(0, 0, 255,0),
                2,8,0);

    cvCircle(ctx->image, center, 10, cvScalar(255, 255, 0,0),3 ,8,0);
}

void display(struct ctx *ctx)
{
	cvShowImage("output", ctx->image);
	cvShowImage("thresholded", ctx->thr_image);
}

int main(int argc, char **argv)
{
	struct ctx ctx = { };
	int key;
	init_capture(&ctx);
	init_windows();
	init_images(&ctx);

	do {
		ctx.image = cvQueryFrame(ctx.capture);
		filter(&ctx);
		cvSetMouseCallback("output", on_mouse, 0);
		if(mouseClicked == 1){
			getPixel(&ctx);
			mouseClicked = 0;
		}
		identifyTarget(&ctx);
		display(&ctx);
		printf("X: %d, Y: %d \n", xClick, yClick);

		key = cvWaitKey(1);
	} while (key != 'q');

	return 0;
}
