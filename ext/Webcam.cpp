//
// Created by James Castiglione on 12/8/25.
//

#include "Webcam.hpp"

namespace camlib {
    Webcam::Webcam() {
        bool result = capture.open(0);
        if (!result) {
            throw std::runtime_error("Error opening webcam 0");
        }
    }
    std::string Webcam::read_frame() {
        cv::Mat frame;
        auto result = capture.read(frame);
        if (!result) {
            throw std::runtime_error("Error reading webcam");
        }
        // Ensure the image data is continuous for direct memory access
        if (!frame.isContinuous()) {
            frame = frame.clone();
        }
        size_t data_size = frame.total() * frame.elemSize();

        // std::string copies the data
        return {reinterpret_cast<char*>(frame.data), data_size};
    }
} // namespace camlib
