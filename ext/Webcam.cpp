//
// Created by James Castiglione on 12/8/25.
//

#include "Webcam.hpp"

namespace camlib {
    Webcam::Webcam(cv::VideoCapture &&cam) : capture(cam) {}

    Webcam Webcam::acquire(int device_id = 0) {
        cv::VideoCapture cam;
        bool result = cam.open(device_id);
        if (!result) {
            throw std::runtime_error(
                    std::format("Error opening webcam ID {}", device_id));
        }

        return Webcam(std::move(cam));
    }
    bool Webcam::is_device_functioning(int device_id) {
        try {
            auto cam = acquire(device_id);
            const auto result = cam.read_frame();
            return !(result.empty());
        } catch (const std::runtime_error &e) {
            return false;
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
        return {reinterpret_cast<char *>(frame.data), data_size};
    }
    int Webcam::frame_width() const {
        return static_cast<int>(capture.get(cv::CAP_PROP_FRAME_WIDTH));
    }

    int Webcam::frame_height() const {
        return static_cast<int>(capture.get(cv::CAP_PROP_FRAME_HEIGHT));
    }

    int Webcam::frame_depth() const {
        const int format = static_cast<int>(capture.get(cv::CAP_PROP_FORMAT));
        return CV_MAT_DEPTH(format);
    }
    int Webcam::frame_channel_count() const {
        const int format = static_cast<int>(capture.get(cv::CAP_PROP_FORMAT));
        return CV_MAT_CN(format);
    }
    double Webcam::frame_rate() const { return capture.get(cv::CAP_PROP_FPS); }
    void Webcam::release() { capture.release(); }
} // namespace camlib
