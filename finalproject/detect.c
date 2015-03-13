#include <stdio.h>

#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui_c.h>

#define THR 20

struct ctx {

	CvCapture	    *capture;	/* Capture handle */
	IplImage 		*image;
	IplImage		*hsv_image;
	IplImage 		*red_image;
	IplImage 		*blue_image;
	IplImage 		*green_image;
	IplImage 		*hue_image;
	IplImage 		*sat_image;
	IplImage 		*val_image;
	IplImage 		*red_image_thr;
	IplImage 		*blue_image_thr;
	IplImage 		*green_image_thr;
	IplImage 		*hue_image_thr;
	IplImage 		*sat_image_thr;
	IplImage 		*val_image_thr;
	IplImage 		*thr_image_rgb;
	IplImage 		*thr_image_hsv;
	IplImage 		*thr_image;

	int				pix_red;
	int				pix_green;
	int				pix_blue;
	int				pix_hue;
	int				pix_sat;
	int				pix_val;
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
	//cvNamedWindow("sliders", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("output", 50, 50);
	cvMoveWindow("thresholded", 700, 50);
	//cvMoveWindow("sliders", 50, 550);
	
	
}

void init_images(struct ctx *ctx)
{
	ctx->hsv_image = cvCreateImage(cvGetSize(ctx->image), 8, 3);
	ctx->thr_image = cvCreateImage(cvGetSize(ctx->image), 8, 1);
	ctx->thr_image_rgb = cvCreateImage(cvGetSize(ctx->image), 8, 1);
	ctx->thr_image_hsv = cvCreateImage(cvGetSize(ctx->image), 8, 1);
}

void filter(struct ctx *ctx) 
{
	cvCvtColor(ctx->image, ctx->hsv_image, CV_BGR2HSV);
	cvInRangeS(ctx->image,
			   cvScalar(ctx->pix_red - THR,ctx->pix_green - THR,ctx->pix_blue - THR,255),
			   cvScalar(ctx->pix_red + THR,ctx->pix_green + THR,ctx->pix_blue + THR,255),
			   ctx->thr_image_rgb);
	cvInRangeS(ctx->hsv_image, cvScalar(100,100,100,100), cvScalar(255,255,255,255), ctx->thr_image_hsv);
	cvMul(ctx->thr_image_rgb, ctx->thr_image_hsv, ctx->thr_image, 1);
	
}

void on_mouse(int event, int x, int y, int flags, void* param, struct ctx *ctx)
{
	if (event == CV_EVENT_LBUTTONDOWN){
		getPixel(&ctx, x, y);
		printf("X: %d, Y: %d \n", x, y);
	}
}

void getPixel(struct ctx *ctx, int x, int y)
{
	CvScalar rgb;
	CvScalar hsv;
	rgb= cvGet2D(ctx->image, x,y);
	hsv= cvGet2D(ctx->hsv_image, x,y);
	
	ctx->pix_red = rgb.val[0];
	ctx->pix_green = rgb.val[1];
	ctx->pix_blue = rgb.val[2];
	ctx->pix_hue = hsv.val[0];
	ctx->pix_sat = hsv.val[1];
	ctx->pix_val = hsv.val[2];
	//printf("B=%f, G=%f, R=%f, H=%f, S=%f, V=%f\n",rgb.val[0],rgb.val[1],rgb.val[2],hsv.val[0],hsv.val[1]);
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
	
	cvSetMouseCallback("output", on_mouse, 0);

	init_capture(&ctx);
	init_windows();
	init_images(&ctx);

	do {
		ctx.image = cvQueryFrame(ctx.capture);
		
		filter(&ctx);
		display(&ctx);

		key = cvWaitKey(1);
	} while (key != 'q');

	return 0;
}
