//
// Created by James Castiglione on 12/8/25.
//

#pragma once
#include <opencv2/opencv.hpp>

namespace camlib {

    class Webcam {
        cv::VideoCapture capture{};
        explicit Webcam(cv::VideoCapture &&cam);

    public:
        static Webcam acquire(int device_id);

        std::string read_frame();
        [[nodiscard]] int frame_width() const;
        [[nodiscard]] int frame_height() const;
        [[nodiscard]] int frame_depth() const;
        [[nodiscard]] int frame_channel_count() const;
        [[nodiscard]] double frame_rate() const;
    };

} // namespace camlib
