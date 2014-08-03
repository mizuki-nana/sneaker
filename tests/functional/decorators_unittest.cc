#include <stdexcept>
#include "../../include/testing/testing.h"
#include "../../include/functional/decorators.h"


using namespace sneaker::functional;


class decorators_unittest : public ::testing::Test {};


class retry_decorator_unittest : public decorators_unittest {
public:
  virtual void SetUp() {
    retry_decorator_unittest::counter = 0;
  }

  static int counter;
};
int retry_decorator_unittest::counter = 0;


TEST_F(retry_decorator_unittest, TestRetrySuccessful)
{
  retry<void> wrapper = []() -> void {
    if(retry_decorator_unittest::counter == 4) {
      throw std::runtime_error("");
    }
    ++retry_decorator_unittest::counter;
  };

  wrapper.operator()<std::runtime_error, 5>();

  //ASSERT_EQ(4, counter);
}
