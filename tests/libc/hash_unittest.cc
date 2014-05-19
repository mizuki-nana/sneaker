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

/* Unit tests for functions in sneaker/libc/hash.h */

#include <unordered_set>
#include "../../include/testing/testing.h"
#include "../../include/libc/hash.h"


/*******************************************************************************
 * Unit test for:
 * hash_t linear_horners_rule_str_hash(const char * str)
 ******************************************************************************/
class linear_horners_rule_str_hash_unittest : public ::testing::Test {};


TEST_F(linear_horners_rule_str_hash_unittest, TestHashNullString)
{
  ASSERT_EQ(
    linear_horners_rule_str_hash(NULL),
    linear_horners_rule_str_hash(NULL)
  );
}

TEST_F(linear_horners_rule_str_hash_unittest, TestHashOneCharacterString)
{
  ASSERT_EQ(
    linear_horners_rule_str_hash("c"),
    linear_horners_rule_str_hash("c")
  );
}

TEST_F(linear_horners_rule_str_hash_unittest, TestHashSameString_1)
{
  ASSERT_EQ(
    linear_horners_rule_str_hash("hash string"),
    linear_horners_rule_str_hash("hash string")
  );
}

TEST_F(linear_horners_rule_str_hash_unittest, TestHashSameString_2)
{
  char str1[] = "HEX is a multi-paradigm programming language that supports "
    "both static and dynamic types, and was designed with the core principles "
    "of simplicity, readability, versatility and scalability to allow "
    "developers to create a diversity of types of computer programs with "
    "modern language features, succinct syntax and semantics that are built "
    "into the core of the language construct.";

  char str2[] = "HEX syntax and semantics resembles a lot of the modern "
    "programming languages that you've probably heard or even used. "
    "For example, HEX has a indented syntax very much like Python. "
    "It supports objected-oriented programming with class and struct "
    "definitions very much like C++ and Java. It utilizes basic control "
    "statements such as if, for, and while just like pretty much every other "
    "languages out there. However, HEX does have its own syntax and semantics "
    "for aspects of the language that might not seen familiar to you.";

  ASSERT_NE(
    linear_horners_rule_str_hash(str1),
    linear_horners_rule_str_hash(str2)
  );
}

TEST_F(linear_horners_rule_str_hash_unittest, TestStress)
{
  const int TOP = 100000;
  std::unordered_set<int> set(TOP);

  for(int i = 0; i < TOP; i++) {
    char buf[10];
    snprintf(buf, sizeof(buf), "%d", i);
    unsigned long int hash = linear_horners_rule_str_hash(buf);
    ASSERT_EQ(set.end(), set.find(hash));
    set.insert(hash);
  }
}


/*******************************************************************************
 * Unit test for:
 * hash_t hash32shift(unsigned int)
 ******************************************************************************/
class hash32shift_unittest : public ::testing::Test {};


TEST_F(hash32shift_unittest, TestHashOnSameKey)
{
  unsigned int key = 6688;
  ASSERT_EQ(
    hash32shift(key),
    hash32shift(key)
  );
}

TEST_F(hash32shift_unittest, TestHashOnDifferentKey)
{
  int i;
  for(i = 0; i < 499; i++) {
    unsigned int key1 = (unsigned int)i;
    unsigned int key2 = (unsigned int)((i*i)+1);

    ASSERT_NE(
      hash32shift(key1),
      hash32shift(key2)
    );
  }
}

TEST_F(hash32shift_unittest, TestStress)
{
  const int TOP = 500000;
  std::unordered_set<int> set(TOP);

  for(int i = 0; i < TOP; i++) {
    unsigned long int hash = hash32shift(i);
    ASSERT_EQ(set.end(), set.find(hash));
    set.insert(hash);
  }
}


/*******************************************************************************
 * Unit test for:
 * hash_t hash64shift(unsigned int)
 ******************************************************************************/
class hash64shift_unittest : public ::testing::Test {};


TEST_F(hash64shift_unittest, TestHashOnSameKey)
{
  unsigned long key = 66888888;
  ASSERT_EQ(
    hash64shift(key),
    hash64shift(key)
  );
}

TEST_F(hash64shift_unittest, TestHashOnDifferentKey)
{
  int i;
  for(i = 0; i < 9999; i++) {
    unsigned long key1 = (unsigned long)i;
    unsigned long key2 = (unsigned long)(i*i+1);

    ASSERT_NE(
      hash64shift(key1),
      hash64shift(key2)
    );
  }
}

TEST_F(hash64shift_unittest, TestStress)
{
  const unsigned long TOP = 500000;
  std::unordered_set<unsigned long> set(TOP);

  for(unsigned long i = 0; i < TOP; i++) {
    unsigned long int hash = hash64shift(i);
    ASSERT_EQ(set.end(), set.find(hash));
    set.insert(hash);
  }
}


/*******************************************************************************
 * Unit Test for:
 * hash_t hash_str_jenkins_one_at_a_time(const char * str)
 ******************************************************************************/
class hash_str_jenkins_one_at_a_time_unittest : public ::testing::Test {};


TEST_F(hash_str_jenkins_one_at_a_time_unittest, TestHashNullString)
{
  ASSERT_EQ(
    hash_str_jenkins_one_at_a_time(NULL),
    hash_str_jenkins_one_at_a_time(NULL)
  );
}

TEST_F(hash_str_jenkins_one_at_a_time_unittest, TestHashSameString)
{
  const char str[] = "this is a test string!!!";

  ASSERT_EQ(
    hash_str_jenkins_one_at_a_time(str),
    hash_str_jenkins_one_at_a_time(str)
  );
}

TEST_F(hash_str_jenkins_one_at_a_time_unittest, TestHashDifferentString_1)
{
  ASSERT_NE(
    hash_str_jenkins_one_at_a_time("test"),
    hash_str_jenkins_one_at_a_time("test test test test")
  );
}

TEST_F(hash_str_jenkins_one_at_a_time_unittest, TestHashDifferentString_2)
{
  ASSERT_NE(
    hash_str_jenkins_one_at_a_time(" "),
    hash_str_jenkins_one_at_a_time("")
  );
}

TEST_F(hash_str_jenkins_one_at_a_time_unittest, TestStress)
{
  const unsigned long TOP = 500000;
  std::unordered_set<unsigned long> set(TOP);

  for(unsigned long i = 0; i < TOP; i++) {
    char buf[10];
    snprintf(buf, sizeof(buf), "%lu", i);
    unsigned long int hash = hash_str_jenkins_one_at_a_time(buf);
    ASSERT_EQ(set.end(), set.find(hash));
    set.insert(hash);
  }
}


/*******************************************************************************
 * Unit test for:
 * hash_t hash_robert_jenkin(unsigned int)
 ******************************************************************************/
class hash_robert_jenkin_unittest : public ::testing::Test {};


TEST_F(hash_robert_jenkin_unittest, TestHashSameNumberTest_1)
{
  ASSERT_EQ(
    hash_robert_jenkin(0),
    hash_robert_jenkin(0)
  );
}

TEST_F(hash_robert_jenkin_unittest, TestHashSameNumberTest_2)
{
  ASSERT_EQ(
    hash_robert_jenkin(9999),
    hash_robert_jenkin(9999)
  );
}

TEST_F(hash_robert_jenkin_unittest, TestHashDifferentNumberTest)
{
  int i;
  for(i = 0; i < 499; i++) {
    ASSERT_NE(
      hash_robert_jenkin(i),
      hash_robert_jenkin(i*i+1)
    );
  }
}

TEST_F(hash_robert_jenkin_unittest, TestStress)
{
  const unsigned long TOP = 500000;
  std::unordered_set<unsigned long> set(TOP);

  for(unsigned long i = 0; i < TOP; i++) {
    unsigned long int hash = hash_robert_jenkin(i);
    ASSERT_EQ(set.end(), set.find(hash));
    set.insert(hash);
  }
}