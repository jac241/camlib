#include "camlib.h"
#include "Webcam.hpp"

#include <rice/rice.hpp>
#include <rice/stl.hpp>

using namespace Rice;

Object test_initialize(Object self)
{
    self.iv_set("@foo", 42);
    return self;
}

Object test_hello(Object /* self */)
{
    String str("hello, world");
    return str.value();
}

extern "C"
void Init_camlib() // Has to match module/gem name?
{
    Module rb_mCamlib = define_module("Camlib");
    Class rb_cTest =
      define_class_under(rb_mCamlib.value(), "Test")
      .define_method("initialize", &test_initialize)
      .define_method("hello", &test_hello);

    Data_Type<camlib::Webcam> rb_cWebcam=
        define_class_under<camlib::Webcam>(rb_mCamlib.value(), "Webcam")
            .define_constructor(Constructor<camlib::Webcam>())
            .define_method("read_frame", &camlib::Webcam::read_frame);
}
