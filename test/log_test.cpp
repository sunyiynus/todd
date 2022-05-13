#include "log.h"


#include <catch2/catch_test_macros.hpp>


TEST_CASE("test logoutput", "[TEST class Logger]")
{
    int tmp = 42;
    todd::Logger::logger().setLogLevel(todd::LogLevel::WARN);
    todd_TRACE("this is test code for %d \n", tmp);
    todd_DEBUG("this is test code for %d \n", tmp);
    todd_INFO("this is test code for %d \n", tmp);
    todd_WARN("this is test code for %d \n", tmp);
    todd_FATAL("this is test code for %d \n", tmp);
    todd_TRACE("this is test code for %d \n", tmp);
    todd_TRACE("this is test code for %d \n", tmp);
    todd_TRACE("this is test code for %d \n", tmp);
    todd_TRACE("this is test code for %d \n", tmp);
}
