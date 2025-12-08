//
// Created by James Castiglione on 12/8/25.
//

#pragma once
#include <opencv2/opencv.hpp>

namespace camlib {

    class Webcam {
        cv::VideoCapture capture{};
        public:
        Webcam();
        std::string read_frame();
    };

} // namespace camlib
