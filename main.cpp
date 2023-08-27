#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include "Colourize.h"

Color averageColors(const std::vector<uint8_t>& bytes) {
    int r = 0, g = 0, b = 0;
    int count = bytes.size();
    if (count == 0) {
        return {{0, 0, 0}};
    }

    for (const auto& byte : bytes) {
        Color color = Colourize::getColour(byte);
        r += color.rgb[0];
        g += color.rgb[1];
        b += color.rgb[2];
    }

    r /= count;
    g /= count;
    b /= count;

    return {{static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b)}};
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        std::cout << "Usage: ./main <binary file path>" << std::endl;
        return -1;
    }

    std::ifstream file(argv[1], std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cout << "Could not read file: " << argv[1] << std::endl;
        return -1;
    }

    std::streamsize size = file.tellg();
    if (size <= 0) {
        std::cerr << "Error: File size is not valid." << std::endl;
        return -1;
    }

    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(size);
    file.read(reinterpret_cast<char*>(buffer.data()), size);

    int width = std::sqrt(size);
    int height = (size + width - 1) / width;
    const int max_dim = 4096;
    int block_size = 1;

    while (width / block_size > max_dim || height / block_size > max_dim) {
        block_size++;
    }

    int reduced_width = width / block_size;
    int reduced_height = height / block_size;
    cv::Mat image(reduced_height, reduced_width, CV_8UC3, cv::Scalar(0, 0, 0));

    for (int y = 0; y < reduced_height; ++y) {
        for (int x = 0; x < reduced_width; ++x) {
            std::vector<uint8_t> block_bytes;
            block_bytes.reserve(block_size * block_size);

            for (int j = 0; j < block_size; ++j) {
                for (int i = 0; i < block_size; ++i) {
                    int idx = (y * block_size + j) * width + (x * block_size + i);
                    if (idx < size) {
                        block_bytes.push_back(buffer[idx]);
                    }
                }
            }

            Color avg_color = averageColors(block_bytes);
            image.at<cv::Vec3b>(y, x) = cv::Vec3b(avg_color.rgb[2], avg_color.rgb[1], avg_color.rgb[0]);
        }
    }

    std::filesystem::path inputPath(argv[1]);
    std::string outputFilename = inputPath.filename().string() + ".png";
    cv::imwrite(outputFilename, image);
    std::cout << "Output: " << outputFilename << std::endl;

    return 0;
}