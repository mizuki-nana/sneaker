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

/* Unit tests for functions in include/libc/hash.h */


#include "../_unittest.h"
#include "../../include/libc/assert.h"
#include "../../include/libc/hash.h"


/********************************************
 * Unit test for:
 * hash_t linear_horners_rule_str_hash(const char * str)
 ********************************************/
class LinearHornersRuleStrHashUnitTest : public ::testing::Test {};


TEST_F(LinearHornersRuleStrHashUnitTest, TestHashNullString)
{
  ASSERT_EQ(
    linear_horners_rule_str_hash(NULL),
    linear_horners_rule_str_hash(NULL)
  );
}

TEST_F(LinearHornersRuleStrHashUnitTest, TestHashOneCharacterString)
{
  ASSERT_EQ(
    linear_horners_rule_str_hash("c"),
    linear_horners_rule_str_hash("c")
  );
}

TEST_F(LinearHornersRuleStrHashUnitTest, TestHashSameString_1)
{
  ASSERT_EQ(
    linear_horners_rule_str_hash("hash string"),
    linear_horners_rule_str_hash("hash string")
  );
}

TEST_F(LinearHornersRuleStrHashUnitTest, TestHashSameString_2)
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


/********************************************
 * Unit test for:
 * hash_t log_horners_rule_str_hash(const char * str)
 ********************************************/
class LogHornersRuleStrHashUnitTest : public ::testing::Test {};


TEST_F(LogHornersRuleStrHashUnitTest, TestHashNullString)
{
  ASSERT_EQ(log_horners_rule_str_hash(NULL), log_horners_rule_str_hash(NULL));
}

TEST_F(LogHornersRuleStrHashUnitTest, TestHashOneCharacterString)
{
  ASSERT_EQ(log_horners_rule_str_hash("c"), log_horners_rule_str_hash("c"));
}

TEST_F(LogHornersRuleStrHashUnitTest, TestHashSameString_1)
{
  ASSERT_EQ(
    log_horners_rule_str_hash("hash string"),
    log_horners_rule_str_hash("hash string")
  );
}

TEST_F(LogHornersRuleStrHashUnitTest, TestHashSameString_2)
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
    log_horners_rule_str_hash(str1),
    log_horners_rule_str_hash(str2)
  );
}


/********************************************
 * Unit test for:
 * hash_t hash32shift(unsigned int)
 ********************************************/
class Hash32ShiftUnitTest : public ::testing::Test {};


TEST_F(Hash32ShiftUnitTest, TestHashOnSameKey)
{
  unsigned int key = 6688;
  ASSERT_EQ(
    hash32shift(key),
    hash32shift(key)
  );
}

TEST_F(Hash32ShiftUnitTest, TestHashOnDifferentKey)
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


/********************************************
 * Unit test for:
 * hash_t hash64shift(unsigned int)
 ********************************************/
class Hash64ShiftUnitTest : public ::testing::Test {};


TEST_F(Hash64ShiftUnitTest, TestHashOnSameKey)
{
  unsigned long key = 66888888;
  ASSERT_EQ(
    hash64shift(key),
    hash64shift(key)
  );
}

TEST_F(Hash64ShiftUnitTest, TestHashOnDifferentKey)
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


/**********************************************************
 * Test for:
 * hash_t hash_str_jenkins_one_at_a_time(const char * str)
 **********************************************************/
class HashStrJenkinsOneAtATimeUnitTest : public ::testing::Test {};


TEST_F(HashStrJenkinsOneAtATimeUnitTest, TestHashNullString)
{
  ASSERT_EQ(
    hash_str_jenkins_one_at_a_time(NULL),
    hash_str_jenkins_one_at_a_time(NULL)
  );
}

TEST_F(HashStrJenkinsOneAtATimeUnitTest, TestHashSameString)
{
  const char str[] = "this is a test string!!!";

  ASSERT_EQ(
    hash_str_jenkins_one_at_a_time(str),
    hash_str_jenkins_one_at_a_time(str)
  );
}

TEST_F(HashStrJenkinsOneAtATimeUnitTest, TestHashDifferentString_1)
{
  ASSERT_NE(
    hash_str_jenkins_one_at_a_time("test"),
    hash_str_jenkins_one_at_a_time("test test test test")
  );
}

TEST_F(HashStrJenkinsOneAtATimeUnitTest, TestHashDifferentString_2)
{
  ASSERT_NE(
    hash_str_jenkins_one_at_a_time(" "),
    hash_str_jenkins_one_at_a_time("")
  );
}


/**********************************************************
 * Unit test for:
 * hash_t hash_robert_jenkin(unsigned int)
 **********************************************************/
class HashRobertJenkinUnitTest : public ::testing::Test {};


TEST_F(HashRobertJenkinUnitTest, TestHashSameNumberTest_1)
{
  ASSERT_EQ(
    hash_robert_jenkin(0),
    hash_robert_jenkin(0)
  );
}

TEST_F(HashRobertJenkinUnitTest, TestHashSameNumberTest_2)
{
  ASSERT_EQ(
    hash_robert_jenkin(9999),
    hash_robert_jenkin(9999)
  );
}

TEST_F(HashRobertJenkinUnitTest, TestHashDifferentNumberTest)
{
  int i;
  for(i = 0; i < 499; i++) {
    ASSERT_NE(
      hash_robert_jenkin(i),
      hash_robert_jenkin(i*i+1)
    );
  }
}