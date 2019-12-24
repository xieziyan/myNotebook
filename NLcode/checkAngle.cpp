#include"st.h"
using namespace std;
using namespace cv;

float getCardAngle(Mat srcImg) {
    resize(srcImg, srcImg, Size(srcImg.cols / 7, srcImg.rows / 7), (0, 0), (0, 0));
    Mat dstImg = srcImg.clone();
    cvtColor(srcImg, srcImg, CV_BGR2GRAY);
    threshold(srcImg, srcImg, 100, 255, CV_THRESH_BINARY); //二值化
    /*imshow("threshold", srcImg);*/

    vector<vector<Point>> contours;
    vector<Vec4i> hierarcy;
    findContours(srcImg, contours, hierarcy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    vector<Rect> boundRect(contours.size());  //定义外接矩形集合
    vector<RotatedRect> box(contours.size()); //定义最小外接矩形集合
    Point2f rect[4];
    vector<Point2f> minR;
    float max = 0;
    float angel = 0;
    for (int i = 0; i < contours.size(); i++)
    {
        box[i] = minAreaRect(Mat(contours[i]));  //计算每个轮廓最小外接矩形
        boundRect[i] = boundingRect(Mat(contours[i]));
        //circle(dstImg, Point(box[i].center.x, box[i].center.y), 5, Scalar(0, 255, 0), -1, 8);  //绘制最小外接矩形的中心点
        //box[i].points(rect);  //把最小外接矩形四个端点复制给rect数组
        //rectangle(dstImg, Point(boundRect[i].x, boundRect[i].y), Point(boundRect[i].x + boundRect[i].width, boundRect[i].y + boundRect[i].height), Scalar(0, 255, 0), 2, 8);
        //for (int j = 0; j < 4; j++)
        //{
        //    line(dstImg, rect[j], rect[(j + 1) % 4], Scalar(0, 0, 255), 2, 8);  //绘制最小外接矩形每条边
        //}
        float s = box[i].size.height * box[i].size.width;
      
        if (s > max) {

            box[i].points(rect);
            max = s;
            if (box[i].size.height<box[i].size.width) {
                angel = box[i].angle;
            }
            else {
                angel = box[i].angle-90;
            }
        }
    }
    //for (int j = 0; j < 4; j++) {
    //    line(dstImg, rect[j], rect[(j + 1) % 4], Scalar(0, 0, 255), 2, 8);
    //}
    //imshow("dst", dstImg);
    //waitKey(0);
    return angel;
}
int getMode(vector<int> &nums) {
    sort(nums.begin(), nums.end());
    return nums[int(nums.size()/2-1)];
}



float getAngle(Mat src)
{
    Mat srcImage;
    resize(src, srcImage, Size(src.cols / 7, src.rows / 7), (0, 0), (0, 0));
    Mat midImage, dstImage;
    Canny(srcImage, midImage, 50, 200, 3);
    Canny(srcImage, midImage, 50, 200, 3);//进行一此canny边缘检测
    cvtColor(midImage, dstImage, CV_GRAY2BGR);//转化边缘检测后的图为灰度图
    vector<Vec4i> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
    HoughLinesP(midImage, lines, 1, CV_PI / 180, 80, 50, 10);
    vector<int> t;
    for (size_t i = 0; i < lines.size(); i++)
    {
        Vec4i l = lines[i];
        line(dstImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186, 88, 255), 1, CV_AA);
        /*float k = (l[1] - l[3])/(l[0] - l[2]);*/
        double angle = atan2(l[3] - l[1], l[2] - l[0]) / 3.141592*180.0;
        int aint = int(angle);
        //cout << "角度："<<angle<<"---"<<aint << endl;

        t.push_back(aint);
        
    }
    
    
    //imshow("【原始图】", srcImage);

    ////【6】边缘检测后的图 
    //imshow("【边缘检测后的图】", midImage);

    ////【7】显示效果图  

    //imshow("【效果图】", dstImage);
    //waitKey(0);

    return float(getMode(t));

}

void rotate_arbitrarily_angle(Mat& src, Mat& dst, float angle)
{
    float radian = (float)(angle / 180.0 * CV_PI);

    //填充图像
    int maxBorder = (int)(max(src.cols, src.rows) * 1.414); //即为sqrt(2)*max
    int dx = (maxBorder - src.cols) / 2;
    int dy = (maxBorder - src.rows) / 2;
    copyMakeBorder(src, dst, dy, dy, dx, dx, BORDER_CONSTANT);

    //旋转
    Point2f center((float)(dst.cols / 2), (float)(dst.rows / 2));
    Mat affine_matrix = getRotationMatrix2D(center, angle, 1.0);//求得旋转矩阵
    warpAffine(dst, dst, affine_matrix, dst.size());

    //计算图像旋转之后包含图像的最大的矩形
    float sinVal = abs(sin(radian));
    float cosVal = abs(cos(radian));
    Size targetSize((int)(src.cols * cosVal + src.rows * sinVal),
        (int)(src.cols * sinVal + src.rows * cosVal));

    //剪掉多余边框
    int x = (dst.cols - targetSize.width) / 2;
    int y = (dst.rows - targetSize.height) / 2;
    Rect rect(x, y, targetSize.width, targetSize.height);
    dst = Mat(dst, rect);
}
Mat getBox(Mat& src,Point a,Point b){
    Rect m_select;
    m_select = Rect(a.x, a.y,b.x , b.y);
    Mat ROI = src(m_select);
    return ROI;
}
int main() {
    //for(int i=19; i < 25; i++) {
    //    string path = "./test/pa/";
    //    string center = to_string(i);
    //    path.append(center);
    //    path.append(".bmp");
    //    Mat src = imread(path);
    //    clock_t startTime = clock();
    //    Mat des;
    //    //resize(src, src, Size(src.cols / 3, src.rows / 3), (0, 0), (0, 0));
    //    float angle = getAngle(src);
    //    rotate_arbitrarily_angle(src, des, angle);
    //    cout << "旋转角度：" << angle << endl;
    //    clock_t endTime = clock();
    //    cout << "The run time is:" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
    //    resize(src, src, Size(src.cols / 3, src.rows / 3), (0, 0), (0, 0));
    //    resize(des, des, Size(des.cols / 3, des.rows / 3), (0, 0), (0, 0));
    //    imshow("src", src);
    //    imshow("des", des);
    //    waitKey(0);
    //}

    //string path = "image37_raw.bmp";
    //Mat re = imread(path);
    //clock_t startTime = clock();
    //Mat src = getBox(re,Point(0, 316),Point(1280, 192));
    ////Mat des;
    ////float angle = getAngle(src);
    ////cout << angle << endl;
    ////rotate_arbitrarily_angle(src, des, angle);
    //clock_t endTime = clock();
    //cout << "The run time is:" << (double)(endTime - startTime) / CLK_TCK << "s" << endl;
    for(int i=11; i < 19; i++) {
        string path = "./test/card/";
        string center = to_string(i);
        path.append(center);
        path.append(".bmp");
        Mat src = imread(path);
        clock_t startTime = clock();
        Mat des;
        //resize(src, src, Size(src.cols / 3, src.rows / 3), (0, 0), (0, 0));
        float angle = getCardAngle(src);
        rotate_arbitrarily_angle(src, des, angle);
        cout << "旋转角度：" << angle << endl;
        clock_t endTime = clock();
        cout << "The run time is:" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
        resize(src, src, Size(src.cols / 3, src.rows / 3), (0, 0), (0, 0));
        resize(des, des, Size(des.cols / 3, des.rows / 3), (0, 0), (0, 0));
        imshow("src", src);
        imshow("des", des);
        waitKey(0);
    }
}