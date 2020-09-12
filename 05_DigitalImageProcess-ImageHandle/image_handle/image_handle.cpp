#include "image_handle.hpp"

static uchar find_min(uchar n1,uchar n2,uchar n3) 
{      
    return n1<n2?(n1<n3?n1:n3):(n2<n3?n2:n3);
}
static uchar find_max(uchar n1,uchar n2,uchar n3) 
{      
    return n1>n2?(n1>n3?n1:n3):(n2>n3?n2:n3);
}
static uchar find_med(uchar n1,uchar n2,uchar n3) 
{
    uchar temp;

    if (n1 > n2)
    {
        temp = n1;
        n1 = n2;
        n2 = temp;
    }
    if (n2 > n3)
    {
        temp = n2;
        n2 = n3;
        n3 = temp;
    }
    return n1 > n2?n1:n2;
}

static void sort(uchar* n1,uchar* n2,uchar* n3) 
{
    uchar temp;

    if (*n1 > *n2)
    {
        temp = *n2;
        *n2 = *n1;
        *n1 = temp;
    }
    if (*n2 > *n3)
    {
        temp = *n3;
        *n3 = *n2;
        *n2 = temp;
    }
    if (*n1 > *n2)
    {
        temp = *n2;
        *n2 = *n1;
        *n1 = temp;
    }
}

static uchar Median(uchar *kenel,uchar row_input[3],int k) 
{   
    uchar temp;
    int i ,j;
    uchar min,med,max;

    /* bubble sort*/
    for (i = 0; i < 3; i++)
    {   
        for (j = 0; j < 3 - i -1; j++)
        { 
            if (row_input[j] > row_input[j + 1])
            {
                temp = row_input[j];
                row_input[j] = row_input[j + 1];
                row_input[j + 1] = temp;
            }
        }
        *(kenel+3*(j+1)+k) = row_input[j + 1];
    }
    *(kenel+k) = row_input[0];
  
    min = find_min(*(kenel+6),*(kenel+7),*(kenel+8));
    med = find_med(*(kenel+3),*(kenel+4),*(kenel+5));
    max = find_max(*(kenel+0),*(kenel+1),*(kenel+2));
    return find_med(min,med,max);
    
#if 0
    #endif
}

static void rgb_to_gray(const Mat src, Mat &dst1)
{
    for (int i = 0; i < src.rows; i++)         
    {                  
        uchar*p = dst1.ptr<uchar>(i);
        for(int j = 0; j < src.cols; j++)
        {                            
            *p= (15*src.at<cv::Vec3b>(i, j)[0] + 75*src.at<cv::Vec3b>(i, j)[1] + 38*src.at<cv::Vec3b>(i, j)[2]) >> 7;
            p++;
        }         
    }
}

void histogram_equalization (const Mat src, Mat& dst)  //ֱ��ͼ���⻯
{
	printf("histogram_equalization\n");

	Mat gray,gray_temp,dst1;
	int pixNum[256] = { 0 };
	float pixP[256] = { 0 };
	float pixNormalize[256] = { 0 };
	int maxPix = 0, minPix = 0;
	int sum = 0;
	int k = 0;

	if (!src.data || !dst.data)
        return;

	gray.create(cv::Size(RGA_ALIGN(src.cols, 16), RGA_ALIGN(src.rows, 16)), CV_8UC1);
	gray_temp.create(cv::Size(RGA_ALIGN(src.cols, 16), RGA_ALIGN(src.rows, 16)), CV_8UC1);
	
	printf("%4d",sum);
	rgb_to_gray(src,gray); 

    //���ص�����
	for (int i = 0; i < gray.rows; i++)         
    {                  
        for(int j = 0; j < gray.cols; j++)
        {                            
			k = gray.at<cv::Vec3b>(i*gray.cols + j)[0]+gray.at<cv::Vec3b>(i*gray.cols + j)[1]+gray.at<cv::Vec3b>(i*gray.cols + j)[2];
			pixNum[k]++;
			//�ҳ����ص����ֵ����Сֵ
			if (maxPix < k)
				maxPix = k;
			else if (minPix > k)
				minPix = k;
        }         
    }
	//1.����ÿ���Ҷȼ������ص�ĸ���������ͼ������ռ�ĸ���
	for (int i = 0; i < 256; i++)
	{
		pixP[i] = (float)(pixNum[i]) / (float)(gray.rows * gray.cols);
	}

	//2.����ͼ����Ҷȼ����ۼƷֲ�����
	for (int m = 0; m < 256; m++)
	{
		for (int n = 0; n <= m; n++)
		{
			pixNormalize[m] += pixP[n];
		}
	}

	for (int im = 0; im < gray.rows; im++)
	{
		for (int jn = 0; jn < gray.cols; jn++)
		{
			int q = src.at<cv::Vec3b>(im*src.cols + jn)[0] + src.at<cv::Vec3b>(im*src.cols + jn)[1] + src.at<cv::Vec3b>(im*src.cols + jn)[2];
			gray.at<cv::Vec3b>(im*gray.cols + jn) = pixNormalize[q] * (maxPix - minPix) + minPix;
		}
	}
	//3.����Sk��ֵ�жϱ任��ĻҶȼ�

	fcv::cvtColor(gray,dst,CV_GRAY2BGR);
} 
void median_smooth_flitering(const Mat src, Mat &dst)  //��ֵƽ���˲�
{
	printf("median_smooth_flitering\n");
}

void edge_extraction(const Mat src, Mat &dst) //��Ե��ȡ
{
	printf("edge_extraction\n");
}

void sharpen_flitering(const Mat src, Mat &dst) //���˲�
{
	Mat gray gray_end;//����2������
	signed char kernel[3][3] = {
		{ 0, -1, 0},
		{ -1, 5, 1},
		{ 0, -1, 0}};
 
	if (!src.data || !dst.data)//һ���ж�
		return;
 
	//��������
	gray.create(cv::Size(RGA_ALIGN(src.cols, 16), RGA_ALIGN(src.rows, 16)), CV_8UC1);//RGA_ALIGN���ڶ����ʽ
 
	gray_end.create(cv::Size(RGA_ALIGN(src.cols, 16), RGA_ALIGN(src.rows, 16)), CV_8UC1);
 
	//rgbת�Ҷ�
	rgb_to_gray(src, gray);
 
	for (int i = 0+1; i < src.rows-1; i++)
	{
		//uchar* p1 = gray.ptr<uchar>(i);//�Ҷ�ͼ��һ��
		//uchar* p2 = gray_reverse.ptr<uchar>(i);//
		uchar* q = gray_end.ptr<uchar>(i);
		for (int j = 0+1; j < src.cols-1; j++)
		{
			gray_end.at<uchar>(i, j) = 0;
			gray_end.at<uchar>(i, j) += gray.at<uchar>(i - 1, j - 1) * kernel[0][0];
			gray_end.at<uchar>(i, j) += gray.at<uchar>(i - 1, j - 0) * kernel[0][1];
			gray_end.at<uchar>(i, j) += gray.at<uchar>(i - 1, j + 1) * kernel[0][2];
			gray_end.at<uchar>(i, j) += gray.at<uchar>(i - 0, j - 1) * kernel[1][0];
			gray_end.at<uchar>(i, j) += gray.at<uchar>(i - 0, j - 0) * kernel[1][1];
			gray_end.at<uchar>(i, j) += gray.at<uchar>(i - 0, j + 1) * kernel[1][2];
			gray_end.at<uchar>(i, j) += gray.at<uchar>(i + 1, j - 1) * kernel[2][0];
			gray_end.at<uchar>(i, j) += gray.at<uchar>(i + 1, j - 0) * kernel[2][1];
			gray_end.at<uchar>(i, j) += gray.at<uchar>(i + 1, j + 1) * kernel[2][2];
		}
	}
	fcv::cvtColor(gray_end, dst, CV_GRAY2BGR);//�Ҷ�תbgr
 
}

void bilinear_interpolation(const Mat src, Mat &dst) //˫���Բ�ֵ
{
	printf("bilinear_interpolation\n");
}

static uchar average(const Mat &img, int x_min,int x_max,int y_min,int y_max,int k)
{
    int count = (x_max- x_min + 1) * (y_max - y_min + 1);
    int sum = 0;
    for (int i = x_min; i <= x_max; i++)
    {
        for (int j = y_min; j <= y_max; j++)
        {
            //caculate the sum of all ���ص�
			sum += img.at<cv::Vec3b>(i, j)[k];  //k  0 - R, 1 - G, 2 - B
        }
    }
    return sum/count;
}

void scale_part_average (const Mat src, Mat &dst)
{


}

void relief(const Mat src, Mat &dst)
{
	
}

static void rgb_to_gray_to_reverse(const Mat src, Mat &dst1,Mat &dst2)
{
    for (int i = 0; i < src.rows; i++)         
    {                  
        uchar*p = dst1.ptr<uchar>(i);
        uchar*q = dst2.ptr<uchar>(i);
        for(int j = 0; j < src.cols; j++)
        {                            
            *p= (15*src.at<cv::Vec3b>(i, j)[0] + 75*src.at<cv::Vec3b>(i, j)[1] + 38*src.at<cv::Vec3b>(i, j)[2]) >> 7;
            *q = (255 - *p);//��ɫ
            p++;
            q++;
        }         
    }
}

/* ***********************�㷨˼��*********************** */
/* ********************************************************
    1.ȥɫ
	2.����ȥɫͼ�㣬���ҷ�ɫ����ɫΪY��i��j�� = 255-X��i��
	3.�Է�ɫͼ����и�˹ģ����
	4.ģ�����ͼ�����ģʽѡ����ɫ����Ч��
********************************************************* */
void sketch(const Mat src, Mat &dst)
{
    printf("sketch\n");
	Mat gray,gray_reverse,gray_end;

    if (!src.data || !dst.data)
        return; 

    gray.create(cv::Size(RGA_ALIGN(src.cols, 16), RGA_ALIGN(src.rows, 16)), CV_8UC1);//����ȥɫͼ��
    gray_reverse.create(cv::Size(RGA_ALIGN(src.cols, 16), RGA_ALIGN(src.rows, 16)), CV_8UC1);//��ɫ
    gray_end.create(cv::Size(RGA_ALIGN(src.cols, 16), RGA_ALIGN(src.rows, 16)), CV_8UC1);

    rgb_to_gray_to_reverse(src,gray,gray_reverse); 
    GaussianBlur(gray_reverse,gray_reverse,Size(11,11),0);//��˹ģ��
    
    for (int i = 0; i < src.rows; i++)         
    {                  
        uchar* p1  = gray.ptr<uchar>(i);       
        uchar* p2  = gray_reverse.ptr<uchar>(i);       
        uchar* q  = gray_end.ptr<uchar>(i);      
        for (int j = 0; j < src.cols; j++)     
        {           
            int tmp1=p1[j];         
            int tmp2=p2[j];         
            q[j] =(uchar) min((tmp1+(tmp1*tmp2)/(256-tmp2)),255); //������ʽ C=MIN(A+ (A*B) / (255-B),255��,
			                                                      //����CΪ��Ͻ����AΪȥɫ������ص㣬BΪ����ģ��������ص�
        }        
    }
    fcv::cvtColor(gray_end,dst,CV_GRAY2BGR);
    //gray.release();
    //gray_reverse.release();
    //gray_end.release();
}
