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

/* Unit test for context management framework. */

#include <stdexcept>
#include <vector>
#include "../../include/testing/testing.h"
#include "../../include/context/context_manager.h"
#include "../../include/context/scoped_context.h"


using namespace sneaker::context;


class scoped_context_unittest : public ::testing::Test {
public:
  class dummy_context_manager : public context_manager {
    protected:
      virtual void __enter__() {
        this->truthy = true;
      }

      virtual void __exit__() {
        this->truthy = false;
      }

    public:
      bool truthy = false;
  };
};


TEST_F(scoped_context_unittest, TestDummyContextManager)
{
  scoped_context_unittest::dummy_context_manager mngr;

  ASSERT_EQ(false, mngr.truthy);

  scoped_context(
    &mngr,
    [](context_manager *mngr, bool expected_truthy) {
      scoped_context_unittest::dummy_context_manager *_mngr = \
        static_cast<scoped_context_unittest::dummy_context_manager*>(mngr);

      ASSERT_EQ(expected_truthy, _mngr->truthy);
    },
    true
  );

  ASSERT_EQ(false, mngr.truthy);
}

TEST_F(scoped_context_unittest, TestExceptionInContext)
{
  scoped_context_unittest::dummy_context_manager mngr;

  ASSERT_THROW(
    {
      sneaker::context::scoped_context(
        &mngr,
        [](context_manager *mngr) {
          throw std::runtime_error("Helle world!");
        }
      );
    },
    std::runtime_error
  );
}


class nested_context_unittest : public ::testing::Test {
public:
  static int count;

  class dummy_context_manager : public context_manager {
    protected:
      virtual void __enter__() {
        nested_context_unittest::count++;
      }

      virtual void __exit__() {
        nested_context_unittest::count--;
      }
  };
};
int nested_context_unittest::count = 0;


TEST_F(nested_context_unittest, TestNestedManager)
{
  ASSERT_EQ(0, nested_context_unittest::count);

  nested_context(
    {
      new dummy_context_manager(),
      new dummy_context_manager(),
      new dummy_context_manager()
    },
    [](std::vector<context_manager*>& context_managers) {
      ASSERT_EQ(3, nested_context_unittest::count);
    }
  );

  ASSERT_EQ(0, nested_context_unittest::count);
}

TEST_F(nested_context_unittest, TestExceptionInContext)
{
  ASSERT_THROW(
    {
      nested_context(
        {
          new dummy_context_manager(),
          new dummy_context_manager(),
          new dummy_context_manager()
        },
        [](std::vector<context_manager*>& context_managers) {
          throw std::runtime_error("Helle world!");
        }
      );
    },
    std::runtime_error
  );
}


class safe_scoped_context_unittest : public ::testing::Test {
public:
  class dummy_context_manager : public context_manager {
    protected:
      virtual void __enter__() {
        this->truthy = true;
      }

      virtual void __exit__() {
        this->truthy = false;
      }

    public:
      bool truthy = false;
  };
};


TEST_F(safe_scoped_context_unittest, TestDummyContextManager)
{
  safe_scoped_context_unittest::dummy_context_manager mngr;

  ASSERT_EQ(false, mngr.truthy);

  safe_scoped_context(
    &mngr,
    [](context_manager *mngr, bool expected_truthy) {
      safe_scoped_context_unittest::dummy_context_manager *_mngr = \
        static_cast<safe_scoped_context_unittest::dummy_context_manager*>(mngr);

      ASSERT_EQ(expected_truthy, _mngr->truthy);
    },
    true
  );

  ASSERT_EQ(false, mngr.truthy);
}

TEST_F(safe_scoped_context_unittest, TestExceptionInContext)
{
  safe_scoped_context_unittest::dummy_context_manager mngr;

  ASSERT_NO_THROW(
    {
      sneaker::context::safe_scoped_context(
        &mngr,
        [](context_manager *mngr) {
          throw std::runtime_error("Helle world!");
        }
      );
    }
  );
}


class safe_nested_context_unittest : public ::testing::Test {
public:
  static int count;

  class dummy_context_manager : public context_manager {
    protected:
      virtual void __enter__() {
        safe_nested_context_unittest::count++;
      }

      virtual void __exit__() {
        safe_nested_context_unittest::count--;
      }
  };
};
int safe_nested_context_unittest::count = 0;


TEST_F(safe_nested_context_unittest, TestNestedManager)
{
  ASSERT_EQ(0, safe_nested_context_unittest::count);

  nested_context(
    {
      new dummy_context_manager(),
      new dummy_context_manager(),
      new dummy_context_manager()
    },
    [](std::vector<context_manager*>& context_managers) {
      ASSERT_EQ(3, safe_nested_context_unittest::count);
    }
  );

  ASSERT_EQ(0, safe_nested_context_unittest::count);
}

TEST_F(safe_nested_context_unittest, TestExceptionInContext)
{
  ASSERT_NO_THROW(
    {
      safe_nested_context(
        {
          new dummy_context_manager(),
          new dummy_context_manager(),
          new dummy_context_manager()
        },
        [](std::vector<context_manager*>& context_managers) {
          throw std::runtime_error("Helle world!");
        }
      );
    }
  );
}