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

/* Unit tests for src/libc/hash.c */


#include "../_unittest.h"
#include "../../include/libc/assert.h"
#include "../../include/libc/hash.h"


/********************************************
 * Test for:
 * hash_t hash_str(const char * str)
 ********************************************/
TEST(hash_strTest, HashNullStringTest) {
  ASSERT_EQ(hash_str(NULL), hash_str(NULL));
}

TEST(hash_strTest, HashOneCharacterStringTest1) {
  ASSERT_EQ(hash_str("c"), hash_str("c"));
}

TEST(hash_strTest, HashOneCharacterStringTest2) {
  ASSERT_NE(hash_str("a"), hash_str(NULL));
}

TEST(hash_strTest, HashOneCharacterStringTest3) {
  ASSERT_NE(hash_str("0"), hash_str("z"));
}

TEST(hash_strTest, HashShortStringTest1) {
  ASSERT_EQ(hash_str("hash string"), hash_str("hash string"));
}

TEST(hash_strTest, HashShortStringTest2) {
  ASSERT_NE(hash_str("hash another string"), hash_str(NULL)); 
}

TEST(hash_strTest, HashShortStringTest3) {
  ASSERT_EQ(hash_str("abbcccbba"), hash_str("abbcccbba"));
}

TEST(hash_strTest, HashShortStringTest4) {
  ASSERT_EQ(hash_str("   "), hash_str("   "));
}

TEST(hash_strTest, HashShortStringTest5) {
  ASSERT_NE(hash_str(" "), hash_str("     "));
}

TEST(hash_strTest, HashShortStringTest6) {
  ASSERT_EQ(hash_str(""), hash_str(NULL));
}

TEST(hash_strTest, HashLongStringTest1) {
  char str[] = "hash a string to test hash a string then hash another string";
  ASSERT_EQ(
    hash_str(str),
    hash_str(str)
  );
}

TEST(hash_strTest, HashLongStringTest2) {
  ASSERT_NE(
    hash_str("put .gitignore in .gitignore to make git ignore .gitignore"),
    hash_str(NULL)
  ); 
}

TEST(hash_strTest, HashLongStringTest3) {
  char palindrome[] = "abbcccddddeeeeeffffffgggggggffffffeeeeeddddcccbba";
  ASSERT_EQ(
    hash_str(palindrome),
    hash_str(palindrome)
  );
}

TEST(hash_strTest, HashLongStringTest4) {
  ASSERT_EQ(
    hash_str("                                                               "),
    hash_str("                                                               ")
  );
}

TEST(hash_strTest, HashLongStringTest5) {
  ASSERT_NE(
    hash_str(" "),
    hash_str("                                                               ")
  );
}

TEST(hash_strTest, HashLongStringTest6) {
  ASSERT_NE(
    hash_str("                                                               "),
    hash_str(NULL)
  );
}

TEST(hash_strTest, HashLongStringTest7) {
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
    hash_str(str1),
    hash_str(str2)
  );
}

/********************************************
 * Test for:
 * hash_t hash32shift(unsigned int)
 ********************************************/
TEST(hash32shiftTest, HashOnSameKeyTest) {
  unsigned int key = 6688;
  ASSERT_EQ(
    hash32shift(key),
    hash32shift(key)
  );
}

TEST(hash32shiftTest, HashOnDifferentKeyTest) {
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
 * Test for:
 * hash_t hash64shift(unsigned int)
 ********************************************/
TEST(hash64shiftTest, HashOnSameKeyTest) {
  unsigned long key = 66888888;
  ASSERT_EQ(
    hash64shift(key),
    hash64shift(key)
  );
}

TEST(hash64shiftTest, HashOnDifferentKeyTest) {
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
TEST(hash_str_jenkins_one_at_a_timeTest, HashNullStringTest) {
  ASSERT_EQ(
    hash_str_jenkins_one_at_a_time(NULL),
    hash_str_jenkins_one_at_a_time(NULL)
  );
}

TEST(hash_str_jenkins_one_at_a_timeTest, HashSameStringTest1) {
  const char str[] = "this is a test string!!!";

  ASSERT_EQ(
    hash_str_jenkins_one_at_a_time(str),
    hash_str_jenkins_one_at_a_time(str)
  );
}

TEST(hash_str_jenkins_one_at_a_timeTest, HashSameStringTest2) {
  const char str[] = "!@#$%^&*()_+{}";

  ASSERT_EQ(
    hash_str_jenkins_one_at_a_time(str),
    hash_str_jenkins_one_at_a_time(str)
  );
}

TEST(hash_str_jenkins_one_at_a_timeTest, HashSameStringTest3) {
  const char str[] = "";

  ASSERT_EQ(
    hash_str_jenkins_one_at_a_time(str),
    hash_str_jenkins_one_at_a_time(str)
  );
}

TEST(hash_str_jenkins_one_at_a_timeTest, HashDifferentStringTest1) {
  ASSERT_NE(
    hash_str_jenkins_one_at_a_time("test"),
    hash_str_jenkins_one_at_a_time("test test test test")
  );
}

TEST(hash_str_jenkins_one_at_a_timeTest, HashDifferentStringTest2) {
  ASSERT_NE(
    hash_str_jenkins_one_at_a_time(" "),
    hash_str_jenkins_one_at_a_time("")
  );
}

/**********************************************************
 * Test for:
 * hash_t hash_robert_jenkin(unsigned int)
 **********************************************************/
TEST(hash_robert_jenkinTest, HashSameNumberTest1) {
  ASSERT_EQ(
    hash_robert_jenkin(0),
    hash_robert_jenkin(0)
  );
}

TEST(hash_robert_jenkinTest, HashSameNumberTest2) {
  ASSERT_EQ(
    hash_robert_jenkin(9999),
    hash_robert_jenkin(9999)
  );
}

TEST(hash_robert_jenkinTest, HashDifferentNumberTest) {
  int i;
  for(i = 0; i < 499; i++) {
    ASSERT_NE(
      hash_robert_jenkin(i),
      hash_robert_jenkin(i*i+1)
    );
  }
}