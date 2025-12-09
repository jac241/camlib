#include "camlib.h"
#include "Webcam.hpp"

#include <rice/rice.hpp>
#include <rice/stl.hpp>

using namespace Rice;

extern "C" void Init_camlib() // Has to match module/gem name?
{
    Module rb_mCamlib = define_module("Camlib");

    Data_Type<camlib::Webcam> rb_cWebcam =
            define_class_under<camlib::Webcam>(rb_mCamlib.value(), "Webcam")
                    .define_singleton_function(
                            "acquire!", &camlib::Webcam::acquire,
                            Arg("device_id") = 0, Function().setNoGvl())
                    .define_method("read_frame", &camlib::Webcam::read_frame,
                                   Function().setNoGvl())
                    .define_method("frame_width", &camlib::Webcam::frame_width)
                    .define_method("frame_height",
                                   &camlib::Webcam::frame_height)
                    .define_method("frame_depth", &camlib::Webcam::frame_depth)
                    .define_method("frame_channel_count",
                                   &camlib::Webcam::frame_channel_count)
                    .define_method("frame_rate", &camlib::Webcam::frame_rate);
}
