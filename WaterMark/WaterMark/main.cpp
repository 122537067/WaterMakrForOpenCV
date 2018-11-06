//
//  main.cpp
//  WaterMark
//
//  Created by one on 2018/10/29.
//  Copyright © 2018 one. All rights reserved.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <dirent.h>
#include<iomanip>
#include "photoDate.hpp"
#include "ColorDetector.hpp"

using namespace cv;
using namespace std;

string photoList[100];  //图片名字加入数组
int photoCount=0;  //图片个数
photoDate myDate; //定义一个日期类
Mat dateMat;        //日期区域

//获取字符串后面的n个字符
//str -- 字符串
//lastN -- 最后几个字符
inline string getStringLastNChar(string str, int lastN)
{
    return str.substr(str.size() - lastN);
}

//获取目录下的所有图片（.png .jpg 格式文件）
void getPhoto()
{
    string lastFour; //文件最后四位字符（则文件格式
    string folderName;  //读取到的文件名
    struct dirent *dirp;
    DIR* dir = opendir("./");
    while ((dirp = readdir(dir)) != nullptr)
    {
        if (dirp->d_type == DT_REG)
        {
            // 文件
            printf("%s\n", dirp->d_name);
            folderName = (string)dirp->d_name; //更新文件名
            lastFour = folderName.substr(folderName.size() - 4); //获取文件格式
            if(lastFour == ".jpg" || lastFour==".png")
            {
                photoList[photoCount] = folderName;
                photoCount++;
            }
        } else if (dirp->d_type == DT_DIR) {
            // 文件夹
        }
    }
    closedir(dir);
}

//根据图片大小(按行或列的最大值判断)定义日期位置和大小
void putDate(Mat image)
{
    //按行或列的最大值判断
    int max = image.cols;
    if(max < image.rows)
    {
        max = image.rows;
    }
    
    //优化算法--v1.1
    //字体大小一个字符长度大约24*fontScale,宽度大约36*fontScale（opencv文档没有资料，自己测出来的，只能说大约）
    //计算不同大小图片的puttext里面参数的算法，基于一个字符的大小=24*size*24*size像素
    int date_size = log2(max/1000) + 1;
    myDate.setDateSize(date_size);
    myDate.setDateBold(log(pow(date_size,5)) + 2*date_size);    //有点像生长曲线函数，简化了一下
    myDate.setDateCols(image.cols - 240 * date_size);
    myDate.setDateRows(image.rows - 10);
    //获取日期区域
    dateMat = image(Rect(image.cols - 240 * date_size,image.rows - (36*date_size +10),date_size * 240,36*date_size+10));
    
    
    //简单算数--v1.0
    //最大像素小于2000
//    if(max < 2000)
//    {
//        myDate.setDateSize(1);
//        myDate.setDateBold(2);
//        myDate.setDateCols(image.cols - 240);
//        myDate.setDateRows(image.rows - 10);
//        dateMat = image(Rect(image.cols - 240,image.rows - 10,240,10));
//    }
//    else if(max < 3900)
//    {
//        myDate.setDateSize(2);
//        myDate.setDateBold(8);
//        myDate.setDateCols(image.cols - 480);
//        myDate.setDateRows(image.rows - 10);
//        dateMat = image(Rect(image.cols - 480,image.rows - 10,480,10));
//    }
//    else if(max < 8000)
//    {
//        myDate.setDateSize(3);
//        myDate.setDateBold(10);
//        myDate.setDateCols(image.cols - 720);
//        myDate.setDateRows(image.rows - 10);
//        dateMat = image(Rect(image.cols - 720,image.rows - 10,720,10));
//    }
//    else
//    {
//        myDate.setDateSize(8);
//        myDate.setDateBold(12);
//        myDate.setDateCols(image.cols - 1200);
//        myDate.setDateRows(image.rows - 10);
//        dateMat = image(Rect(image.cols - 1200,image.rows - 10,1200,10));
//    }
}

void changeTextColor(double rate)
{
    //当字体颜色/背景颜色 比重 大于 30%时
    //将字体设为白色
    if(rate > 0.3)
    {
        myDate.colorB = 255;
        myDate.colorG = 255;
        myDate.colorR = 255;
    }
}

int main()
{
    getPhoto();
    cout<<"--------"<<endl;
    
    for(int i=0;i<photoCount;i++)
    {
        string photoName = photoList[i];                           //获取图片名
        string photoDate = photoName.substr(0,10);                 //取图片名的前10个字符串获取日期  (则日期格式为:yyyy-mm-dd)
        Mat image = imread(photoName, CV_LOAD_IMAGE_UNCHANGED);    //找到图片（相对路径需要放在跟exe文件同一目录）
        
        //如果图片为空
        if(image.empty())
        {
            cout<<"打开图片"<<photoName<<"失败"<<endl;
            exit(EXIT_FAILURE);  //退出程序
        }
        //初始化字体颜色rgb值（橙色）
        myDate.colorB = 18;
        myDate.colorG = 153;
        myDate.colorR = 255;
        
        putDate(image);                                            //设置放置日期的大小和位置
        
        
        // 1. 创建图像处理器对象
        ColorDetector cdetect(0, 0, 0,0);

        // 3. 设置输入参数
        cdetect.setTargetColor(myDate.colorB, myDate.colorG, myDate.colorR);    // 这里表示需要寻找相似的颜色
        cdetect.setColorDistanceThreshold(50);  //阙值（则相似范围）

        // 4. 处理图像并显示结果
        imshow("Result", cdetect(dateMat));

        //测试日期区域是否正确
        imshow("resultDate", dateMat);

        //根据颜色比重改变字体颜色
        cout<<"RATE:"<<cdetect.getColorRate()<<endl;
        changeTextColor(cdetect.getColorRate());
        
        putText(image,                                             //处理的图片
                photoDate,                                         //放置的文本（日期）
                Point(myDate.getDateCols(),myDate.getDateRows()),  //文本的位置
                FONT_HERSHEY_SIMPLEX,                              //字体格式
                myDate.getDateSize(),                              //字体大小
                Scalar(myDate.colorB, myDate.colorG, myDate.colorR),//字体颜色
                myDate.getDateBold());                             //字体粗细
        
        //waitKey(0);
        
        imwrite("new" + photoName, image);                         //导出图片
        cout<<"生成"<<i+1<<"张图片成功"<<endl;
    }
    
    return 0;
}
