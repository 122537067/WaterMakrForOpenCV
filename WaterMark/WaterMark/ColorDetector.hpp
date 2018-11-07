//
//  ColorDetector.hpp
//  WaterMark
//
//  Created by one on 2018/11/2.
//  Copyright © 2018 one. All rights reserved.
//

#ifndef ColorDetector_hpp
#define ColorDetector_hpp
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>


using namespace cv;

class ColorDetector{
public:
    double rate;        //相似颜色比重
private:
    int maxDist;        // 允许的最大颜色差距
    Vec3b target;    // 目标颜色
    
    // 计算两个颜色之间的距离
    int getColorDistance(const Vec3b &color1,
                         const Vec3b &color2) const {
        // 用opencv函数计算欧式距离
        return static_cast<int>(
                                norm<int, 3>(Vec3i(color1[0] - color2[0],
                                                   color1[1] - color2[1], color1[2] - color2[2]))
                                );
    }
    
    // 计算与目标颜色的差距
    int getDistanceToTargetColor(const Vec3b &color) const {
        return getColorDistance(color, target);
    }
    
public:
    // 空构造函数，在此初始化默认参数
    ColorDetector();
    
    // 完整构造函数
    ColorDetector(uchar blue, uchar green, uchar red,int maxDist);
    
    // 操作符重载
    Mat operator()(const Mat &image);
    
    // 从输入图像image中检测与target颜色相近的像素，在结果图中用白色表示
    // 其他颜色像素在结果图中用黑色表示
    Mat process(const Mat &image);
    
    // 设置颜色差距的阈值
    // 阈值必须是正数，否则就置为0
    void setColorDistanceThreshold(int distance);
    
    // 取得颜色差距的阈值
    int getColorDistanceThreshold() const;
    
    // 设置需要检测的颜色
    void setTargetColor(uchar blue, uchar green, uchar red);
    
    // 设置需要检测的颜色
    void setTargetColor(Vec3b color);
    
    // 获取需要检测的颜色
    Vec3b getTargetColor() const;
    
    double getColorRate();
    void setColorRate(double setrate);
};

#endif /* ColorDetector_hpp */
