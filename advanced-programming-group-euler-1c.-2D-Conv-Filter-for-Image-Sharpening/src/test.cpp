/**
 * @file SobelFilterTest.cpp
 * @brief Simple test for the SobelFilter class
 * @group [Euler]
 */

#include "SobelFilter.h"
#include "PrewittFilter.h"
#include "RobertsCrossFilter.h"
#include "ScharrFilter.h"
#include <iostream>
#include <string>

int main() {
    // 测试配置
    std::string inputPath = "../Images/gracehopper.png"; // 替换为你的测试图片路径
    std::string outputPath = "../Images/gracehopper_edges.png";
    
    try {
        // 加载输入图像
        std::cout << "Loading image: " << inputPath << std::endl;
        Image inputImage(inputPath);
        std::cout << "Image loaded successfully. Dimensions: " 
                  << inputImage.getWidth() << "x" << inputImage.getHeight() 
                  << ", Channels: " << inputImage.getChannels() << std::endl;
        
        // 创建并应用Sobel边缘检测滤镜
        std::cout << "Applying Sobel edge detection filter..." << std::endl;
        ScharrFilter filter;
        Image outputImage = filter.apply(inputImage);
        
        // 保存结果
        std::cout << "Saving result to: " << outputPath << std::endl;
        if (outputImage.saveToFile(outputPath)) {
            std::cout << "Edge detection result saved successfully!" << std::endl;
        } else {
            std::cout << "Failed to save the edge detection result." << std::endl;
            return 1;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}