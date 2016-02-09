/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2016 Yanzheng Li

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************************/

/* Unit tests for definitions defined in sneaker/functional/function.h */

#include "../../include/functional/function.h"

#include "../../include/testing/testing.h"

#include <functional>
#include <stdio.h>


// -----------------------------------------------------------------------------

using namespace sneaker::functional;

// -----------------------------------------------------------------------------

class function_unittest_base : public ::testing::Test {};

// -----------------------------------------------------------------------------

class function_as_call_unittest : public function_unittest_base {
public:
  static int expected_result;
  static int actual_result;

  static void func() {
    function_as_call_unittest::actual_result = function_as_call_unittest::expected_result;
  }

  void test_call() {
    ASSERT_EQ(
      function_as_call_unittest::expected_result,
      function_as_call_unittest::actual_result
    );
  }

protected:
  virtual void TearDown() {
    function_as_call_unittest::actual_result = 0;
  }
};

// -----------------------------------------------------------------------------

int function_as_call_unittest::expected_result = 25257758;

// -----------------------------------------------------------------------------

int function_as_call_unittest::actual_result = 0;

// -----------------------------------------------------------------------------

TEST_F(function_as_call_unittest, TestInitializationFromLambdaAndCall)
{
  function<void> func(
    []() -> void {
      return function_as_call_unittest::func();
    }
  );

  func();

  test_call();
}

// -----------------------------------------------------------------------------

TEST_F(function_as_call_unittest, TestInitializationFromLambdaAndCall2)
{
  function<void> func = []() -> void {
    return function_as_call_unittest::func();
  };

  func();

  test_call();
}

// -----------------------------------------------------------------------------

TEST_F(function_as_call_unittest, TestInitializationFromStaticFunctionAndCall)
{
  function<void> func = function_as_call_unittest::func;

  func();

  test_call();
}

// -----------------------------------------------------------------------------

TEST_F(function_as_call_unittest, TestExplicitConversionFromLambdaToStdFunction)
{
  function<void> func(
    []() -> void {
      return function_as_call_unittest::func();
    }
  );

  std::function<void()> f = func;

  f();

  test_call();
}

// -----------------------------------------------------------------------------

TEST_F(function_as_call_unittest, TestExplicitConvertionFromStaticFunctionToStdFunction)
{
  function<void> func = function_as_call_unittest::func;

  std::function<void()> f = func;

  f();

  test_call();
}

// -----------------------------------------------------------------------------

TEST_F(function_as_call_unittest, TestConversionFromStaticFunctionToFunctionPointer)
{
  function<void> func = function_as_call_unittest::func;

  void(*f)(void) = func;

  f();

  test_call();
}

// -----------------------------------------------------------------------------

TEST_F(function_as_call_unittest, TestConversionFromLambdaToFunctionPointer)
{
  function<void> func(
    []() -> void {
      return function_as_call_unittest::func();
    }
  );

  void(*f)(void) = func;

  f();

  test_call();
}

// -----------------------------------------------------------------------------

TEST_F(function_as_call_unittest, TestCopyConstructor)
{
  function<void> func = function_as_call_unittest::func;

  function<void> copied = func;

  copied();

  test_call();
}

TEST_F(function_as_call_unittest, TestInvokeAsync)
{
  function<void> func = function_as_call_unittest::func;

  func.invoke_async();

  sleep(1);

  test_call();
}

// -----------------------------------------------------------------------------

class function_as_action_unittest : public function_unittest_base {
public:
  static void func(const char* name, const int num, char* dst) {
    sprintf(dst, "My name is %s and my favorite number is %d.", name, num);
  }

  void test_call(char* dst) {
    ASSERT_STREQ(
      "My name is Jimmy and my favorite number is 8.",
      dst
    );
  }

protected:
  virtual void SetUp() {
    m_dst = (char*)malloc(50);
  }

  virtual void TearDown() {
    free(m_dst);
    m_dst = NULL;
  }

  const char * m_name = "Jimmy";
  const int m_num = 8;
  char * m_dst = NULL;
};

// -----------------------------------------------------------------------------

TEST_F(function_as_action_unittest, TestInitializationFromLambdaAndCall)
{
  function<void, const char*, const int, char*> func(
    [](const char* name, const int num, char*dst) -> void {
      return function_as_action_unittest::func(name, num, dst);
    }
  );

  func(m_name, m_num, m_dst);

  test_call(m_dst);
}

TEST_F(function_as_action_unittest, TestInitializationFromLambdaAndCall2)
{
  function<void, const char*, const int, char*> func = 
    [](const char* name, const int num, char*dst) -> void
  {
    return function_as_action_unittest::func(name, num, dst);
  };

  func(m_name, m_num, m_dst);

  test_call(m_dst);
}

// -----------------------------------------------------------------------------

TEST_F(function_as_action_unittest, TestInitializationFromStaticFunctionAndCall)
{
  function<void, const char*, const int, char*> func = function_as_action_unittest::func;

  func(m_name, m_num, m_dst);

  test_call(m_dst);
}

// -----------------------------------------------------------------------------

TEST_F(function_as_action_unittest, TestExplicitConversionFromLambdaToStdFunction)
{
  function<void, const char*, const int, char*> func(
    [](const char* name, const int num, char* dst) -> void {
      return function_as_action_unittest::func(name, num, dst);
    }
  );

  std::function<void(const char*, const int, char*)> f = func;

  f(m_name, m_num, m_dst);

  test_call(m_dst);
}

// -----------------------------------------------------------------------------

TEST_F(function_as_action_unittest, TestExplicitConvertionFromStaticFunctionToStdFunction)
{
  function<void, const char*, const int, char*> func = function_as_action_unittest::func;

  std::function<void(const char*, const int, char*)> f = func;

  f(m_name, m_num, m_dst);

  test_call(m_dst);
}

// -----------------------------------------------------------------------------

TEST_F(function_as_action_unittest, TestConversionFromStaticFunctionToFunctionPointer)
{
  function<void, const char*, const int, char*> func = function_as_action_unittest::func;

  void(*f)(const char*, const int, char*) = func;

  f(m_name, m_num, m_dst);

  test_call(m_dst);
}

// -----------------------------------------------------------------------------

TEST_F(function_as_action_unittest, TestConversionFromLambdaToFunctionPointer)
{
  function<void, const char*, const int, char*> func(
    [](const char* name, const int num, char* dst) -> void {
      return function_as_action_unittest::func(name, num, dst);
    }
  );

  void(*f)(const char*, const int, char*) = func;

  f(m_name, m_num, m_dst);

  test_call(m_dst);
}

// -----------------------------------------------------------------------------

TEST_F(function_as_action_unittest, TestCopyConstructor)
{
  function<void, const char*, const int, char*> func = function_as_action_unittest::func;

  function<void, const char*, const int, char*> copied = func;

  copied(m_name, m_num, m_dst);

  test_call(m_dst);
}

// -----------------------------------------------------------------------------

TEST_F(function_as_action_unittest, TestInvokeAsync)
{
  function<void, const char*, const int, char*> func = function_as_action_unittest::func;

  func.invoke_async(m_name, m_num, m_dst);

  sleep(1);

  test_call(m_dst);
}

// -----------------------------------------------------------------------------

class function_as_function_unittest : public function_unittest_base {
public:
  static int func(const int a, const int b) {
    return a + b;
  }

  void test_call(int actual_result) {
    ASSERT_EQ(expected_result, actual_result);
  }

protected:
  const int a = 8;
  const int b = 100;
  const int expected_result = 108;
};

// -----------------------------------------------------------------------------

TEST_F(function_as_function_unittest, TestInitializationFromLambdaAndCall)
{
  function<int, const int, const int> func(
    [](const int a, const int b) -> int {
      return function_as_function_unittest::func(a, b);
    }
  );

  int actual_result = func(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(function_as_function_unittest, TestInitializationFromLambdaAndCall2)
{
  function<int, const int, const int> func =
    [](const int a, const int b) -> int
  {
    return function_as_function_unittest::func(a, b);
  };

  int actual_result = func(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(function_as_function_unittest, TestInitializationFromStaticFunctionAndCall)
{
  function<int, const int, const int> func = function_as_function_unittest::func;

  int actual_result = func(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(function_as_function_unittest, TestExplicitConversionFromLambdaToStdFunction)
{
  function<int, const int, const int> func(
    [](const int a, const int b) -> int {
      return function_as_function_unittest::func(a, b);
    }
  );

  std::function<int(const int, const int)> f = func;

  int actual_result = f(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(function_as_function_unittest, TestExplicitConvertionFromStaticFunctionToStdFunction)
{
  function<int, const int, const int> func = function_as_function_unittest::func;

  std::function<int(const int, const int)> f = func;

  int actual_result = f(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(function_as_function_unittest, TestConversionFromStaticFunctionToFunctionPointer)
{
  function<int, const int, const int> func = function_as_function_unittest::func;

  int(*f)(const int, const int) = func;

  int actual_result = f(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(function_as_function_unittest, TestConversionFromLambdaToFunctionPointer)
{
  function<int, const int, const int> func(
    [](const int a, const int b) -> int {
      return function_as_function_unittest::func(a, b);
    }
  );

  int(*f)(const int, const int) = func;

  int actual_result = f(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(function_as_function_unittest, TestCopyConstructor)
{
  function<int, const int, const int> func = function_as_function_unittest::func;

  function<int, const int, const int> copied = func;

  int actual_result = copied(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------

class function_as_predicate_unittest : public function_unittest_base {
public:
  static bool func(int a, int b) {
    return a >= b;
  }

  void test_call(int actual_result) {
    ASSERT_EQ(expected_result, actual_result);
  }

protected:
  int a = 100;
  int b = 8;
  int expected_result = a > b;
};

// -----------------------------------------------------------------------------

TEST_F(function_as_predicate_unittest, TestInitializationFromLambdaAndCall)
{
  function<bool, int, int> func(
    [](int a, int b) -> bool {
      return function_as_predicate_unittest::func(a, b);
    }
  );

  int actual_result = func(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(function_as_predicate_unittest, TestInitializationFromLambdaAndCall2)
{
  function<bool, int, int> func =
    [](int a, int b) -> bool
  {
    return function_as_predicate_unittest::func(a, b);
  };

  int actual_result = func(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(function_as_predicate_unittest, TestInitializationFromStaticFunctionAndCall)
{
  function<bool, int, int> func = function_as_predicate_unittest::func;

  int actual_result = func(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(function_as_predicate_unittest, TestExplicitConversionFromLambdaToStdFunction)
{
  function<bool, int, int> func(
    [](int a, int b) -> bool {
      return function_as_predicate_unittest::func(a, b);
    }
  );

  std::function<bool(int, int)> f = func;

  bool actual_result = f(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(function_as_predicate_unittest, TestExplicitConvertionFromStaticFunctionToStdFunction)
{
  function<bool, int, int> func = function_as_predicate_unittest::func;

  std::function<bool(int, int)> f = func;

  bool actual_result = f(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(function_as_predicate_unittest, TestConversionFromStaticFunctionToFunctionPointer)
{
  function<bool, int, int> func = function_as_predicate_unittest::func;

  bool(*f)(int, int) = func;

  bool actual_result = f(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(function_as_predicate_unittest, TestConversionFromLambdaToFunctionPointer)
{
  function<bool, int, int> func(
    [](int a, int b) -> bool {
      return function_as_predicate_unittest::func(a, b);
    }
  );

  bool(*f)(int, int) = func;

  bool actual_result = f(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(function_as_predicate_unittest, TestCopyConstructor)
{
  function<bool, int, int> func = function_as_predicate_unittest::func;

  function<bool, int, int> copied = func;

  bool actual_result = copied(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------


/****************** END OF `sneaker::functional::function` ********************/


// -----------------------------------------------------------------------------

class call_unittest : public function_as_call_unittest {};

// -----------------------------------------------------------------------------

TEST_F(call_unittest, TestInitializationFromLambdaAndCall)
{
  call func(
    []() -> void {
      return call_unittest::func();
    }
  );

  func();

  test_call();
}

// -----------------------------------------------------------------------------

TEST_F(call_unittest, TestInitializationFromLambdaAndCall2)
{
  call func = []() -> void {
    return call_unittest::func();
  };

  func();

  test_call();
}

// -----------------------------------------------------------------------------

TEST_F(call_unittest, TestInitializationFromStaticFunctionAndCall)
{
  call func = call_unittest::func;

  func();

  test_call();
}

// -----------------------------------------------------------------------------

TEST_F(call_unittest, TestExplicitConversionFromLambdaToStdFunction)
{
  call func(
    []() -> void {
      return call_unittest::func();
    }
  );

  std::function<void()> f = func;

  f();

  test_call();
}

// -----------------------------------------------------------------------------

TEST_F(call_unittest, TestExplicitConvertionFromStaticFunctionToStdFunction)
{
  call func = call_unittest::func;

  std::function<void()> f = func;

  f();

  test_call();
}

// -----------------------------------------------------------------------------

TEST_F(call_unittest, TestConversionFromStaticFunctionToFunctionPointer)
{
  call func = call_unittest::func;

  void(*f)(void) = func;

  f();

  test_call();
}

// -----------------------------------------------------------------------------

TEST_F(call_unittest, TestConversionFromLambdaToFunctionPointer)
{
  call func(
    []() -> void {
      return function_as_call_unittest::func();
    }
  );

  void(*f)(void) = func;

  f();

  test_call();
}

// -----------------------------------------------------------------------------

TEST_F(call_unittest, TestCopyConstructor)
{
  call func = call_unittest::func;

  call copied = func;

  copied();

  test_call();
}

// -----------------------------------------------------------------------------

TEST_F(call_unittest, TestInvokeAsync)
{
  call func = call_unittest::func;

  func.invoke_async();

  sleep(1);

  test_call();
}

// -----------------------------------------------------------------------------


/****************** END OF `sneaker::functional::call` ************************/


// -----------------------------------------------------------------------------

class action_unittest : public function_as_action_unittest {};

// -----------------------------------------------------------------------------

TEST_F(action_unittest, TestInitializationFromLambdaAndCall)
{
  action<const char*, const int, char*> func(
    [](const char* name, const int num, char*dst) -> void {
      return action_unittest::func(name, num, dst);
    }
  );

  func(m_name, m_num, m_dst);

  test_call(m_dst);
}

// -----------------------------------------------------------------------------

TEST_F(action_unittest, TestInitializationFromLambdaAndCall2)
{
  action<const char*, const int, char*> func = 
    [](const char* name, const int num, char*dst) -> void
  {
    return action_unittest::func(name, num, dst);
  };

  func(m_name, m_num, m_dst);

  test_call(m_dst);
}

// -----------------------------------------------------------------------------

TEST_F(action_unittest, TestInitializationFromStaticFunctionAndCall)
{
  action<const char*, const int, char*> func = action_unittest::func;

  func(m_name, m_num, m_dst);

  test_call(m_dst);
}

// -----------------------------------------------------------------------------

TEST_F(action_unittest, TestExplicitConversionFromLambdaToStdFunction)
{
  action<const char*, const int, char*> func(
    [](const char* name, const int num, char* dst) -> void {
      return action_unittest::func(name, num, dst);
    }
  );

  std::function<void(const char*, const int, char*)> f = func;

  f(m_name, m_num, m_dst);

  test_call(m_dst);
}

// -----------------------------------------------------------------------------

TEST_F(action_unittest, TestExplicitConvertionFromStaticFunctionToStdFunction)
{
  action<const char*, const int, char*> func = action_unittest::func;

  std::function<void(const char*, const int, char*)> f = func;

  f(m_name, m_num, m_dst);

  test_call(m_dst);
}

// -----------------------------------------------------------------------------

TEST_F(action_unittest, TestConversionFromStaticFunctionToFunctionPointer)
{
  action<const char*, const int, char*> func = action_unittest::func;

  void(*f)(const char*, const int, char*) = func;

  f(m_name, m_num, m_dst);

  test_call(m_dst);
}

// -----------------------------------------------------------------------------

TEST_F(action_unittest, TestConversionFromLambdaToFunctionPointer)
{
  action<const char*, const int, char*> func(
    [](const char* name, const int num, char* dst) -> void {
      return action_unittest::func(name, num, dst);
    }
  );

  void(*f)(const char*, const int, char*) = func;

  f(m_name, m_num, m_dst);

  test_call(m_dst);
}

// -----------------------------------------------------------------------------

TEST_F(action_unittest, TestCopyConstructor)
{
  action<const char*, const int, char*> func = action_unittest::func;

  action<const char*, const int, char*> copied = func;

  copied(m_name, m_num, m_dst);

  test_call(m_dst);
}

// -----------------------------------------------------------------------------

TEST_F(action_unittest, TestInvokeAsync)
{
  action<const char*, const int, char*> func = action_unittest::func;

  func.invoke_async(m_name, m_num, m_dst);

  sleep(1);

  test_call(m_dst);
}

// -----------------------------------------------------------------------------


/****************** END OF `sneaker::functional::action` **********************/


// -----------------------------------------------------------------------------

class predicate_unittest : public function_as_predicate_unittest {};

// -----------------------------------------------------------------------------

TEST_F(predicate_unittest, TestInitializationFromLambdaAndCall)
{
  predicate<int, int> func(
    [](int a, int b) -> bool {
      return predicate_unittest::func(a, b);
    }
  );

  int actual_result = func(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(predicate_unittest, TestInitializationFromLambdaAndCall2)
{
  predicate<int, int> func =
    [](int a, int b) -> bool
  {
    return predicate_unittest::func(a, b);
  };

  int actual_result = func(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(predicate_unittest, TestInitializationFromStaticFunctionAndCall)
{
  predicate<int, int> func = predicate_unittest::func;

  int actual_result = func(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(predicate_unittest, TestExplicitConversionFromLambdaToStdFunction)
{
  predicate<int, int> func(
    [](int a, int b) -> bool {
      return predicate_unittest::func(a, b);
    }
  );

  std::function<bool(int, int)> f = func;

  bool actual_result = f(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(predicate_unittest, TestExplicitConvertionFromStaticFunctionToStdFunction)
{
  predicate<int, int> func = predicate_unittest::func;

  std::function<bool(int, int)> f = func;

  bool actual_result = f(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(predicate_unittest, TestConversionFromStaticFunctionToFunctionPointer)
{
  predicate<int, int> func = predicate_unittest::func;

  bool(*f)(int, int) = func;

  bool actual_result = f(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(predicate_unittest, TestConversionFromLambdaToFunctionPointer)
{
  predicate<int, int> func(
    [](int a, int b) -> bool {
      return predicate_unittest::func(a, b);
    }
  );

  bool(*f)(int, int) = func;

  bool actual_result = f(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(predicate_unittest, TestCopyConstructor)
{
  predicate<int, int> func = predicate_unittest::func;

  predicate<int, int> copied = func;

  bool actual_result = copied(a, b);

  test_call(actual_result);
}

// -----------------------------------------------------------------------------

/****************** END OF `sneaker::functional::predicate` *******************/

// -----------------------------------------------------------------------------
