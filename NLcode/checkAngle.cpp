#include"st.h"
using namespace std;
using namespace cv;

float getCardAngle(Mat srcImg) {
    resize(srcImg, srcImg, Size(srcImg.cols / 7, srcImg.rows / 7), (0, 0), (0, 0));
    Mat dstImg = srcImg.clone();
    cvtColor(srcImg, srcImg, CV_BGR2GRAY);
    threshold(srcImg, srcImg, 100, 255, CV_THRESH_BINARY); //��ֵ��
    /*imshow("threshold", srcImg);*/

    vector<vector<Point>> contours;
    vector<Vec4i> hierarcy;
    findContours(srcImg, contours, hierarcy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    vector<Rect> boundRect(contours.size());  //������Ӿ��μ���
    vector<RotatedRect> box(contours.size()); //������С��Ӿ��μ���
    Point2f rect[4];
    vector<Point2f> minR;
    float max = 0;
    float angel = 0;
    for (int i = 0; i < contours.size(); i++)
    {
        box[i] = minAreaRect(Mat(contours[i]));  //����ÿ��������С��Ӿ���
        boundRect[i] = boundingRect(Mat(contours[i]));
        //circle(dstImg, Point(box[i].center.x, box[i].center.y), 5, Scalar(0, 255, 0), -1, 8);  //������С��Ӿ��ε����ĵ�
        //box[i].points(rect);  //����С��Ӿ����ĸ��˵㸴�Ƹ�rect����
        //rectangle(dstImg, Point(boundRect[i].x, boundRect[i].y), Point(boundRect[i].x + boundRect[i].width, boundRect[i].y + boundRect[i].height), Scalar(0, 255, 0), 2, 8);
        //for (int j = 0; j < 4; j++)
        //{
        //    line(dstImg, rect[j], rect[(j + 1) % 4], Scalar(0, 0, 255), 2, 8);  //������С��Ӿ���ÿ����
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
    Canny(srcImage, midImage, 50, 200, 3);//����һ��canny��Ե���
    cvtColor(midImage, dstImage, CV_GRAY2BGR);//ת����Ե�����ͼΪ�Ҷ�ͼ
    vector<Vec4i> lines;//����һ��ʸ���ṹlines���ڴ�ŵõ����߶�ʸ������
    HoughLinesP(midImage, lines, 1, CV_PI / 180, 80, 50, 10);
    vector<int> t;
    for (size_t i = 0; i < lines.size(); i++)
    {
        Vec4i l = lines[i];
        line(dstImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186, 88, 255), 1, CV_AA);
        /*float k = (l[1] - l[3])/(l[0] - l[2]);*/
        double angle = atan2(l[3] - l[1], l[2] - l[0]) / 3.141592*180.0;
        int aint = int(angle);
        //cout << "�Ƕȣ�"<<angle<<"---"<<aint << endl;

        t.push_back(aint);
        
    }
    
    
    //imshow("��ԭʼͼ��", srcImage);

    ////��6����Ե�����ͼ 
    //imshow("����Ե�����ͼ��", midImage);

    ////��7����ʾЧ��ͼ  

    //imshow("��Ч��ͼ��", dstImage);
    //waitKey(0);

    return float(getMode(t));

}

void rotate_arbitrarily_angle(Mat& src, Mat& dst, float angle)
{
    float radian = (float)(angle / 180.0 * CV_PI);

    //���ͼ��
    int maxBorder = (int)(max(src.cols, src.rows) * 1.414); //��Ϊsqrt(2)*max
    int dx = (maxBorder - src.cols) / 2;
    int dy = (maxBorder - src.rows) / 2;
    copyMakeBorder(src, dst, dy, dy, dx, dx, BORDER_CONSTANT);

    //��ת
    Point2f center((float)(dst.cols / 2), (float)(dst.rows / 2));
    Mat affine_matrix = getRotationMatrix2D(center, angle, 1.0);//�����ת����
    warpAffine(dst, dst, affine_matrix, dst.size());

    //����ͼ����ת֮�����ͼ������ľ���
    float sinVal = abs(sin(radian));
    float cosVal = abs(cos(radian));
    Size targetSize((int)(src.cols * cosVal + src.rows * sinVal),
        (int)(src.cols * sinVal + src.rows * cosVal));

    //��������߿�
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
    //    cout << "��ת�Ƕȣ�" << angle << endl;
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
        cout << "��ת�Ƕȣ�" << angle << endl;
        clock_t endTime = clock();
        cout << "The run time is:" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
        resize(src, src, Size(src.cols / 3, src.rows / 3), (0, 0), (0, 0));
        resize(des, des, Size(des.cols / 3, des.rows / 3), (0, 0), (0, 0));
        imshow("src", src);
        imshow("des", des);
        waitKey(0);
    }
}