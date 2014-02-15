/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2014 Yanzheng Li

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

/* Unit test for `sneaker::functional::function` defined in
 * include/functional/function.h */

#include <stdio.h>
#include <functional>
#include "../../include/testing/testing.h"
#include "../../include/functional/function.h"


using namespace sneaker::functional;


class FunctionUnitTestBase : public ::testing::Test {};


class FunctionAsCallUnitTest : public FunctionUnitTestBase {
public:
  static int expected_result;
  static int actual_result;

  static void func() {
    FunctionAsCallUnitTest::actual_result = FunctionAsCallUnitTest::expected_result;
  }

  void test_call() {
    ASSERT_EQ(
      FunctionAsCallUnitTest::expected_result,
      FunctionAsCallUnitTest::actual_result
    );
  }

protected:
  virtual void TearDown() {
    FunctionAsCallUnitTest::actual_result = 0;
  }
};

int FunctionAsCallUnitTest::expected_result = 25257758;
int FunctionAsCallUnitTest::actual_result = 0;


TEST_F(FunctionAsCallUnitTest, TestInitializationFromLambdaAndCall)
{
  function<void> func(
    []() -> void {
      return FunctionAsCallUnitTest::func();
    }
  );

  func();

  test_call();
}

TEST_F(FunctionAsCallUnitTest, TestInitializationFromLambdaAndCall2)
{
  function<void> func = []() -> void {
    return FunctionAsCallUnitTest::func();
  };

  func();

  test_call();
}

TEST_F(FunctionAsCallUnitTest, TestInitializationFromStaticFunctionAndCall)
{
  function<void> func = FunctionAsCallUnitTest::func;

  func();

  test_call();
}

TEST_F(FunctionAsCallUnitTest, TestExplicitConversionFromLambdaToStdFunction)
{
  function<void> func(
    []() -> void {
      return FunctionAsCallUnitTest::func();
    }
  );

  std::function<void()> f = func;

  f();

  test_call();
}

TEST_F(FunctionAsCallUnitTest, TestExplicitConvertionFromStaticFunctionToStdFunction)
{
  function<void> func = FunctionAsCallUnitTest::func;

  std::function<void()> f = func;

  f();

  test_call();
}

TEST_F(FunctionAsCallUnitTest, TestConversionFromStaticFunctionToFunctionPointer)
{
  function<void> func = FunctionAsCallUnitTest::func;

  void(*f)(void) = func;

  f();

  test_call();
}

TEST_F(FunctionAsCallUnitTest, TestConversionFromLambdaToFunctionPointer)
{
  function<void> func(
    []() -> void {
      return FunctionAsCallUnitTest::func();
    }
  );

  void(*f)(void) = func;

  f();

  test_call();
}

TEST_F(FunctionAsCallUnitTest, TestCopyConstructor)
{
  function<void> func = FunctionAsCallUnitTest::func;

  function<void> copied = func;

  copied();

  test_call();
}

TEST_F(FunctionAsCallUnitTest, TestInvokeAsync)
{
  function<void> func = FunctionAsCallUnitTest::func;

  func.invoke_async();

  sleep(1);

  test_call();
}


class FunctionAsActionUnitTest : public FunctionUnitTestBase {
public:
  static void func(const char* name, const int num, char*dst) {
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
    dst = (char*)malloc(50);
  }

  virtual void TearDown() {
    free(dst);
    dst = NULL;
  }

  const char * name = "Jimmy";
  const int num = 8;
  char * dst = NULL;
};


TEST_F(FunctionAsActionUnitTest, TestInitializationFromLambdaAndCall)
{
  function<void, const char*, const int, char*> func(
    [](const char* name, const int num, char*dst) -> void {
      return FunctionAsActionUnitTest::func(name, num, dst);
    }
  );

  func(name, num, dst);

  test_call(dst);
}

TEST_F(FunctionAsActionUnitTest, TestInitializationFromLambdaAndCall2)
{
  function<void, const char*, const int, char*> func = 
    [](const char* name, const int num, char*dst) -> void
  {
    return FunctionAsActionUnitTest::func(name, num, dst);
  };

  func(name, num, dst);

  test_call(dst);
}

TEST_F(FunctionAsActionUnitTest, TestInitializationFromStaticFunctionAndCall)
{
  function<void, const char*, const int, char*> func = FunctionAsActionUnitTest::func;

  func(name, num, dst);

  test_call(dst);
}

TEST_F(FunctionAsActionUnitTest, TestExplicitConversionFromLambdaToStdFunction)
{
  function<void, const char*, const int, char*> func(
    [](const char* name, const int num, char* dst) -> void {
      return FunctionAsActionUnitTest::func(name, num, dst);
    }
  );

  std::function<void(const char*, const int, char*)> f = func;

  f(name, num, dst);

  test_call(dst);
}

TEST_F(FunctionAsActionUnitTest, TestExplicitConvertionFromStaticFunctionToStdFunction)
{
  function<void, const char*, const int, char*> func = FunctionAsActionUnitTest::func;

  std::function<void(const char*, const int, char*)> f = func;

  f(name, num, dst);

  test_call(dst);
}

TEST_F(FunctionAsActionUnitTest, TestConversionFromStaticFunctionToFunctionPointer)
{
  function<void, const char*, const int, char*> func = FunctionAsActionUnitTest::func;

  void(*f)(const char*, const int, char*) = func;

  f(name, num, dst);

  test_call(dst);
}

TEST_F(FunctionAsActionUnitTest, TestConversionFromLambdaToFunctionPointer)
{
  function<void, const char*, const int, char*> func(
    [](const char* name, const int num, char* dst) -> void {
      return FunctionAsActionUnitTest::func(name, num, dst);
    }
  );

  void(*f)(const char*, const int, char*) = func;

  f(name, num, dst);

  test_call(dst);
}

TEST_F(FunctionAsActionUnitTest, TestCopyConstructor)
{
  function<void, const char*, const int, char*> func = FunctionAsActionUnitTest::func;

  function<void, const char*, const int, char*> copied = func;

  copied(name, num, dst);

  test_call(dst);
}

TEST_F(FunctionAsActionUnitTest, TestInvokeAsync)
{
  function<void, const char*, const int, char*> func = FunctionAsActionUnitTest::func;

  func.invoke_async(name, num, dst);

  sleep(1);

  test_call(dst);
}


class FunctionAsFunctionUnitTest : public FunctionUnitTestBase {
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


TEST_F(FunctionAsFunctionUnitTest, TestInitializationFromLambdaAndCall)
{
  function<int, const int, const int> func(
    [](const int a, const int b) -> int {
      return FunctionAsFunctionUnitTest::func(a, b);
    }
  );

  int actual_result = func(a, b);

  test_call(actual_result);
}

TEST_F(FunctionAsFunctionUnitTest, TestInitializationFromLambdaAndCall2)
{
  function<int, const int, const int> func =
    [](const int a, const int b) -> int
  {
    return FunctionAsFunctionUnitTest::func(a, b);
  };

  int actual_result = func(a, b);

  test_call(actual_result);
}

TEST_F(FunctionAsFunctionUnitTest, TestInitializationFromStaticFunctionAndCall)
{
  function<int, const int, const int> func = FunctionAsFunctionUnitTest::func;

  int actual_result = func(a, b);

  test_call(actual_result);
}

TEST_F(FunctionAsFunctionUnitTest, TestExplicitConversionFromLambdaToStdFunction)
{
  function<int, const int, const int> func(
    [](const int a, const int b) -> int {
      return FunctionAsFunctionUnitTest::func(a, b);
    }
  );

  std::function<int(const int, const int)> f = func;

  int actual_result = f(a, b);

  test_call(actual_result);
}

TEST_F(FunctionAsFunctionUnitTest, TestExplicitConvertionFromStaticFunctionToStdFunction)
{
  function<int, const int, const int> func = FunctionAsFunctionUnitTest::func;

  std::function<int(const int, const int)> f = func;

  int actual_result = f(a, b);

  test_call(actual_result);
}

TEST_F(FunctionAsFunctionUnitTest, TestConversionFromStaticFunctionToFunctionPointer)
{
  function<int, const int, const int> func = FunctionAsFunctionUnitTest::func;

  int(*f)(const int, const int) = func;

  int actual_result = f(a, b);

  test_call(actual_result);
}

TEST_F(FunctionAsFunctionUnitTest, TestConversionFromLambdaToFunctionPointer)
{
  function<int, const int, const int> func(
    [](const int a, const int b) -> int {
      return FunctionAsFunctionUnitTest::func(a, b);
    }
  );

  int(*f)(const int, const int) = func;

  int actual_result = f(a, b);

  test_call(actual_result);
}

TEST_F(FunctionAsFunctionUnitTest, TestCopyConstructor)
{
  function<int, const int, const int> func = FunctionAsFunctionUnitTest::func;

  function<int, const int, const int> copied = func;

  int actual_result = copied(a, b);

  test_call(actual_result);
}


class FunctionAsPredicateUnitTest : public FunctionUnitTestBase {
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


TEST_F(FunctionAsPredicateUnitTest, TestInitializationFromLambdaAndCall)
{
  function<bool, int, int> func(
    [](int a, int b) -> bool {
      return FunctionAsPredicateUnitTest::func(a, b);
    }
  );

  int actual_result = func(a, b);

  test_call(actual_result);
}

TEST_F(FunctionAsPredicateUnitTest, TestInitializationFromLambdaAndCall2)
{
  function<bool, int, int> func =
    [](int a, int b) -> bool
  {
    return FunctionAsPredicateUnitTest::func(a, b);
  };

  int actual_result = func(a, b);

  test_call(actual_result);
}

TEST_F(FunctionAsPredicateUnitTest, TestInitializationFromStaticFunctionAndCall)
{
  function<bool, int, int> func = FunctionAsPredicateUnitTest::func;

  int actual_result = func(a, b);

  test_call(actual_result);
}

TEST_F(FunctionAsPredicateUnitTest, TestExplicitConversionFromLambdaToStdFunction)
{
  function<bool, int, int> func(
    [](int a, int b) -> bool {
      return FunctionAsPredicateUnitTest::func(a, b);
    }
  );

  std::function<bool(int, int)> f = func;

  bool actual_result = f(a, b);

  test_call(actual_result);
}

TEST_F(FunctionAsPredicateUnitTest, TestExplicitConvertionFromStaticFunctionToStdFunction)
{
  function<bool, int, int> func = FunctionAsPredicateUnitTest::func;

  std::function<bool(int, int)> f = func;

  bool actual_result = f(a, b);

  test_call(actual_result);
}

TEST_F(FunctionAsPredicateUnitTest, TestConversionFromStaticFunctionToFunctionPointer)
{
  function<bool, int, int> func = FunctionAsPredicateUnitTest::func;

  bool(*f)(int, int) = func;

  bool actual_result = f(a, b);

  test_call(actual_result);
}

TEST_F(FunctionAsPredicateUnitTest, TestConversionFromLambdaToFunctionPointer)
{
  function<bool, int, int> func(
    [](int a, int b) -> bool {
      return FunctionAsPredicateUnitTest::func(a, b);
    }
  );

  bool(*f)(int, int) = func;

  bool actual_result = f(a, b);

  test_call(actual_result);  
}

TEST_F(FunctionAsPredicateUnitTest, TestCopyConstructor)
{
  function<bool, int, int> func = FunctionAsPredicateUnitTest::func;

  function<bool, int, int> copied = func;

  bool actual_result = copied(a, b);

  test_call(actual_result);
}


/****************** END OF `sneaker::functional::function` ********************/


class CallUnitTest : public FunctionAsCallUnitTest {};


TEST_F(CallUnitTest, TestInitializationFromLambdaAndCall)
{
  call func(
    []() -> void {
      return CallUnitTest::func();
    }
  );

  func();

  test_call();
}

TEST_F(CallUnitTest, TestInitializationFromLambdaAndCall2)
{
  call func = []() -> void {
    return CallUnitTest::func();
  };

  func();

  test_call();
}

TEST_F(CallUnitTest, TestInitializationFromStaticFunctionAndCall)
{
  call func = CallUnitTest::func;

  func();

  test_call();
}

TEST_F(CallUnitTest, TestExplicitConversionFromLambdaToStdFunction)
{
  call func(
    []() -> void {
      return CallUnitTest::func();
    }
  );

  std::function<void()> f = func;

  f();

  test_call();
}

TEST_F(CallUnitTest, TestExplicitConvertionFromStaticFunctionToStdFunction)
{
  call func = CallUnitTest::func;

  std::function<void()> f = func;

  f();

  test_call();
}

TEST_F(CallUnitTest, TestConversionFromStaticFunctionToFunctionPointer)
{
  call func = CallUnitTest::func;

  void(*f)(void) = func;

  f();

  test_call();
}

TEST_F(CallUnitTest, TestConversionFromLambdaToFunctionPointer)
{
  call func(
    []() -> void {
      return FunctionAsCallUnitTest::func();
    }
  );

  void(*f)(void) = func;

  f();

  test_call();
}

TEST_F(CallUnitTest, TestCopyConstructor)
{
  call func = CallUnitTest::func;

  call copied = func;

  copied();

  test_call();
}

TEST_F(CallUnitTest, TestInvokeAsync)
{
  call func = CallUnitTest::func;

  func.invoke_async();

  sleep(1);

  test_call();
}


/****************** END OF `sneaker::functional::call` ************************/


class ActionUnitTest : public FunctionAsActionUnitTest {};


TEST_F(ActionUnitTest, TestInitializationFromLambdaAndCall)
{
  action<const char*, const int, char*> func(
    [](const char* name, const int num, char*dst) -> void {
      return ActionUnitTest::func(name, num, dst);
    }
  );

  func(name, num, dst);

  test_call(dst);
}

TEST_F(ActionUnitTest, TestInitializationFromLambdaAndCall2)
{
  action<const char*, const int, char*> func = 
    [](const char* name, const int num, char*dst) -> void
  {
    return ActionUnitTest::func(name, num, dst);
  };

  func(name, num, dst);

  test_call(dst);
}

TEST_F(ActionUnitTest, TestInitializationFromStaticFunctionAndCall)
{
  action<const char*, const int, char*> func = ActionUnitTest::func;

  func(name, num, dst);

  test_call(dst);
}

TEST_F(ActionUnitTest, TestExplicitConversionFromLambdaToStdFunction)
{
  action<const char*, const int, char*> func(
    [](const char* name, const int num, char* dst) -> void {
      return ActionUnitTest::func(name, num, dst);
    }
  );

  std::function<void(const char*, const int, char*)> f = func;

  f(name, num, dst);

  test_call(dst);
}

TEST_F(ActionUnitTest, TestExplicitConvertionFromStaticFunctionToStdFunction)
{
  action<const char*, const int, char*> func = ActionUnitTest::func;

  std::function<void(const char*, const int, char*)> f = func;

  f(name, num, dst);

  test_call(dst);
}

TEST_F(ActionUnitTest, TestConversionFromStaticFunctionToFunctionPointer)
{
  action<const char*, const int, char*> func = ActionUnitTest::func;

  void(*f)(const char*, const int, char*) = func;

  f(name, num, dst);

  test_call(dst);
}

TEST_F(ActionUnitTest, TestConversionFromLambdaToFunctionPointer)
{
  action<const char*, const int, char*> func(
    [](const char* name, const int num, char* dst) -> void {
      return ActionUnitTest::func(name, num, dst);
    }
  );

  void(*f)(const char*, const int, char*) = func;

  f(name, num, dst);

  test_call(dst);
}

TEST_F(ActionUnitTest, TestCopyConstructor)
{
  action<const char*, const int, char*> func = ActionUnitTest::func;

  action<const char*, const int, char*> copied = func;

  copied(name, num, dst);

  test_call(dst);
}

TEST_F(ActionUnitTest, TestInvokeAsync)
{
  action<const char*, const int, char*> func = ActionUnitTest::func;

  func.invoke_async(name, num, dst);

  sleep(1);

  test_call(dst);
}


/****************** END OF `sneaker::functional::action` **********************/


class PredicateUnitTest : public FunctionAsPredicateUnitTest {};


TEST_F(PredicateUnitTest, TestInitializationFromLambdaAndCall)
{
  predicate<int, int> func(
    [](int a, int b) -> bool {
      return PredicateUnitTest::func(a, b);
    }
  );

  int actual_result = func(a, b);

  test_call(actual_result);
}

TEST_F(PredicateUnitTest, TestInitializationFromLambdaAndCall2)
{
  predicate<int, int> func =
    [](int a, int b) -> bool
  {
    return PredicateUnitTest::func(a, b);
  };

  int actual_result = func(a, b);

  test_call(actual_result);
}

TEST_F(PredicateUnitTest, TestInitializationFromStaticFunctionAndCall)
{
  predicate<int, int> func = PredicateUnitTest::func;

  int actual_result = func(a, b);

  test_call(actual_result);
}

TEST_F(PredicateUnitTest, TestExplicitConversionFromLambdaToStdFunction)
{
  predicate<int, int> func(
    [](int a, int b) -> bool {
      return PredicateUnitTest::func(a, b);
    }
  );

  std::function<bool(int, int)> f = func;

  bool actual_result = f(a, b);

  test_call(actual_result);
}

TEST_F(PredicateUnitTest, TestExplicitConvertionFromStaticFunctionToStdFunction)
{
  predicate<int, int> func = PredicateUnitTest::func;

  std::function<bool(int, int)> f = func;

  bool actual_result = f(a, b);

  test_call(actual_result);
}

TEST_F(PredicateUnitTest, TestConversionFromStaticFunctionToFunctionPointer)
{
  predicate<int, int> func = PredicateUnitTest::func;

  bool(*f)(int, int) = func;

  bool actual_result = f(a, b);

  test_call(actual_result);
}

TEST_F(PredicateUnitTest, TestConversionFromLambdaToFunctionPointer)
{
  predicate<int, int> func(
    [](int a, int b) -> bool {
      return PredicateUnitTest::func(a, b);
    }
  );

  bool(*f)(int, int) = func;

  bool actual_result = f(a, b);

  test_call(actual_result);  
}

TEST_F(PredicateUnitTest, TestCopyConstructor)
{
  predicate<int, int> func = PredicateUnitTest::func;

  predicate<int, int> copied = func;

  bool actual_result = copied(a, b);

  test_call(actual_result);
}


/****************** END OF `sneaker::functional::predicate` *******************/