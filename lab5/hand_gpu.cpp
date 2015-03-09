
#include <stdio.h>
#include <iostream>

#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui_c.h>
#include "opencv2/gpu/gpu.hpp"
#include "opencv2/opencv.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define SERVER_PORT htons(50007)

#define VIDEO_FILE	"video.avi"
#define VIDEO_FORMAT	CV_FOURCC('M', 'J', 'P', 'G')
#define NUM_FINGERS	5
#define NUM_DEFECTS	8

#define RED     CV_RGB(255, 0, 0)
#define GREEN   CV_RGB(0, 255, 0)
#define BLUE    CV_RGB(0, 0, 255)
#define YELLOW  CV_RGB(255, 255, 0)
#define PURPLE  CV_RGB(255, 0, 255)
#define GREY    CV_RGB(200, 200, 200)
#define BLACK   CV_RGB(0, 0, 0)
#define LAG_AMOUNT 3

using namespace std;
using namespace cv;
//using namespace cv::gpu;


struct ctx {

    CvCapture	    *capture;	/* Capture handle */
	CvVideoWriter	*writer;	/* File recording handle */
    VideoCapture cap;

	Mat	    image;		/* Input image */
	Mat	    thr_image;	/* After filtering and thresholding */
	//Mat	    temp_image1;	/* Temporary image (1 channel) */
	Mat 	temp_image3;	/* Temporary image (3 channels) */

    IplImage	   *ipl_thr_image;	/* After filtering and thresholding */
    IplImage	   *ipl_temp_image1;	/* Temporary image (1 channel) */
    IplImage	   *ipl_image;		/* Input image */

    gpu::GpuMat	    gpu_image;		/* Input image */
	gpu::GpuMat	    gpu_thr_image;	/* After filtering and thresholding */
	gpu::GpuMat  	gpu_temp_image3;	/* Temporary image (3 channels) */

	CvSeq		*contour;	/* Hand contour */
	CvSeq		*hull;		/* Hand convex hull */
    CvSeq		*contour2;	/* Hand contour */
	CvSeq		*hull2;		/* Hand convex hull */

	CvPoint		hand_center;
    CvPoint		hand_center2;
	CvPoint		*fingers;	/* Detected fingers positions */
	CvPoint		*defects;	/* Convexity defects depth points */
    CvPoint		*defects2;
    CvPoint     firstPt;
    CvPoint     firstPt2;

	CvMemStorage	*hull_st;
	CvMemStorage	*contour_st;
    CvMemStorage	*hull_st2;
	CvMemStorage	*contour_st2;
	CvMemStorage	*temp_st;
    CvMemStorage	*temp_st2;
	CvMemStorage	*defects_st;
    CvMemStorage	*defects_st2;

	Mat	kernel;	/* Kernel for morph operations */

	int		num_fingers;
	int		hand_radius;
    int		hand_radius2;
	int		num_defects;
    int		num_defects2;
    int     y_pos;
    int     y_pos2;
    int     frameCounter;
	int		clientNum;
};

void init_comm(struct ctx *ctx)
{
	char buffer[1000];


    int serverSock=socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = SERVER_PORT;
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    /* bind (this socket, local address, address length)
       bind server socket (serverSock) to server address (serverAddr).  
       Necessary so that server can use a specific port */ 
    bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr));

    // wait for a client
    /* listen (this socket, request queue length) */
    listen(serverSock,1);
	bzero(buffer, 1000);

    sockaddr_in clientAddr;
    socklen_t sin_size=sizeof(struct sockaddr_in);
    int clientSock=accept(serverSock,(struct sockaddr*)&clientAddr, &sin_size);
	ctx->clientNum = clientSock;
}

void init_images(struct ctx *ctx)
{
	
    ctx->capture = cvCaptureFromCAM(0);
	if (!ctx->capture) {
		fprintf(stderr, "Error initializing capture\n");
		exit(1);
	}
	ctx->ipl_image = cvQueryFrame(ctx->capture);  
	ctx->image = Mat(ctx->ipl_image);

    cvNamedWindow("output", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("thresholded", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("output", 50, 50);
	cvMoveWindow("thresholded", 700, 50);

    ctx->thr_image = cvarrToMat(cvCreateImage(cvGetSize(ctx->ipl_image), 8, 1));
    ctx->ipl_thr_image = cvCreateImage(cvGetSize(ctx->ipl_image), 8, 1);
	//ctx->temp_image1 = Mat(cvCreateImage(cvGetSize(ctx->ipl_image), 8, 1));
    ctx->ipl_temp_image1 = cvCreateImage(cvGetSize(ctx->ipl_image), 8, 1);
	ctx->temp_image3 = cvarrToMat(cvCreateImage(cvGetSize(ctx->ipl_image), 8, 3));
	//ctx->kernel = cvCreateStructuringElementEx(9, 9, 4, 4, CV_SHAPE_RECT,NULL);
	Mat element = getStructuringElement(MORPH_RECT, Size(9, 9), Point(4, 4) );

	ctx->contour_st = cvCreateMemStorage(0);
	ctx->hull_st = cvCreateMemStorage(0);
	ctx->temp_st = cvCreateMemStorage(0);
	ctx->fingers = (CvPoint*)calloc(NUM_FINGERS + 1, sizeof(CvPoint));
	ctx->defects = (CvPoint*)calloc(NUM_DEFECTS, sizeof(CvPoint));
}

void init_recording(struct ctx *ctx)
{
	int fps, width, height;

	fps = cvGetCaptureProperty(ctx->capture, CV_CAP_PROP_FPS);
	width = cvGetCaptureProperty(ctx->capture, CV_CAP_PROP_FRAME_WIDTH);
	height = cvGetCaptureProperty(ctx->capture, CV_CAP_PROP_FRAME_HEIGHT);

	if (fps < 0)
		fps = 10;

	ctx->writer = cvCreateVideoWriter(VIDEO_FILE, VIDEO_FORMAT, fps,
					  cvSize(width, height), 1);

	if (!ctx->writer) {
		fprintf(stderr, "Error initializing video writer\n");
		exit(1);
	}
}

void filter_threshold(struct ctx *ctx)
{
	ctx->gpu_image.upload(ctx->image);
	
	gpu::cvtColor(ctx->gpu_image, ctx->gpu_temp_image3, CV_BGR2HSV); 
	ctx->gpu_temp_image3.download(ctx->temp_image3);  
   
	inRange(ctx->temp_image3,
		            Scalar(0, 55, 40, 255),
		            Scalar(40, 175, 120, 255), ctx->temp_image3); //CHANGE TO 1 CHANNEL
	
    ctx->gpu_temp_image3.upload(ctx->temp_image3);
    
    gpu::GaussianBlur(ctx->gpu_temp_image3, ctx->gpu_temp_image3, Size(11,11), 0, 0);
   
	
    /* Apply morphological opening */
	gpu::morphologyEx(ctx->gpu_temp_image3, ctx->gpu_temp_image3, MORPH_OPEN, ctx->kernel);
    gpu::GaussianBlur(ctx->gpu_temp_image3, ctx->gpu_thr_image, Size(3,3), 0, 0);
    
    ctx->gpu_thr_image.download(ctx->thr_image);  
	medianBlur(ctx->thr_image, ctx->thr_image, 11);
}

void find_contour(struct ctx *ctx)
{
	double area, area2, max_area = 0.0, max_area2 = 0.0;
	CvSeq *contours, *contours2, *tmp, *tmp2, *contour = NULL, *contour2 = NULL;

    //Convert thr_image from Mat back to IplImage
	ctx->ipl_thr_image = cvCreateImage(cvSize(ctx->thr_image.cols,ctx->thr_image.rows),8,1);
	IplImage ipltemp=ctx->thr_image;
	cvCopy(&ipltemp,ctx->ipl_thr_image);

	/* cvFindContours modifies input image, so make a copy */
	cvCopy(ctx->ipl_thr_image, ctx->ipl_temp_image1, NULL);
    cvRectangle(ctx->ipl_temp_image1, cvPoint(0,0), cvPoint(cvGetSize(ctx->ipl_image).width/2,cvGetSize(ctx->ipl_image).height), BLACK,  CV_FILLED,8, 0 );

	cvFindContours(ctx->ipl_temp_image1, ctx->temp_st, &contours,
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

	/* cvFindContours modifies input image, so make a copy */
	cvCopy(ctx->ipl_thr_image, ctx->ipl_temp_image1, NULL);
	/* Approximate contour with poly-line */
	if (contour) {
		contour = cvApproxPoly(contour, sizeof(CvContour),
				       ctx->contour_st, CV_POLY_APPROX_DP, 2,
				       1);
		ctx->contour = contour;
	}

    cvRectangle(ctx->ipl_temp_image1, cvPoint(cvGetSize(ctx->ipl_image).width/2,0), cvPoint(cvGetSize(ctx->ipl_image).width,cvGetSize(ctx->ipl_image).height), BLACK,  CV_FILLED,8, 0 );

	cvFindContours(ctx->ipl_temp_image1, ctx->temp_st, &contours2,
		       sizeof(CvContour), CV_RETR_EXTERNAL,
		       CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	/* Select contour having greatest area */
	for (tmp2 = contours2; tmp2; tmp2 = tmp2->h_next) {
        area2 = fabs(cvContourArea(tmp2, CV_WHOLE_SEQ, 0));
        if (area2 > max_area2) {
            max_area2 = area2;
            contour2 = tmp2;
        }
    }

	/* Approximate contour with poly-line */
	if (contour2) {
		contour2 = cvApproxPoly(contour2, sizeof(CvContour),
				       ctx->contour_st2, CV_POLY_APPROX_DP, 2,
				       1);
		ctx->contour2 = contour2;
	}
    
}

void find_convex_hull(struct ctx *ctx)
{
	CvSeq *defects;
    CvSeq *defects2;
	CvConvexityDefect *defect_array;
    CvConvexityDefect *defect_array2;
	int i;
	int x = 0, y = 0;
	int dist = 0;

	ctx->hull = NULL;
    ctx->hull2 = NULL;

	if (!ctx->contour){
		return;
	}
    if (!ctx->contour2){
		return;
	}

	ctx->hull = cvConvexHull2(ctx->contour, ctx->hull_st, CV_CLOCKWISE, 0);
    ctx->hull2 = cvConvexHull2(ctx->contour2, ctx->hull_st2, CV_CLOCKWISE, 0);

	if (ctx->hull) {

		/* Get convexity defects of contour w.r.t. the convex hull */
		defects = cvConvexityDefects(ctx->contour, ctx->hull,
					     ctx->defects_st);

		if (defects && defects->total) {
			defect_array = (CvConvexityDefect*)calloc(defects->total,
					      sizeof(CvConvexityDefect));
			cvCvtSeqToArray(defects, defect_array, CV_WHOLE_SEQ);

			/* Average depth points to get hand center */
			for (i = 0; i < defects->total && i < NUM_DEFECTS; i++) {
				x += defect_array[i].depth_point->x;
				y += defect_array[i].depth_point->y;

				ctx->defects[i] = cvPoint(defect_array[i].depth_point->x,
							  defect_array[i].depth_point->y);
			}

			x /= defects->total;
			y /= defects->total;

			ctx->num_defects = defects->total;
			ctx->hand_center = cvPoint(x, y);

			/* Compute hand radius as mean of distances of
			   defects' depth point to hand center */
			for (i = 0; i < defects->total; i++) {
				int d = (x - defect_array[i].depth_point->x) *
					(x - defect_array[i].depth_point->x) +
					(y - defect_array[i].depth_point->y) *
					(y - defect_array[i].depth_point->y);

				dist += sqrt(d);
			}
			ctx->hand_radius = dist / defects->total;
			free(defect_array);
		}
	}
    if (ctx->hull2) {
        int x = 0, y = 0;
	    int dist = 0;
		// Get convexity defects of contour w.r.t. the convex hull 
		defects2 = cvConvexityDefects(ctx->contour2, ctx->hull2,
					     ctx->defects_st2);

		if (defects2 && defects2->total) {
			defect_array2 = (CvConvexityDefect*)calloc(defects2->total,
                            sizeof(CvConvexityDefect));
			cvCvtSeqToArray(defects2, defect_array2, CV_WHOLE_SEQ);
            
			// Average depth points to get hand center 
			for (i = 0; i < defects2->total && i < NUM_DEFECTS; i++) {
				x += defect_array2[i].depth_point->x;
				y += defect_array2[i].depth_point->y;
                
				//ctx->defects2[i] = cvPoint(defect_array2[i].depth_point->x,
				//			  defect_array2[i].depth_point->y);

			}
    
			x /= defects2->total;
			y /= defects2->total;

			ctx->num_defects2 = defects2->total;
			ctx->hand_center2 = cvPoint(x, y);

			//Compute hand radius as mean of distances of
			//   defects' depth point to hand center 
			for (i = 0; i < defects2->total; i++) {
				int d = (x - defect_array2[i].depth_point->x) *
					(x - defect_array2[i].depth_point->x) +
					(y - defect_array2[i].depth_point->y) *
					(y - defect_array2[i].depth_point->y);

				dist += sqrt(d);
			}

			ctx->hand_radius2 = dist / defects2->total;
			free(defect_array2);
		}
	}
    
}

void display(struct ctx *ctx)
{
    int velLeft, velRight, n;
    ctx->frameCounter++;	
    // Calculate velocity every 3 frames (reduce noise)
    /*if (ctx->frameCounter % LAG_AMOUNT == 0){   
        velLeft = ctx->y_pos - ctx->hand_center.y;
        printf("Velocity Left: %d \t", velLeft);
        ctx->y_pos = ctx->hand_center.y; // Update value
        velRight = ctx->y_pos2 - ctx->hand_center2.y;
        printf("Velocity Right: %d \n", velRight);
        ctx->y_pos2 = ctx->hand_center2.y; // Update value
    }*/
	
	if (ctx->hand_center.y > 300)
		n = write(ctx->clientNum, "D", 1);
    if (ctx->hand_center.y < 180)
		n = write(ctx->clientNum, "U", 1);
    
	//if (ctx->num_fingers == NUM_FINGERS) {
    if (1 == 1){
#if defined(SHOW_HAND_CONTOUR)
		cvDrawContours(ctx->ipl_image, ctx->contour, BLUE, GREEN, 0, 1,
			       CV_AA, cvPoint(0, 0));
        cvDrawContours(ctx->ipl_image, ctx->contour2, BLUE, GREEN, 0, 1,
			       CV_AA, cvPoint(0, 0));
#endif
		cvCircle(ctx->ipl_image, ctx->hand_center, 5, RED, 1, CV_AA, 0);
		cvCircle(ctx->ipl_image, ctx->hand_center, ctx->hand_radius,
			 RED, 1, CV_AA, 0);
        cvCircle(ctx->ipl_image, ctx->hand_center2, 5, GREEN, 1, CV_AA, 0);
		cvCircle(ctx->ipl_image, ctx->hand_center2, ctx->hand_radius2,
			 GREEN, 1, CV_AA, 0);

		/*for (i = 0; i < ctx->num_fingers; i++) {

			cvCircle(ctx->image, ctx->fingers[i], 10,
				 GREEN, 3, CV_AA, 0);

			cvLine(ctx->image, ctx->hand_center, ctx->fingers[i],
			       YELLOW, 1, CV_AA, 0);
		}

		for (i = 0; i < ctx->num_defects; i++) {
			cvCircle(ctx->image, ctx->defects[i], 2,
				 GREY, 2, CV_AA, 0);
		}*/
	}

	cvShowImage("output", ctx->ipl_image);
	cvShowImage("thresholded", ctx->ipl_thr_image);
}

int main(int argc, char **argv)
{
	struct ctx ctx = { };
	int key;

	init_comm(&ctx);
	init_images(&ctx);
	init_recording(&ctx);

	do {
		ctx.ipl_image = cvQueryFrame(ctx.capture);
        ctx.image = Mat(ctx.ipl_image);

		filter_threshold(&ctx);
		find_contour(&ctx);
		find_convex_hull(&ctx);
		//find_fingers(&ctx);

		display(&ctx);
		cvWriteFrame(ctx.writer, ctx.ipl_image);

		key = cvWaitKey(1);
	} while (key != 'q');

	return 0;
}
