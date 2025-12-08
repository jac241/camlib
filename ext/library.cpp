#include "library.h"

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
    return str;
}

extern "C"
void Init_test()
{
    Class rb_cTest =
      define_class("Test")
      .define_method("initialize", &test_initialize)
      .define_method("hello", &test_hello);
}
