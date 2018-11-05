#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//核心思想：检测强度（0-255）突然变化和模糊子区域，创建了一个虚构的矩形（1x9像素）遍历图像。 
//对于矩形的每一个移动,读取9个垂直像素，并检查强度的突然增量（在这种情况下为30），并用滤波模糊尖刺部分

int main()
{
    Mat image;
    image = imread("clahe.jpg", IMREAD_GRAYSCALE);
	imshow("src",image);
    if (image.data==NULL)
        cout << "Img not found" << endl;
    namedWindow("hand", WINDOW_AUTOSIZE);
    Mat orig = image.clone();
    threshold(image, image, 180, 255, THRESH_BINARY);

    int height = 8; 
    for(int x=0;x<image.cols;x++)  //两个for循环设定1*8 的矩形算子移动的轨迹
        for (int y = height / 2; y < image.rows - height / 2; y++)
        {
            int whiteToBlack = 0, blackToWhite = 0;
            for (int rectY = y - height / 2; rectY < y + height / 2 - 1; rectY++) //这个for 比较矩形算子内部的像素的前后是否有跳变现象
                if (image.at<uchar>(rectY, x) > image.at<uchar>(rectY + 1, x) && image.at<uchar>(rectY, x) - image.at<uchar>(rectY + 1, x) > 30)
                    whiteToBlack++; //统计跳变的类型和个数
                else if (image.at<uchar>(rectY, x) < image.at<uchar>(rectY + 1, x) && image.at<uchar>(rectY + 1, x) - image.at<uchar>(rectY, x) > 30)
                    blackToWhite++;
            if (whiteToBlack > 0 && blackToWhite > 0)
                for (int rectY = y - height / 4; rectY < y + height / 4 - 1; rectY++)
            blur(orig(Rect(x, y - height / 2, 4, height + 1)), orig(Rect(x, y - height / 2, 4, height + 1)),Size(3,3)); //对当前的矩形算子内部进行滤波模糊，从而达到去除背毛的作用
        }

    imshow("hand", orig);
    waitKey(0);
}
