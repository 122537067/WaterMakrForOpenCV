//
//  ColorDetector.cpp
//  WaterMark
//
//  Created by one on 2018/11/2.
//  Copyright © 2018 one. All rights reserved.
//

#include "ColorDetector.hpp"
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

// 空构造函数，在此初始化默认参数
ColorDetector::ColorDetector() : maxDist(100), target(0, 0, 0) {}

// 完整构造函数
ColorDetector::ColorDetector(uchar blue, uchar green, uchar red,int maxDist)
{
    // 设置目标颜色
    maxDist = 100;
    setTargetColor(blue, green, red);
}

// 操作符重载
Mat ColorDetector::operator()(const Mat &image) {
    return process(image);
}

// 从输入图像image中检测与target颜色相近的像素，在结果图中用白色表示
// 其他颜色像素在结果图中用黑色表示
Mat ColorDetector::process(const Mat &image) {
    Mat result;
    int allColor=0;   //总颜色量
    int likeColor=0;  //接近颜色
    // 重新分配二值结果图像
    // 与输入图像的尺寸相同，不过是单通道
    result.create(image.size(), CV_8U);
    
    // 遍历图像，处理每个像素
    for (int j = 0; j < image.rows; j++) {
        // 取得行j的首地址
        const Vec3b *input = image.ptr<Vec3b>(j);
        uchar *output = result.ptr<uchar>(j);
        // 遍历该行的每一个像素
        for (int i = 0; i < image.cols; i++) {
            allColor++;
            // 比较与目标颜色的差距
            if (getDistanceToTargetColor(input[i]) <= maxDist)
            {
                //颜色接近
                output[i] = 255;
                likeColor++;
            }
            else
            output[i] = 0;
        }
    }
    rate = (double)likeColor/(double)allColor;
    //cout<<"rate:"<<rate<<endl;
    return result;
}

// 设置颜色差距的阈值
// 阈值必须是正数，否则就置为0
void ColorDetector::setColorDistanceThreshold(int distance) {
    if (distance < 0)
        distance = 0;
    maxDist = distance;
}

// 取得颜色差距的阈值
int ColorDetector::getColorDistanceThreshold() const {
    return maxDist;
}

// 设置需要检测的颜色
void ColorDetector::setTargetColor(uchar blue, uchar green, uchar red) {
    // 次序为BGR
    target = Vec3b(blue, green, red);
}

// 设置需要检测的颜色
void ColorDetector::setTargetColor(Vec3b color) {
    target = color;
}

// 获取需要检测的颜色
Vec3b ColorDetector::getTargetColor() const {
    return target;
}

double ColorDetector::getColorRate(){
    return rate;
}
void ColorDetector::setColorRate(double setrate){
    rate = setrate;
}



